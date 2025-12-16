#include "util.h"
#include "preset.h"
#include "tablet.h"
#include "resources.h"

#define MAIN_WNDCLASSNAME       L"tabd"
#define TRAY_WNDCLASSNAME       L"tabd-tray"
#define TRAY_WM_ICON_MESSAGE    (WM_USER+1)
#define TRAY_WM_SHOW_MENU       (WM_USER+2)
#define TRAY_WM_ACTIVATE_PRESET (WM_USER+3)
#define TRAY_MENU_EXIT_ITEM     1
#define TRAY_MENU_PRESET_ITEM_0 100

static void InitThreadMessageQueue(void);
static void _Log(DWORD tid, PCWSTR file, int line, PCSTR func, PCWSTR message, ...);
#define Log(_message, ...) \
    _Log(GetCurrentThreadId(), __WFILE__, __LINE__, __func__, _message, ##__VA_ARGS__);

static void WinEventHookCallback(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
);
static LRESULT MainWindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

/* A whole separate thread with a hidden window and its message queue are dedicated for tray menu 
only because TrackPopupMenu() blocks the calling thread and sometimes fails if called from a thread 
different to which the parent window was created in. */
static DWORD WINAPI TrayThreadProc(HANDLE thread_ready);
static LRESULT TrayWindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

static void SetTrayIconTabletActiveStatus(bool active);

static bool TryInitTablet(PCWSTR path);
static bool BeginReadingTablet(void);
static void CleanUpTablet(void);
static DWORD CALLBACK DeviceChangedCallback(
    HCMNOTIFICATION       notification,
    PVOID                 arg,
    CM_NOTIFY_ACTION      action,
    PCM_NOTIFY_EVENT_DATA data,
    DWORD                 data_size
);

static void SynthesizeInput(const TabletReport *report);

static DWORD s_main_thread_id;
static HINSTANCE s_hinstance;
static HANDLE s_hconsole;
static POINT s_screen_size;
static HWINEVENTHOOK s_win_event_hook;

static CRITICAL_SECTION s_tray_lock;
static HANDLE s_tray_thread;
static DWORD s_tray_thread_id;
static NOTIFYICONDATAW s_tray_icon_data;

static CRITICAL_SECTION s_tablet_lock;
static OVERLAPPED s_tablet_overlapped;
static HCMNOTIFICATION s_device_notification;
static HANDLE s_tablet_handle = INVALID_HANDLE_VALUE;
static TabletInfo s_tablet_info;
static BYTE s_tablet_packet[1024];
static int s_tablet_preset_idx;
static TabletReport s_tablet_previous_report;
static HSYNTHETICPOINTERDEVICE s_ink_device;
static HWND s_ink_foreground_window;

void _start(void) {
    s_main_thread_id = GetCurrentThreadId();
    s_hinstance = GetModuleHandleW(0);
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        s_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    s_screen_size = (POINT){ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    s_ink_foreground_window = GetForegroundWindow();

    s_ink_device = CreateSyntheticPointerDevice(PT_PEN, 1, POINTER_FEEDBACK_DEFAULT);
    ASSERT(s_ink_device);

    InitThreadMessageQueue();

    WNDCLASSEXW wndclass = {
        .cbSize = sizeof(wndclass),
        .hInstance = s_hinstance,
        .lpszClassName = MAIN_WNDCLASSNAME,
        .lpfnWndProc = MainWindowEventHandler,
    };
    ASSERT(RegisterClassExW(&wndclass));
    HWND hwnd = CreateWindowExW(0, MAIN_WNDCLASSNAME, 0, 0, 0, 0, 0, 0, 0, 0, s_hinstance, 0);
    ASSERT(hwnd);

    s_win_event_hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,
        EVENT_SYSTEM_FOREGROUND,
        0,
        WinEventHookCallback,
        0,
        0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );
    ASSERT(s_win_event_hook);

    InitializeCriticalSection(&s_tray_lock);
    HANDLE thread_ready = CreateEventW(0, false, false, 0);
    s_tray_thread = CreateThread(0, 0, TrayThreadProc, thread_ready, 0, &s_tray_thread_id);
    ASSERT(WaitForSingleObject(thread_ready, INFINITE) == WAIT_OBJECT_0);

    InitializeCriticalSection(&s_tablet_lock);
    s_tablet_overlapped.hEvent = CreateEventW(0, false, false, 0);

    HDEVINFO devices = SetupDiGetClassDevsW(
        &GUID_DEVINTERFACE_HID, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT
    );
    for (int i = 0; ; i++) {
        SP_DEVICE_INTERFACE_DATA iface = { .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA) };
        if (!SetupDiEnumDeviceInterfaces(devices, 0, &GUID_DEVINTERFACE_HID, i, &iface))
            break;

        WCHAR details_buffer[1024] = {0};
        PSP_DEVICE_INTERFACE_DETAIL_DATA_W details = (void*)details_buffer;
        details->cbSize = sizeof(details);
        SetupDiGetDeviceInterfaceDetailW(
            devices, &iface, details, COUNTOF(details_buffer) - 1, 0, 0
        );

        if (TryInitTablet(details->DevicePath)) {
            break;
        }
    }

    CM_NOTIFY_FILTER filter = {
        .cbSize = sizeof(filter),
        .u.DeviceInterface.ClassGuid = GUID_DEVINTERFACE_HID,
        .FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE,
    };
    ASSERT(!CM_Register_Notification(&filter, 0, DeviceChangedCallback, &s_device_notification));

    for (bool is_running = true; is_running; ) {
        DWORD wait = MsgWaitForMultipleObjects(
            1, &s_tablet_overlapped.hEvent, false, INFINITE, QS_ALLINPUT
        );

        if (wait == WAIT_OBJECT_0) {
            DWORD packet_size = 0;
            BOOL packet_ready = GetOverlappedResult(
                s_tablet_handle, &s_tablet_overlapped, &packet_size,  false
            );

            if (!packet_ready || !BeginReadingTablet()) {
                Log(L"Tablet lost");
                CleanUpTablet();
                SetTrayIconTabletActiveStatus(false);
                continue;
            }

            EnterCriticalSection(&s_tablet_lock);
            TabletReport report = {0};
            if (s_tablet_info.Parse(s_tablet_packet, packet_size, &report)) {
                SynthesizeInput(&report);
            }
            LeaveCriticalSection(&s_tablet_lock);
        } else if (wait == WAIT_OBJECT_0 + 1) {
            for (MSG msg; PeekMessageW(&msg, 0, 0, 0, PM_REMOVE); ) {
                if (msg.hwnd) {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                    continue;
                }

                if (msg.message == WM_QUIT) {
                    is_running = false;
                } else if (msg.message == TRAY_WM_ACTIVATE_PRESET) {
                    EnterCriticalSection(&s_tablet_lock);
                    s_tablet_preset_idx = msg.lParam;
                    LeaveCriticalSection(&s_tablet_lock);
                    Log(L"Activated \"%ls\" preset", g_presets[msg.lParam].name);
                }
            }
        } else if (wait - WAIT_ABANDONED_0 == 0) {
            Log(L"Wait abandoned? (%d)", GetLastError());
            CleanUpTablet();
        } else {
            Log(L"MsgWaitForMultipleObjects() error %d", GetLastError());
            TRAP();
        }
    }

    CleanUpTablet();
    DeleteCriticalSection(&s_tablet_lock);

    PostThreadMessageW(s_tray_thread_id, WM_QUIT, 0, 0);
    WaitForSingleObject(s_tray_thread, INFINITE);
    DeleteCriticalSection(&s_tray_lock);

    UnhookWinEvent(s_win_event_hook);
    DestroySyntheticPointerDevice(s_ink_device);

    ExitProcess(0);
}

LRESULT MainWindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_QUIT) {
        PostThreadMessageW(s_main_thread_id, WM_QUIT, 0, 0);
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

DWORD WINAPI TrayThreadProc(HANDLE thread_ready) {
    InitThreadMessageQueue();

    WNDCLASSEXW wndclass = {
        .cbSize = sizeof(wndclass),
        .hInstance = s_hinstance,
        .lpszClassName = TRAY_WNDCLASSNAME,
        .lpfnWndProc = TrayWindowEventHandler,
    };
    ASSERT(RegisterClassExW(&wndclass));
    HWND hwnd = CreateWindowExW(0, TRAY_WNDCLASSNAME, 0, 0, 0, 0, 0, 0, 0, 0, s_hinstance, 0);
    ASSERT(hwnd);

    EnterCriticalSection(&s_tray_lock);
    s_tray_icon_data = (NOTIFYICONDATAW){
        .cbSize = sizeof(s_tray_icon_data),
        .hWnd = hwnd,
        .uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP,
        .uCallbackMessage = TRAY_WM_ICON_MESSAGE,
        .hIcon = LoadIconW(s_hinstance, MAKEINTRESOURCEW(IDI_TRAYICON_OFFLINE)),
    };
    swprintf_s(s_tray_icon_data.szTip, sizeof(s_tray_icon_data.szTip), L"tabd");
    LeaveCriticalSection(&s_tray_lock);
    ASSERT(Shell_NotifyIconW(NIM_ADD, &s_tray_icon_data));

    HMENU presets = CreateMenu();
    for (int i = 0; i < COUNTOF(g_presets); i++) {
        AppendMenuW(presets, MF_STRING, TRAY_MENU_PRESET_ITEM_0 + i, g_presets[i].name);
    }
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_POPUP, (UINT_PTR)presets, L"Presets");
    AppendMenuW(menu, MF_STRING, TRAY_MENU_EXIT_ITEM, L"Exit");

    SetEvent(thread_ready);
    thread_ready = 0;

    for (MSG msg; GetMessageW(&msg, 0, 0, 0) > 0; ) {
        if (msg.hwnd) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        } else if (msg.message == TRAY_WM_SHOW_MENU) {
            POINT cursor;
            GetCursorPos(&cursor);
            SetForegroundWindow(hwnd);

            int choice = TrackPopupMenuEx(
                menu, TPM_RETURNCMD | TPM_NONOTIFY, cursor.x, cursor.y, hwnd, 0
            );

            if (choice == TRAY_MENU_EXIT_ITEM) {
                PostThreadMessageW(s_main_thread_id, WM_QUIT, 0, 0);
            } else if (choice >= TRAY_MENU_PRESET_ITEM_0) {
                ASSERT(choice < TRAY_MENU_PRESET_ITEM_0 + COUNTOF(g_presets));
                PostThreadMessageW(
                    s_main_thread_id, TRAY_WM_ACTIVATE_PRESET, 0, choice - TRAY_MENU_PRESET_ITEM_0
                );
            } else {
                Log(L"TrackPopupMenuEx() returned %d (%d)", choice, GetLastError());
            }
        }
    }

    DestroyMenu(menu);
    DestroyMenu(presets);
    Shell_NotifyIconW(NIM_DELETE, &s_tray_icon_data);
    DestroyWindow(hwnd);
    UnregisterClassW(TRAY_WNDCLASSNAME, s_hinstance);
    return 0;
}

LRESULT TrayWindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == TRAY_WM_ICON_MESSAGE && LOWORD(lp) == WM_RBUTTONDOWN) {
        PostThreadMessageW(s_tray_thread_id, TRAY_WM_SHOW_MENU, 0, 0);
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

void SetTrayIconTabletActiveStatus(bool active) {
    EnterCriticalSection(&s_tray_lock);
    s_tray_icon_data.hIcon = LoadIconW(
        s_hinstance, MAKEINTRESOURCEW((active) ? (IDI_TRAYICON_ONLINE) : (IDI_TRAYICON_OFFLINE))
    );
    LeaveCriticalSection(&s_tray_lock);
    Shell_NotifyIconW(NIM_MODIFY, &s_tray_icon_data);
}

bool TryInitTablet(PCWSTR path) {
    EnterCriticalSection(&s_tablet_lock);
    s_tablet_handle = CreateFileW(path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    if (s_tablet_handle == INVALID_HANDLE_VALUE) {
        goto Failure;
    }

    HIDD_ATTRIBUTES attrs = { .Size = sizeof(attrs) };
    bool valid = 
        HidD_GetAttributes(s_tablet_handle, &attrs)
        && FindTabletInfo(attrs.VendorID, attrs.ProductID, &s_tablet_info)
        && HidD_SetFeature(s_tablet_handle, s_tablet_info.features, s_tablet_info.features_size)
        && BeginReadingTablet();
    if (!valid) {
        goto Failure;
    }

    Log(L"Initialized %ls at \"%ls\"", s_tablet_info.name, path);
    LeaveCriticalSection(&s_tablet_lock);
    SetTrayIconTabletActiveStatus(true);
    return true;

    Failure:
    Log(L"Failed to initialize \"%ls\"", path);
    CleanUpTablet();
    LeaveCriticalSection(&s_tablet_lock);
    return false;
}

bool BeginReadingTablet(void) {
    EnterCriticalSection(&s_tablet_lock);
    bool read_ok = ReadFile(
        s_tablet_handle, s_tablet_packet, sizeof(s_tablet_packet), 0, &s_tablet_overlapped
    ) || GetLastError() == ERROR_IO_PENDING;
    LeaveCriticalSection(&s_tablet_lock);
    return read_ok;
}

void CleanUpTablet(void) {
    EnterCriticalSection(&s_tablet_lock);
    CloseHandle(s_tablet_handle);
    s_tablet_handle = INVALID_HANDLE_VALUE;
    s_tablet_info = (TabletInfo){0};
    s_tablet_previous_report = (TabletReport){0};
    LeaveCriticalSection(&s_tablet_lock);
}

DWORD CALLBACK DeviceChangedCallback(
    HCMNOTIFICATION       notification,
    PVOID                 arg,
    CM_NOTIFY_ACTION      action,
    PCM_NOTIFY_EVENT_DATA data,
    DWORD                 data_size
) {
    EnterCriticalSection(&s_tablet_lock);
    if (s_tablet_handle == INVALID_HANDLE_VALUE && action == CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL) {
        TryInitTablet(data->u.DeviceInterface.SymbolicLink);
    }
    LeaveCriticalSection(&s_tablet_lock);
    return ERROR_SUCCESS;
}

void SynthesizeInput(const TabletReport *report) {
    EnterCriticalSection(&s_tablet_lock);
    Vec2 point = MapTabletPointToScreen(
        &g_presets[s_tablet_preset_idx],
        &s_tablet_info,
        report->point
    );

    if (g_presets[s_tablet_preset_idx].mode == MODE_MOUSE) {
        INPUT input = {
            .type = INPUT_MOUSE,
            .mi = (MOUSEINPUT){
                .dx = point.x * 65535,
                .dy = point.y * 65535,
                .dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,
            },
        };

        // FIXME
        bool pointer_down = report->flags & TABLET_REPORT_POINTER_DOWN;
        bool was_pointer_down = s_tablet_previous_report.flags & TABLET_REPORT_POINTER_DOWN;
        bool b1_down = report->flags & TABLET_REPORT_BUTTON_DOWN(0);
        bool was_b1_down = s_tablet_previous_report.flags & TABLET_REPORT_BUTTON_DOWN(0);
        if      (pointer_down && !was_pointer_down) input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
        else if (!pointer_down && was_pointer_down) input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
        if      (b1_down && !was_b1_down) input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
        else if (!b1_down && was_b1_down) input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;

        SendInput(1, &input, sizeof(input));
    } else if (g_presets[s_tablet_preset_idx].mode == MODE_INK) {
        POINT pixel_location = { point.x * s_screen_size.x, point.y * s_screen_size.y };
        POINTER_TYPE_INFO input = {
            .type = PT_PEN,
            .penInfo = {
                .pointerInfo = {
                    .pointerType = PT_PEN,
                    .hwndTarget = s_ink_foreground_window,
                    .pointerFlags = POINTER_FLAG_INRANGE | (
                        (report->pressure)
                            ? (POINTER_FLAG_INCONTACT | POINTER_FLAG_DOWN)
                            : (POINTER_FLAG_UP)
                    ),
                    .ptPixelLocation = pixel_location,
                    .ptPixelLocationRaw = pixel_location,
                },
                .penMask = PEN_MASK_PRESSURE,
                .pressure = report->pressure * 1024,
            }
        };
        InjectSyntheticPointerInput(s_ink_device, &input, 1);
    }

    s_tablet_previous_report = *report;
    LeaveCriticalSection(&s_tablet_lock);
}

void InitThreadMessageQueue(void) {
    MSG m;
    PeekMessageA(&m, 0, WM_USER, WM_USER, PM_NOREMOVE); 
}

void _Log(DWORD tid, PCWSTR file, int line, PCSTR func, PCWSTR message, ...) {
    if (s_hconsole == INVALID_HANDLE_VALUE)
        return;

    WCHAR buffer[1024];
    int length = 0;

    length += swprintf_s(
        buffer + length,
        sizeof(buffer) - length,
        L"%lu %ls:%d:%hs() ",
        tid,
        PathFindFileNameW(file),
        line,
        func
    );

    va_list args;
    va_start(args, message);
    length += vswprintf_s(buffer + length, sizeof(buffer) - length, message, args);
    va_end(args);

    length += swprintf_s(buffer + length, sizeof(buffer) - length, L"\n");

    WriteConsoleW(s_hconsole, buffer, length, 0, 0);
}

void WinEventHookCallback(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
) {
    if (event != EVENT_SYSTEM_FOREGROUND)
        return;

    EnterCriticalSection(&s_tablet_lock);
    s_ink_foreground_window = hwnd;
    LeaveCriticalSection(&s_tablet_lock);
}

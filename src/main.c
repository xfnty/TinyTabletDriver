#include "stdinc.h"

/* macros */
#define TABLET_VID 1386
#define TABLET_PID 891
#define COUNTOF(_a) (sizeof(_a)/sizeof((_a)[0]))
#define ASSERT(_e) do { if (!(_e)) __debugbreak(); } while (0)

/* function declarations */
static DWORD FormatTextVA(char *buffer, size_t max_size, const char *format, va_list args);
static DWORD FormatTextA(char *buffer, size_t max_size, const char *format, ...);
static void Println(const char *message, ...);
static LONG WINAPI GlobalExceptionHandler(EXCEPTION_POINTERS *ex);
static DWORD WINAPI CrashReportThreadProc(LPVOID arg);
static DWORD CALLBACK DeviceChangedCallback(
    HCMNOTIFICATION       notification,
    PVOID                 arg,
    CM_NOTIFY_ACTION      action,
    PCM_NOTIFY_EVENT_DATA data,
    DWORD                 data_size
);

/* variables */
static HANDLE s_stdout;
static CRITICAL_SECTION s_crash_lock;
static HANDLE s_device;
static HANDLE s_device_connected;
static HCMNOTIFICATION s_device_changed_notification;
static OVERLAPPED s_overlapped;

/* function implementations */
void _start(void) {
    AttachConsole(ATTACH_PARENT_PROCESS);
    s_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!IsDebuggerPresent()) {
        InitializeCriticalSection(&s_crash_lock);
        SymSetOptions(SYMOPT_UNDNAME);
        ASSERT(SymInitialize(GetCurrentProcess(), 0, true));
        SetUnhandledExceptionFilter(GlobalExceptionHandler);
    }

    s_device_connected = CreateEventA(0, true, false, 0);

    HDEVINFO hid_device_set = SetupDiGetClassDevsA(
        &GUID_DEVINTERFACE_HID, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT
    );
    for (int i = 0; ; i++) {
        SP_DEVICE_INTERFACE_DATA iface_data = { .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA) };
        if (!SetupDiEnumDeviceInterfaces(hid_device_set, 0, &GUID_DEVINTERFACE_HID, i, &iface_data))
            break;

        char details_buffer[1024] = {0};
        PSP_DEVICE_INTERFACE_DETAIL_DATA_A details = (void*)details_buffer;
        details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
        SetupDiGetDeviceInterfaceDetailA(
            hid_device_set, &iface_data, details, countof(details_buffer) - 1, 0, 0
        );

        s_device = CreateFileA(
            details->DevicePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0
        );
        if (s_device != INVALID_HANDLE_VALUE) {
            HIDD_ATTRIBUTES attrs = { .Size = sizeof(attrs) };
            bool valid = 
                HidD_GetAttributes(s_device, &attrs)
                && attrs.VendorID == TABLET_VID
                && attrs.ProductID == TABLET_PID
                && HidD_SetFeature(s_device, (BYTE[]){ 0x02, 0x02 }, 2);
            if (valid) {
                Println("Connected Wacom CTL-672 tablet.");
                SetEvent(s_device_connected);
                break;
            }
        }

        CloseHandle(s_device);
        s_device = INVALID_HANDLE_VALUE;
    }

    CM_NOTIFY_FILTER filter = {
        .cbSize = sizeof(filter),
        .u.DeviceInterface.ClassGuid = GUID_DEVINTERFACE_HID,
        .FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE,
    };
    DWORD register_result = CM_Register_Notification(
        &filter, 0, DeviceChangedCallback, &s_device_changed_notification
    );
    ASSERT(register_result == ERROR_SUCCESS);

    s_overlapped.hEvent = CreateEventA(0, false, false, 0);

    while (true) {
        DWORD wait = WaitForSingleObject(s_device_connected, INFINITE);
        ASSERT(wait == WAIT_OBJECT_0);

        while (true) {
            BYTE buffer[10] = {0};
            BOOL read_ok = ReadFile(s_device, buffer, sizeof(buffer), 0, &s_overlapped);
            DWORD read_error = GetLastError();
            if (!read_ok && read_error != ERROR_IO_PENDING) {
                s_device = INVALID_HANDLE_VALUE;
                Println("Tablet lost.");
                ResetEvent(s_device_connected);
                break;
            }

            DWORD bytes_read = 0;
            GetOverlappedResult(s_device, &s_overlapped, &bytes_read, true);

            if (buffer[0] != 0x02 || (buffer[1] == 0x00 || buffer[1] == 0x80))
                continue;

            Println(
                "%02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX %02hhX ",
                buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], 
                buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]
            );
        }
    }

    ExitProcess(0);
}

DWORD FormatTextVA(char *buffer, size_t max_size, const char *format, va_list args) {
    int r = __stdio_common_vsnprintf_s(0, buffer, max_size, max_size - 1, format, 0, args);
    ASSERT(r >= 0);
    return (DWORD)r;
}

DWORD FormatTextA(char *buffer, size_t max_size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    DWORD r = FormatTextVA(buffer, max_size, format, args);
    va_end(args);
    return r;
}

void Println(const char *message, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, message);
    int length = FormatTextVA(buffer, sizeof(buffer), message, args);
    va_end(args);

    buffer[length++] = '\n';

    if (IsDebuggerPresent()) {
        buffer[length] = '\0';
        OutputDebugStringA(buffer);
    } else {
        WriteConsoleA(s_stdout, buffer, length, 0, 0);
    }
}

LONG WINAPI GlobalExceptionHandler(EXCEPTION_POINTERS *ex) {
    EnterCriticalSection(&s_crash_lock);

    DWORD64 excode = ex->ExceptionRecord->ExceptionCode;
    DWORD64 exaddr = ex->ExceptionRecord->ExceptionAddress;
    DWORD tid = GetCurrentThreadId();
    DWORD pid = GetCurrentProcessId();
    HANDLE process = GetCurrentProcess();

    HANDLE threads = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te;
    te.dwSize = sizeof(te);
    if (Thread32First(threads, &te)) {
        do {
            if (te.th32OwnerProcessID == pid && te.th32ThreadID != tid) {
                HANDLE t = OpenThread(THREAD_SUSPEND_RESUME, false, te.th32ThreadID);
                if (!t) continue;

                SuspendThread(t);
                CloseHandle(t);
            }

            te.dwSize = sizeof(te);
        } while (Thread32Next(threads, &te));
    }
    CloseHandle(threads);

    char msg[256] = { '\0' };
    size_t msgl = 0;

    static const struct {
        const char *name;
        DWORD code;
    } names[] = {
        { "Access violation", EXCEPTION_ACCESS_VIOLATION },
        { "Breakpoint", EXCEPTION_BREAKPOINT },
        { "Divided by zero", EXCEPTION_FLT_DIVIDE_BY_ZERO },
        { "Divided by zero", EXCEPTION_INT_DIVIDE_BY_ZERO },
        { "Noncontinuable exception", EXCEPTION_NONCONTINUABLE_EXCEPTION },
        { "Stack overflow", EXCEPTION_STACK_OVERFLOW },
    };
    bool found_name = false;
    for (size_t i = 0; i < COUNTOF(names); i++) {
        if (excode == names[i].code) {
            msgl += FormatTextA(msg + msgl, sizeof(msg) - msgl, "%s", names[i].name);
            found_name = true;
            break;
        }
    }
    if (!found_name) {
        msgl += FormatTextA( msg + msgl, sizeof(msg) - msgl, "Exception %08X", excode);
    }

    uint64_t displacement;
    uint8_t symbuf[sizeof(SYMBOL_INFO) + 256];
    SYMBOL_INFO *sym = (SYMBOL_INFO*)symbuf;
    *sym = (SYMBOL_INFO){ .MaxNameLen = 255, .SizeOfStruct = sizeof(*sym) };

    DWORD ld;
    IMAGEHLP_LINE line = { .SizeOfStruct = sizeof(line) };

    BOOL sym_ok = SymFromAddr(process, exaddr, &displacement, sym);
    BOOL line_ok = SymGetLineFromAddr64(process, exaddr, &ld, &line);

    if (sym_ok && line_ok) {
        msgl += FormatTextA(
            msg + msgl,
            sizeof(msg) - msgl,
            " in %s:%d %s()",
            PathFindFileNameA(line.FileName),
            line.LineNumber,
            sym->Name
        );
    } else if (sym_ok) {
        msgl += FormatTextA(msg + msgl, sizeof(msg) - msgl, " in %s()", sym->Name);
    } else {
        msgl += FormatTextA(msg + msgl, sizeof(msg) - msgl, " at 0x%08llX", exaddr);
    }

    HANDLE crash_report_thread = CreateThread(NULL, 0, CrashReportThreadProc, msg, 0, NULL);
    WaitForSingleObject(crash_report_thread, INFINITE);
    ExitProcess((DWORD)excode);
    return EXCEPTION_CONTINUE_SEARCH;
}

DWORD WINAPI CrashReportThreadProc(LPVOID arg) {
    MSG m;
    PeekMessageA(&m, NULL, WM_USER, WM_USER, PM_NOREMOVE); 
    MessageBoxA(0, (const char*)arg, "Tiny Tablet Driver has crashed.", MB_ICONERROR | MB_OK);
    return 0;
}

DWORD CALLBACK DeviceChangedCallback(
    HCMNOTIFICATION       notification,
    PVOID                 arg,
    CM_NOTIFY_ACTION      action,
    PCM_NOTIFY_EVENT_DATA data,
    DWORD                 data_size
) {
    (void)notification; (void)arg; (void)data_size;

    if (action != CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL || s_device != INVALID_HANDLE_VALUE)
        return ERROR_SUCCESS;

    s_device = CreateFileW(
        data->u.DeviceInterface.SymbolicLink,
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0
    );
    if (s_device == INVALID_HANDLE_VALUE)
        return ERROR_SUCCESS;

    HIDD_ATTRIBUTES attrs = { .Size = sizeof(attrs) };
    bool valid = 
        HidD_GetAttributes(s_device, &attrs)
        && attrs.VendorID == TABLET_VID
        && attrs.ProductID == TABLET_PID
        && HidD_SetFeature(s_device, (BYTE[]){ 0x02, 0x02 }, 2);
    if (!valid) {
        CloseHandle(s_device);
        s_device = INVALID_HANDLE_VALUE;
        return ERROR_SUCCESS;
    }

    Println("Connected Wacom CTL-672 tablet.");
    SetEvent(s_device_connected);
    return ERROR_SUCCESS;
}

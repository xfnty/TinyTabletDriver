#ifndef _TABD_UTIL_H
#define _TABD_UTIL_H

/* misc */
typedef unsigned short wchar_t;
typedef struct {
    float x, y;
} Vec2;

#define _CHAR_LITERAL_TO_WCHAR_2(_x) L ## _x
#define CHAR_LITERAL_TO_WCHAR(_x) _CHAR_LITERAL_TO_WCHAR_2(_x)
#define __WFILE__ CHAR_LITERAL_TO_WCHAR(__FILE__)
#define COUNTOF(_a) (sizeof(_a)/sizeof((_a)[0]))
#define TRAP() __debugbreak()
#define ASSERT(_e) do { if (!(_e)) TRAP(); } while(0)
#define CONSTRAIN(_v, _min, _max) ((_v) < (_min) ? (_min) : ((_v) > (_max) ? (_max) : (_v)))


/* stdlib */
#pragma comment(lib, "ucrt.lib")
#pragma comment(lib, "vcruntime.lib")

typedef unsigned __int64 size_t;
typedef unsigned char bool;
typedef char* va_list;
typedef void *_locale_t;

#define true 1
#define false 0
#define va_start(_list, _arg) ((void)__va_start(&(_list), (_arg)))
#define va_end(_list) ((void)((_list) = (va_list)0))

double __cdecl sin(double _X);
double __cdecl cos(double _X);
void __cdecl __va_start(va_list* , ...);
void *memset(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
int __cdecl __stdio_common_vsnwprintf_s(
    size_t         _Options,
    wchar_t*       _Buffer,
    size_t         _BufferCount,
    size_t         _MaxCount,
    wchar_t const* _Format,
    _locale_t      _Locale,
    va_list        _ArgList
);

int swprintf_s(wchar_t *buffer, size_t size, const wchar_t *format, ...) {
    va_list args;
    va_start(args, format);
    return __stdio_common_vsnwprintf_s(0, buffer, size, size-1, format, 0, args);
}

int vswprintf_s(wchar_t *buffer, size_t size, const wchar_t *format, va_list args) {
    return __stdio_common_vsnwprintf_s(0, buffer, size, size-1, format, 0, args);
}


/* windows.h */
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")

#define WINAPI __stdcall
#define CALLBACK __stdcall

typedef unsigned char UCHAR, BYTE, BOOLEAN;
typedef int BOOL, INT32;
typedef long LONG;
typedef unsigned long ULONG, *PULONG;
typedef __int64 LONG_PTR, LPARAM, LRESULT;
typedef unsigned int DWORD, *PDWORD, *LPDWORD, UINT, UINT32;
typedef unsigned short USHORT, WCHAR, *PWSTR, WORD, ATOM;
typedef void VOID, *PVOID, *LPVOID;
typedef PVOID HANDLE, HWND, HMENU, HINSTANCE, HICON, HCURSOR, HBRUSH, HMODULE, 
HSYNTHETICPOINTERDEVICE, HWINEVENTHOOK;
typedef const PWSTR PCWSTR, LPCWSTR;
typedef const char *PCSTR, *LPSTR, *LPCSTR;
typedef unsigned __int64 ULONG_PTR, UINT_PTR, SIZE_T, DWORD_PTR, WPARAM, UINT64;
typedef struct _OVERLAPPED {
  ULONG_PTR Internal;
  ULONG_PTR InternalHigh;
  union {
    struct {
      DWORD Offset;
      DWORD OffsetHigh;
    } DUMMYSTRUCTNAME;
    PVOID Pointer;
  };
  HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  LPVOID lpSecurityDescriptor;
  BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef struct _LIST_ENTRY _LIST_ENTRY;
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;
typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD Type;
    WORD CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD  CreatorBackTraceIndexHigh;
    WORD  SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;
#pragma pack(push, 8)
typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION, *LPCRITICAL_SECTION;
#pragma pack(pop)
typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
typedef struct tagMOUSEINPUT {
  LONG      dx;
  LONG      dy;
  DWORD     mouseData;
  DWORD     dwFlags;
  DWORD     time;
  ULONG_PTR dwExtraInfo;
} MOUSEINPUT, *PMOUSEINPUT, *LPMOUSEINPUT;
typedef struct tagKEYBDINPUT {
  WORD      wVk;
  WORD      wScan;
  DWORD     dwFlags;
  DWORD     time;
  ULONG_PTR dwExtraInfo;
} KEYBDINPUT, *PKEYBDINPUT, *LPKEYBDINPUT;
typedef struct tagHARDWAREINPUT {
  DWORD uMsg;
  WORD  wParamL;
  WORD  wParamH;
} HARDWAREINPUT, *PHARDWAREINPUT, *LPHARDWAREINPUT;
typedef struct tagINPUT {
  DWORD type;
  union {
    MOUSEINPUT    mi;
    KEYBDINPUT    ki;
    HARDWAREINPUT hi;
  };
} INPUT, *PINPUT, *LPINPUT;
typedef struct tagPOINT {
  LONG x;
  LONG y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;
typedef struct tagMSG {
  HWND   hwnd;
  UINT   message;
  WPARAM wParam;
  LPARAM lParam;
  DWORD  time;
  POINT  pt;
  DWORD  lPrivate;
} MSG, *PMSG, *NPMSG, *LPMSG;
typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;
typedef struct _GUID {
    ULONG   Data1;
    USHORT  Data2;
    USHORT  Data3;
    UCHAR   Data4[8];
} GUID;
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef struct tagWNDCLASSEXW {
    UINT        cbSize;
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCWSTR     lpszMenuName;
    LPCWSTR     lpszClassName;
    HICON       hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;
typedef struct _HIDD_ATTRIBUTES {
  ULONG  Size;
  USHORT VendorID;
  USHORT ProductID;
  USHORT VersionNumber;
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;
enum {
    PT_POINTER = 1,
    PT_TOUCH   = 2,
    PT_PEN     = 3,
    PT_MOUSE   = 4,
};
typedef DWORD POINTER_INPUT_TYPE;
typedef UINT32 POINTER_FLAGS;
typedef enum {
    POINTER_FEEDBACK_DEFAULT = 1,   // The injected pointer input feedback may get suppressed by the end-user settings in the Pen and Touch control panel.
    POINTER_FEEDBACK_INDIRECT = 2,  // The injected pointer input feedback overrides the end-user settings in the Pen and Touch control panel.
    POINTER_FEEDBACK_NONE = 3,      // No touch visualizations.
} POINTER_FEEDBACK_MODE;
typedef enum tagPOINTER_BUTTON_CHANGE_TYPE {
    POINTER_CHANGE_NONE,
    POINTER_CHANGE_FIRSTBUTTON_DOWN,
    POINTER_CHANGE_FIRSTBUTTON_UP,
    POINTER_CHANGE_SECONDBUTTON_DOWN,
    POINTER_CHANGE_SECONDBUTTON_UP,
    POINTER_CHANGE_THIRDBUTTON_DOWN,
    POINTER_CHANGE_THIRDBUTTON_UP,
    POINTER_CHANGE_FOURTHBUTTON_DOWN,
    POINTER_CHANGE_FOURTHBUTTON_UP,
    POINTER_CHANGE_FIFTHBUTTON_DOWN,
    POINTER_CHANGE_FIFTHBUTTON_UP,
} POINTER_BUTTON_CHANGE_TYPE;
typedef struct tagPOINTER_INFO {
    POINTER_INPUT_TYPE    pointerType;
    UINT32          pointerId;
    UINT32          frameId;
    POINTER_FLAGS   pointerFlags;
    HANDLE          sourceDevice;
    HWND            hwndTarget;
    POINT           ptPixelLocation;
    POINT           ptHimetricLocation;
    POINT           ptPixelLocationRaw;
    POINT           ptHimetricLocationRaw;
    DWORD           dwTime;
    UINT32          historyCount;
    INT32           InputData;
    DWORD           dwKeyStates;
    UINT64          PerformanceCount;
    POINTER_BUTTON_CHANGE_TYPE ButtonChangeType;
} POINTER_INFO;
typedef UINT32 TOUCH_FLAGS;
typedef UINT32 TOUCH_MASK;
typedef struct tagPOINTER_TOUCH_INFO {
    POINTER_INFO    pointerInfo;
    TOUCH_FLAGS     touchFlags;
    TOUCH_MASK      touchMask;
    RECT            rcContact;
    RECT            rcContactRaw;
    UINT32          orientation;
    UINT32          pressure;
} POINTER_TOUCH_INFO;
typedef UINT32 PEN_FLAGS;
typedef UINT32 PEN_MASK;
typedef struct tagPOINTER_PEN_INFO {
    POINTER_INFO    pointerInfo;
    PEN_FLAGS       penFlags;
    PEN_MASK        penMask;
    UINT32          pressure;
    UINT32          rotation;
    INT32           tiltX;
    INT32           tiltY;
} POINTER_PEN_INFO;
typedef struct tagPOINTER_TYPE_INFO {
    POINTER_INPUT_TYPE  type;
    union{
        POINTER_TOUCH_INFO touchInfo;
        POINTER_PEN_INFO   penInfo;
    };
}POINTER_TYPE_INFO, *PPOINTER_TYPE_INFO;
typedef VOID (CALLBACK* WINEVENTPROC)(
    HWINEVENTHOOK hWinEventHook,
    DWORD         event,
    HWND          hwnd,
    LONG          idObject,
    LONG          idChild,
    DWORD         idEventThread,
    DWORD         dwmsEventTime
);

#define ERROR_SUCCESS                      0L
#define ATTACH_PARENT_PROCESS              ((DWORD)-1)
#define STD_OUTPUT_HANDLE                  ((DWORD)-11)
#define INVALID_HANDLE_VALUE               ((HANDLE)(LONG_PTR)-1)
#define GENERIC_READ                       0x80000000L
#define GENERIC_WRITE                      0x40000000L
#define FILE_SHARE_READ                    0x00000001
#define FILE_SHARE_WRITE                   0x00000002
#define OPEN_EXISTING                      3
#define FILE_FLAG_OVERLAPPED               0x40000000
#define WM_QUIT                            0x0012
#define WM_RBUTTONDOWN                     0x0204
#define WM_USER                            0x0400
#define PM_NOREMOVE                        0x0000
#define PM_REMOVE                          0x0001
#define INFINITE                           0xFFFFFFFF
#define MF_STRING                          0x00000000L
#define MF_POPUP                           0x00000010L
#define TPM_NONOTIFY                       0x0080L
#define TPM_RETURNCMD                      0x0100L
#define WAIT_OBJECT_0                      0x00000000L
#define WAIT_ABANDONED_0                   0x00000080L
#define QS_ALLINPUT                        0x047B
#define ERROR_IO_PENDING                   997
#define INPUT_MOUSE                        0
#define INPUT_KEYBOARD                     1
#define INPUT_HARDWARE                     2
#define MOUSEEVENTF_MOVE                   0x0001
#define MOUSEEVENTF_LEFTDOWN               0x0002
#define MOUSEEVENTF_LEFTUP                 0x0004
#define MOUSEEVENTF_RIGHTDOWN              0x0008
#define MOUSEEVENTF_RIGHTUP                0x0010
#define MOUSEEVENTF_ABSOLUTE               0x8000
#define SM_CXSCREEN                        0
#define SM_CYSCREEN                        1
#define PEN_FLAG_NONE                      0x00000000
#define PEN_FLAG_BARREL                    0x00000001
#define PEN_FLAG_INVERTED                  0x00000002
#define PEN_FLAG_ERASER                    0x00000004
#define PEN_MASK_NONE                      0x00000000
#define PEN_MASK_PRESSURE                  0x00000001
#define PEN_MASK_ROTATION                  0x00000002
#define PEN_MASK_TILT_X                    0x00000004
#define PEN_MASK_TILT_Y                    0x00000008
#define POINTER_FLAG_INRANGE               0x00000002
#define POINTER_FLAG_INCONTACT             0x00000004
#define POINTER_FLAG_FIRSTBUTTON           0x00000010
#define POINTER_FLAG_SECONDBUTTON          0x00000020
#define POINTER_FLAG_THIRDBUTTON           0x00000040
#define POINTER_FLAG_FOURTHBUTTON          0x00000080
#define POINTER_FLAG_FIFTHBUTTON           0x00000100
#define POINTER_FLAG_PRIMARY               0x00002000
#define POINTER_FLAG_DOWN                  0x00010000
#define POINTER_FLAG_UP                    0x00040000
#define EVENT_SYSTEM_FOREGROUND            0x0003
#define WINEVENT_OUTOFCONTEXT              0x0000
#define WINEVENT_SKIPOWNPROCESS            0x0002
#define LOWORD(l)                          ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)                          ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define MAKEINTRESOURCEW(i)                ((LPCWSTR)((ULONG_PTR)((WORD)(i))))
#define GUID_DEVINTERFACE_HID \
    (GUID){ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }

DWORD GetLastError(void);
void SetLastError(DWORD dwErrCode);
BOOL WINAPI AttachConsole(DWORD dwProcessId);
HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
void ExitProcess(DWORD code);
BOOL WINAPI WriteConsoleW(
    HANDLE  hConsoleOutput,
    PCWSTR  buffer,
    DWORD   length,
    LPDWORD written,
    LPVOID  reserved
);
HANDLE CreateFileW(
    LPCWSTR               lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
);
BOOL ReadFile(HANDLE file, LPVOID buf, DWORD size, LPDWORD read, LPOVERLAPPED ol);
HANDLE CreateEventW(
    PVOID   lpEventAttributes,
    BOOL    bManualReset,
    BOOL    bInitialState,
    LPCWSTR lpName
);
BOOL GetOverlappedResult(
    HANDLE       hFile,
    LPOVERLAPPED lpOverlapped,
    LPDWORD      lpNumberOfBytesTransferred,
    BOOL         bWait
);
DWORD WaitForSingleObject(
    HANDLE hHandle,
    DWORD  dwMilliseconds
);
DWORD WaitForMultipleObjects(
    DWORD         nCount,
    const HANDLE *lpHandles,
    BOOL          bWaitAll,
    DWORD         dwMilliseconds
);
BOOL CloseHandle(HANDLE hObject);
VOID InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
DWORD GetCurrentThreadId(void);
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES   lpThreadAttributes,
    SIZE_T                  dwStackSize,
    LPTHREAD_START_ROUTINE  lpStartAddress,
    LPVOID                  lpParameter,
    DWORD                   dwCreationFlags,
    LPDWORD                 lpThreadId
);
HMODULE WINAPI GetModuleHandleW(LPCWSTR lpModuleName);
UINT SendInput(UINT cInputs, LPINPUT pInputs, int cbSize);
ATOM WINAPI RegisterClassExW(const WNDCLASSEXW *cls);
BOOL WINAPI UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance);
HWND WINAPI CreateWindowExW(
    DWORD dwExStyle,
    LPCWSTR lpClassName,
    LPCWSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
);
BOOL WINAPI DestroyWindow(HWND hWnd);
BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI TranslateMessage(const MSG *lpMsg);
LRESULT WINAPI DispatchMessageW(const MSG *lpMsg);
DWORD MsgWaitForMultipleObjects(
    DWORD         nCount,
    const HANDLE *pHandles,
    BOOL          fWaitAll,
    DWORD         dwMilliseconds,
    DWORD         dwWakeMask
);
int GetSystemMetrics(int nIndex);
BOOL PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);
HICON WINAPI LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName);
HMENU WINAPI CreateMenu(void);
HMENU CreatePopupMenu(void);
BOOL WINAPI DestroyMenu(HMENU hMenu);
BOOL AppendMenuW(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCWSTR lpNewItem);
BOOL TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int _, HWND hWnd, const RECT *prcRect);
BOOL GetCursorPos(LPPOINT lpPoint);
BOOL SetForegroundWindow(HWND hWnd);
BOOLEAN HidD_GetAttributes(HANDLE HidDeviceObject, PHIDD_ATTRIBUTES Attributes);
BOOLEAN HidD_SetFeature(HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength);
HWND GetForegroundWindow(void);
HSYNTHETICPOINTERDEVICE WINAPI CreateSyntheticPointerDevice(
    POINTER_INPUT_TYPE pointerType, ULONG maxCount, POINTER_FEEDBACK_MODE mode
);
BOOL WINAPI InjectSyntheticPointerInput(
    HSYNTHETICPOINTERDEVICE device, const POINTER_TYPE_INFO* pointerInfo, UINT32 count
);
void WINAPI DestroySyntheticPointerDevice(HSYNTHETICPOINTERDEVICE device);
HWINEVENTHOOK SetWinEventHook(
    DWORD        eventMin,
    DWORD        eventMax,
    HMODULE      hmodWinEventProc,
    WINEVENTPROC pfnWinEventProc,
    DWORD        idProcess,
    DWORD        idThread,
    DWORD        dwFlags
);
BOOL UnhookWinEvent(HWINEVENTHOOK hWinEventHook);
LRESULT CALLBACK DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


/* shlwapi.h */
#pragma comment(lib, "shlwapi.lib")

#define NIF_MESSAGE                        0x00000001
#define NIF_ICON                           0x00000002
#define NIF_TIP                            0x00000004
#define NIM_ADD                            0x00000000
#define NIM_MODIFY                         0x00000001
#define NIM_DELETE                         0x00000002

typedef struct _NOTIFYICONDATAW {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    WCHAR  szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    WCHAR  szInfo[256];
    union {
        UINT  uTimeout;
        UINT  uVersion;
    };
    WCHAR  szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID guidItem;
    HICON hBalloonIcon;
} NOTIFYICONDATAW, *PNOTIFYICONDATAW;

LPCWSTR PathFindFileNameW(LPCWSTR pszPath);
BOOL Shell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATAW lpData);


/* cfgmgr32.h */
#pragma comment(lib, "cfgmgr32.lib")

typedef enum _CM_NOTIFY_FILTER_TYPE {
    CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE = 0,
    CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE,
    CM_NOTIFY_FILTER_TYPE_DEVICEINSTANCE,
    CM_NOTIFY_FILTER_TYPE_MAX
} CM_NOTIFY_FILTER_TYPE, *PCM_NOTIFY_FILTER_TYPE;
typedef struct _CM_NOTIFY_FILTER {
    DWORD                 cbSize;
    DWORD                 Flags;
    CM_NOTIFY_FILTER_TYPE FilterType;
    DWORD                 Reserved;
    union {
        struct {
            GUID ClassGuid;
        } DeviceInterface;
        struct {
            HANDLE hTarget;
        } DeviceHandle;
        struct {
            WCHAR InstanceId[200];
        } DeviceInstance;
    } u;
} CM_NOTIFY_FILTER, *PCM_NOTIFY_FILTER;
typedef HANDLE HCMNOTIFICATION, *PHCMNOTIFICATION;
typedef enum _CM_NOTIFY_ACTION {
    CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL = 0,
    CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL,
    CM_NOTIFY_ACTION_DEVICEQUERYREMOVE,
    CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED,
    CM_NOTIFY_ACTION_DEVICEREMOVEPENDING,
    CM_NOTIFY_ACTION_DEVICEREMOVECOMPLETE,
    CM_NOTIFY_ACTION_DEVICECUSTOMEVENT,
    CM_NOTIFY_ACTION_DEVICEINSTANCEENUMERATED,
    CM_NOTIFY_ACTION_DEVICEINSTANCESTARTED,
    CM_NOTIFY_ACTION_DEVICEINSTANCEREMOVED,
    CM_NOTIFY_ACTION_MAX
} CM_NOTIFY_ACTION, *PCM_NOTIFY_ACTION;
typedef struct _CM_NOTIFY_EVENT_DATA {
    CM_NOTIFY_FILTER_TYPE FilterType;
    DWORD                 Reserved;
    union {
        struct {
            GUID  ClassGuid;
            WCHAR SymbolicLink[1];
        } DeviceInterface;
        struct {
            GUID  EventGuid;
            LONG  NameOffset;
            DWORD DataSize;
            BYTE  Data[1];
        } DeviceHandle;
        struct {
            WCHAR InstanceId[1];
        } DeviceInstance;
    } u;
} CM_NOTIFY_EVENT_DATA, *PCM_NOTIFY_EVENT_DATA;
typedef DWORD (CALLBACK *PCM_NOTIFY_CALLBACK)(
    HCMNOTIFICATION       hNotify,
    PVOID                 Context,
    CM_NOTIFY_ACTION      Action,
    PCM_NOTIFY_EVENT_DATA EventData,
    DWORD                 EventDataSize
);

DWORD CM_Register_Notification(
    PCM_NOTIFY_FILTER   pFilter,
    PVOID               pContext,
    PCM_NOTIFY_CALLBACK pCallback,
    PHCMNOTIFICATION    pNotifyContext
);
DWORD CM_Unregister_Notification(HCMNOTIFICATION NotifyContext);


/* setupapi.h */
#pragma comment(lib, "setupapi.lib")

#define DIGCF_PRESENT         0x00000002
#define DIGCF_DEVICEINTERFACE 0x00000010

typedef HANDLE HDEVINFO;
typedef struct _SP_DEVINFO_DATA {
    DWORD cbSize;
    GUID  ClassGuid;
    DWORD DevInst;
    ULONG_PTR Reserved;
} SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;
typedef struct _SP_DEVICE_INTERFACE_DATA {
    DWORD cbSize;
    GUID  InterfaceClassGuid;
    DWORD Flags;
    ULONG_PTR Reserved;
} SP_DEVICE_INTERFACE_DATA, *PSP_DEVICE_INTERFACE_DATA;
typedef struct _SP_DEVICE_INTERFACE_DETAIL_DATA_W {
    DWORD cbSize;
    WCHAR DevicePath[1];
} SP_DEVICE_INTERFACE_DETAIL_DATA_W, *PSP_DEVICE_INTERFACE_DETAIL_DATA_W;

HDEVINFO SetupDiGetClassDevsW(
    const GUID *ClassGuid, PCWSTR Enumerator, HWND hwndParent, DWORD Flags
);
BOOL SetupDiEnumDeviceInterfaces(
    HDEVINFO                  DeviceInfoSet,
    PSP_DEVINFO_DATA          DeviceInfoData,
    const GUID                *InterfaceClassGuid,
    DWORD                     MemberIndex,
    PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData
);
BOOL SetupDiGetDeviceInterfaceDetailW(
    HDEVINFO                           DeviceInfoSet,
    PSP_DEVICE_INTERFACE_DATA          DeviceInterfaceData,
    PSP_DEVICE_INTERFACE_DETAIL_DATA_W DeviceInterfaceDetailData,
    DWORD                              DeviceInterfaceDetailDataSize,
    PDWORD                             RequiredSize,
    PSP_DEVINFO_DATA                   DeviceInfoData
);

#endif /* _TABD_UTIL_H */

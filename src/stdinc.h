#pragma once

#if !defined(_WIN64)
    #error Only Win64 is supported.
#endif

#if !defined(_MSC_VER)
    #error Only MSVC is supported.
#endif

/* stdbool.h */
typedef unsigned char bool;
#define true 1
#define false 0

/* stdint.h */
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t, uintptr_t;

/* windows.h */
#define WINAPI __stdcall
#define CALLBACK __stdcall

typedef int BOOL;
typedef void *HWND, *HMENU, *HINSTANCE, *HICON, *HCURSOR, *HBRUSH, *HMODULE;
typedef void VOID, *PVOID, *LPVOID;
typedef void *HANDLE;
typedef HANDLE HDEVINFO;
typedef char CHAR, *PCHAR;
typedef long LONG;
typedef LONG NTSTATUS;
typedef __int64 LONG_PTR;
typedef unsigned long ULONG, *PULONG;
typedef unsigned __int64 ULONG_PTR, UINT_PTR;
typedef unsigned int DWORD, *PDWORD, *LPDWORD;
typedef unsigned char UCHAR, *PUCHAR, BYTE, *PBYTE, BOOLEAN;
typedef unsigned long ULONG;
typedef unsigned long long DWORD64, *PDWORD64, ULONGLONG, ULONG64;
typedef long long LONGLONG;
typedef unsigned short USHORT;
typedef unsigned short WCHAR;
typedef unsigned short WORD, ATOM;
typedef unsigned int UINT;
typedef const char *PCSTR, *LPSTR, *LPCSTR;
typedef const WCHAR *PCWSTR, *LPCWSTR;
typedef char *PSTR;
typedef char* va_list;
typedef unsigned __int64 size_t;
typedef PVOID _locale_t;
typedef USHORT USAGE, *PUSAGE;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;
typedef ULONG_PTR SIZE_T, DWORD_PTR;
typedef PVOID WINUSB_INTERFACE_HANDLE, *PWINUSB_INTERFACE_HANDLE;
typedef struct _GUID {
    ULONG   Data1;
    USHORT  Data2;
    USHORT  Data3;
    UCHAR   Data4[8];
} GUID;
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
typedef struct _SP_DEVICE_INTERFACE_DETAIL_DATA_A {
    DWORD cbSize;
    CHAR  DevicePath[1];
} SP_DEVICE_INTERFACE_DETAIL_DATA_A, *PSP_DEVICE_INTERFACE_DETAIL_DATA_A;
typedef struct _OVERLAPPED {
  ULONG_PTR Internal;
  ULONG_PTR InternalHigh;
  union {
    struct {
      DWORD Offset;
      DWORD OffsetHigh;
    } DUMMYSTRUCTNAME;
    PVOID Pointer;
  } DUMMYUNIONNAME;
  HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;
typedef struct _HIDD_ATTRIBUTES {
  ULONG  Size;
  USHORT VendorID;
  USHORT ProductID;
  USHORT VersionNumber;
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;
typedef struct _HIDP_PREPARSED_DATA *PHIDP_PREPARSED_DATA;
typedef struct _HIDP_CAPS {
  USAGE  Usage;
  USAGE  UsagePage;
  USHORT InputReportByteLength;
  USHORT OutputReportByteLength;
  USHORT FeatureReportByteLength;
  USHORT Reserved[17];
  USHORT NumberLinkCollectionNodes;
  USHORT NumberInputButtonCaps;
  USHORT NumberInputValueCaps;
  USHORT NumberInputDataIndices;
  USHORT NumberOutputButtonCaps;
  USHORT NumberOutputValueCaps;
  USHORT NumberOutputDataIndices;
  USHORT NumberFeatureButtonCaps;
  USHORT NumberFeatureValueCaps;
  USHORT NumberFeatureDataIndices;
} HIDP_CAPS, *PHIDP_CAPS;
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
typedef struct _LIST_ENTRY _LIST_ENTRY;
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;
typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD   CreatorBackTraceIndexHigh;
    WORD   SpareWORD  ;
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
#pragma pack(pop)
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION, *LPCRITICAL_SECTION;
typedef struct _EXCEPTION_RECORD64 {
    DWORD    ExceptionCode;
    DWORD ExceptionFlags;
    DWORD64 ExceptionRecord;
    DWORD64 ExceptionAddress;
    DWORD NumberParameters;
    DWORD __unusedAlignment;
    DWORD64 ExceptionInformation[15];
} EXCEPTION_RECORD64, *PEXCEPTION_RECORD64;
typedef EXCEPTION_RECORD64 EXCEPTION_RECORD, *PEXCEPTION_RECORD;
typedef struct __declspec(align(16)) _M128A {
    ULONGLONG Low;
    LONGLONG High;
} M128A, *PM128A;
typedef struct __declspec(align(16)) _XSAVE_FORMAT {
    WORD  ControlWord;
    WORD  StatusWord;
    BYTE  TagWord;
    BYTE  Reserved1;
    WORD  ErrorOpcode;
    DWORD ErrorOffset;
    WORD  ErrorSelector;
    WORD  Reserved2;
    DWORD DataOffset;
    WORD  DataSelector;
    WORD  Reserved3;
    DWORD MxCsr;
    DWORD MxCsr_Mask;
    M128A FloatRegisters[8];
    M128A XmmRegisters[16];
    BYTE  Reserved4[96];
} XSAVE_FORMAT, *PXSAVE_FORMAT;
typedef XSAVE_FORMAT XMM_SAVE_AREA32, *PXMM_SAVE_AREA32;
typedef struct _NEON128 {
    ULONGLONG Low;
    LONGLONG High;
} NEON128, *PNEON128;
typedef struct _CONTEXT {
  DWORD64 P1Home;
  DWORD64 P2Home;
  DWORD64 P3Home;
  DWORD64 P4Home;
  DWORD64 P5Home;
  DWORD64 P6Home;
  DWORD   ContextFlags;
  DWORD   MxCsr;
  WORD    SegCs;
  WORD    SegDs;
  WORD    SegEs;
  WORD    SegFs;
  WORD    SegGs;
  WORD    SegSs;
  DWORD   EFlags;
  DWORD64 Dr0;
  DWORD64 Dr1;
  DWORD64 Dr2;
  DWORD64 Dr3;
  DWORD64 Dr6;
  DWORD64 Dr7;
  DWORD64 Rax;
  DWORD64 Rcx;
  DWORD64 Rdx;
  DWORD64 Rbx;
  DWORD64 Rsp;
  DWORD64 Rbp;
  DWORD64 Rsi;
  DWORD64 Rdi;
  DWORD64 R8;
  DWORD64 R9;
  DWORD64 R10;
  DWORD64 R11;
  DWORD64 R12;
  DWORD64 R13;
  DWORD64 R14;
  DWORD64 R15;
  DWORD64 Rip;
  union {
    XMM_SAVE_AREA32 FltSave;
    NEON128         Q[16];
    ULONGLONG       D[32];
    struct {
      M128A Header[2];
      M128A Legacy[8];
      M128A Xmm0;
      M128A Xmm1;
      M128A Xmm2;
      M128A Xmm3;
      M128A Xmm4;
      M128A Xmm5;
      M128A Xmm6;
      M128A Xmm7;
      M128A Xmm8;
      M128A Xmm9;
      M128A Xmm10;
      M128A Xmm11;
      M128A Xmm12;
      M128A Xmm13;
      M128A Xmm14;
      M128A Xmm15;
    } DUMMYSTRUCTNAME;
    DWORD           S[32];
  } DUMMYUNIONNAME;
  M128A   VectorRegister[26];
  DWORD64 VectorControl;
  DWORD64 DebugControl;
  DWORD64 LastBranchToRip;
  DWORD64 LastBranchFromRip;
  DWORD64 LastExceptionToRip;
  DWORD64 LastExceptionFromRip;
} CONTEXT, *PCONTEXT;
typedef struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT          ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
typedef LONG (WINAPI *PTOP_LEVEL_EXCEPTION_FILTER)(PEXCEPTION_POINTERS ExceptionInfo);
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;
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
typedef struct tagTHREADENTRY32 {
  DWORD dwSize;
  DWORD cntUsage;
  DWORD th32ThreadID;
  DWORD th32OwnerProcessID;
  LONG  tpBasePri;
  LONG  tpDeltaPri;
  DWORD dwFlags;
} THREADENTRY32, *LPTHREADENTRY32;
typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(
    LPVOID lpThreadParameter
);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  LPVOID lpSecurityDescriptor;
  BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef struct _SYMBOL_INFO {
    ULONG       SizeOfStruct;
    ULONG       TypeIndex;
    ULONG64     Reserved[2];
    ULONG       Index;
    ULONG       Size;
    ULONG64     ModBase;
    ULONG       Flags;
    ULONG64     Value;
    ULONG64     Address;
    ULONG       Register;
    ULONG       Scope;
    ULONG       Tag;
    ULONG       NameLen;
    ULONG       MaxNameLen;
    CHAR        Name[1];
} SYMBOL_INFO, *PSYMBOL_INFO;
typedef struct _IMAGEHLP_LINE64 {
    DWORD    SizeOfStruct;
    PVOID    Key;
    DWORD    LineNumber;
    PCHAR    FileName;
    DWORD64  Address;
} IMAGEHLP_LINE64, *PIMAGEHLP_LINE64;
typedef IMAGEHLP_LINE64 IMAGEHLP_LINE;
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
typedef struct _NOTIFYICONDATAA {
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    CHAR   szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    CHAR   szInfo[256];
    CHAR   szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID guidItem;
    HICON hBalloonIcon;
} NOTIFYICONDATAA, *PNOTIFYICONDATAA;
typedef LRESULT (*WNDPROC)(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
typedef struct tagWNDCLASSEXA {
    UINT      cbSize;
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
    HICON     hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA;
typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

#define NULL                               0
#define ERROR_SUCCESS                      0L
#define ATTACH_PARENT_PROCESS              ((DWORD)-1)
#define STD_OUTPUT_HANDLE                  ((DWORD)-11)
#define INVALID_HANDLE_VALUE               ((HANDLE)(LONG_PTR)-1)
#define DIGCF_PRESENT                      0x00000002
#define DIGCF_DEVICEINTERFACE              0x00000010
#define GENERIC_READ                       0x80000000L
#define GENERIC_WRITE                      0x40000000L
#define FILE_SHARE_READ                    0x00000001
#define FILE_SHARE_WRITE                   0x00000002
#define OPEN_EXISTING                      3
#define FILE_FLAG_OVERLAPPED               0x40000000
#define ERROR_NO_MORE_ITEMS                259
#define ERROR_IO_PENDING                   997
#define WAIT_ABANDONED_0                   0x00000080L
#define WAIT_OBJECT_0                      0x00000000L
#define WAIT_TIMEOUT                       0x00000102L
#define INPUT_MOUSE                        0
#define INPUT_KEYBOARD                     1
#define INPUT_HARDWARE                     2
#define MOUSEEVENTF_MOVE                   0x0001
#define MOUSEEVENTF_LEFTDOWN               0x0002
#define MOUSEEVENTF_LEFTUP                 0x0004
#define MOUSEEVENTF_ABSOLUTE               0x8000
#define SM_CXSCREEN                        0
#define SM_CYSCREEN                        1
#define SYMOPT_UNDNAME                     0x00000002
#define PM_NOREMOVE                        0x0000
#define PM_REMOVE                          0x0001
#define WM_USER                            0x0400
#define WM_QUIT                            0x0012
#define WM_RBUTTONDOWN                     0x0204
#define WM_COMMAND                         0x0111
#define MB_OK                              0x00000000L
#define MB_ICONERROR                       0x00000010L
#define TH32CS_SNAPTHREAD                  0x00000004
#define THREAD_SUSPEND_RESUME              0x0002
#define EXCEPTION_ACCESS_VIOLATION         0xC0000005L
#define EXCEPTION_BREAKPOINT               0x80000003L
#define EXCEPTION_FLT_DIVIDE_BY_ZERO       0xC000008EL
#define EXCEPTION_INT_DIVIDE_BY_ZERO       0xC0000094L
#define EXCEPTION_NONCONTINUABLE_EXCEPTION 0xC0000025L
#define EXCEPTION_STACK_OVERFLOW           0xC00000FDL
#define EXCEPTION_CONTINUE_SEARCH          0x0
#define INFINITE                           0xFFFFFFFF
#define NIF_MESSAGE                        0x00000001
#define NIF_ICON                           0x00000002
#define NIF_TIP                            0x00000004
#define NIM_ADD                            0x00000000
#define NIM_DELETE                         0x00000002
#define IDI_APPLICATION                    32512
#define MF_STRING                          0x00000000L
#define MF_SEPARATOR                       0x00000800L
#define QS_ALLINPUT                        (0x0002 | 0x0001 | 0x0400 | 0x0008 | 0x0010 | 0x0020 | 0x0040)
#define HIDP_STATUS_SUCCESS                ((NTSTATUS)(0x11 << 16))
#define HIDP_STATUS_INVALID_PREPARSED_DATA ((NTSTATUS)(((0xC) << 28) | (0x11 << 16) | 1))
#define GUID_DEVINTERFACE_HID              (GUID){ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }
#define LOWORD(l)                          ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)                          ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define MAKEINTRESOURCEA(i)                ((LPSTR)((ULONG_PTR)((WORD)(i))))

#define va_start(_list, _arg) ((void)__va_start(&(_list), (_arg)))
#define va_end(_list) ((void)((_list) = (va_list)0))

/* MSVC built-ins */
void __cdecl __va_start(va_list* , ...);

/* kernel32.lib */
DWORD GetLastError(void);
void SetLastError(DWORD dwErrCode);
BOOL WINAPI AttachConsole(DWORD dwProcessId);
HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
void ExitProcess(DWORD code);
BOOL WINAPI WriteConsoleA(
    HANDLE hConsoleOutput,
    const char *buffer,
    DWORD length,
    LPDWORD written,
    LPVOID reserved
);
BOOL WINAPI WriteConsoleW(
    HANDLE hConsoleOutput,
    const WCHAR *buffer,
    DWORD length,
    LPDWORD written,
    LPVOID reserved
);
#define WriteConsole(...) WriteConsoleA(__VA_ARGS__)
HANDLE CreateFileA(
    LPCSTR                lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
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
#define CreateFile(...) CreateFileA(__VA_ARGS__)
BOOL ReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
);
HANDLE CreateEventA(
    PVOID  lpEventAttributes,
    BOOL   bManualReset,
    BOOL   bInitialState,
    LPCSTR lpName
);
#define CreateEvent(...) CreateEventA(__VA_ARGS__)
BOOL SetEvent(HANDLE hEvent);
BOOL ResetEvent(HANDLE hEvent);
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
    DWORD        nCount,
    const HANDLE *lpHandles,
    BOOL         bWaitAll,
    DWORD        dwMilliseconds
);
BOOL CloseHandle(HANDLE hObject);
VOID InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL IsDebuggerPresent(void);
HANDLE GetCurrentProcess(void);
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
);
VOID OutputDebugStringA(LPCSTR lpOutputString);
VOID OutputDebugStringW(LPCWSTR lpOutputString);
#define OutputDebugString(...) OutputDebugStringA(__VA_ARGS__)
DWORD GetCurrentThreadId(void);
DWORD GetCurrentProcessId(void);
HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
BOOL Thread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
BOOL Thread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES   lpThreadAttributes,
    SIZE_T                  dwStackSize,
    LPTHREAD_START_ROUTINE  lpStartAddress,
    LPVOID                  lpParameter,
    DWORD                   dwCreationFlags,
    LPDWORD                 lpThreadId
);
HANDLE OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
DWORD SuspendThread(HANDLE hThread);
VOID Sleep(DWORD dwMilliseconds);
HMODULE WINAPI GetModuleHandleA(LPCSTR lpModuleName);

/* user32.lib */
int GetSystemMetrics(int nIndex);
UINT SendInput(
    UINT    cInputs,
    LPINPUT pInputs,
    int     cbSize
);
BOOL PeekMessageA(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax,
    UINT  wRemoveMsg
);
#define PeekMessage(...) PeekMessageA(__VA_ARGS__)
int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
#define MessageBox(...) MessageBoxA(__VA_ARGS__)
DWORD MsgWaitForMultipleObjects(
    DWORD        nCount,
    const HANDLE *pHandles,
    BOOL         fWaitAll,
    DWORD        dwMilliseconds,
    DWORD        dwWakeMask
);
BOOL PostThreadMessageA(
    DWORD  idThread,
    UINT   Msg,
    WPARAM wParam,
    LPARAM lParam
);
#define PostThreadMessage(...) PostThreadMessageA(__VA_ARGS__)
ATOM RegisterClassExA(const WNDCLASSEXA *cls);
HWND CreateWindowExA(
    DWORD     dwExStyle,
    LPCSTR    lpClassName,
    LPCSTR    lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
);
LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
HICON WINAPI LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName);
BOOL WINAPI TranslateMessage(const MSG *lpMsg);
LRESULT WINAPI DispatchMessageA(const MSG *lpMsg);
HMENU CreatePopupMenu(void);
BOOL AppendMenuA(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCSTR lpNewItem);
BOOL TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int _, HWND hWnd, const RECT *prcRect);
BOOL GetCursorPos(LPPOINT lpPoint);
BOOL SetForegroundWindow(HWND hWnd);

/* vcruntime.lib */
int __cdecl __stdio_common_vsnprintf_s(
    unsigned __int64 _Options,
    const char*      _Buffer,
    size_t           _BufferCount,
    size_t           _MaxCount,
    char const*      _Format,
    _locale_t        _Locale,
    va_list          _ArgList
);
int __cdecl __stdio_common_vsnwprintf_s(
    unsigned __int64 _Options,
    WCHAR*           _Buffer,
    size_t           _BufferCount,
    size_t           _MaxCount,
    WCHAR const*     _Format,
    _locale_t        _Locale,
    va_list          _ArgList
);
void *memset(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);

/* hid.lib */
BOOLEAN HidD_GetAttributes(
    HANDLE           HidDeviceObject,
    PHIDD_ATTRIBUTES Attributes
);
BOOLEAN HidD_GetPreparsedData(
    HANDLE               HidDeviceObject,
    PHIDP_PREPARSED_DATA *PreparsedData
);
BOOLEAN HidD_FreePreparsedData(
    PHIDP_PREPARSED_DATA PreparsedData
);
NTSTATUS HidP_GetCaps(
    PHIDP_PREPARSED_DATA PreparsedData,
    PHIDP_CAPS           Capabilities
);
BOOLEAN HidD_GetFeature(
    HANDLE HidDeviceObject,
    PVOID  ReportBuffer,
    ULONG  ReportBufferLength
);
BOOLEAN HidD_SetFeature(
  HANDLE HidDeviceObject,
  PVOID  ReportBuffer,
  ULONG  ReportBufferLength
);

/* setupapi.lib */
HDEVINFO SetupDiGetClassDevsA(
    const GUID *ClassGuid,
    PCSTR Enumerator,
    HWND hwndParent,
    DWORD Flags
);
BOOL SetupDiEnumDeviceInterfaces(
    HDEVINFO                  DeviceInfoSet,
    PSP_DEVINFO_DATA          DeviceInfoData,
    const GUID                *InterfaceClassGuid,
    DWORD                     MemberIndex,
    PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData
);
BOOL SetupDiGetDeviceInterfaceDetailA(
    HDEVINFO                           DeviceInfoSet,
    PSP_DEVICE_INTERFACE_DATA          DeviceInterfaceData,
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A DeviceInterfaceDetailData,
    DWORD                              DeviceInterfaceDetailDataSize,
    PDWORD                             RequiredSize,
    PSP_DEVINFO_DATA                   DeviceInfoData
);
BOOL SetupDiEnumDeviceInfo(
    HDEVINFO         DeviceInfoSet,
    DWORD            MemberIndex,
    PSP_DEVINFO_DATA DeviceInfoData
);

/* dbghelp.lib */
DWORD SymSetOptions(DWORD SymOptions);
BOOL SymInitialize(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess);
BOOL SymFromAddr(
    HANDLE       hProcess,
    DWORD64      Address,
    PDWORD64     Displacement,
    PSYMBOL_INFO Symbol
);
BOOL SymGetLineFromAddr64(
    HANDLE           hProcess,
    DWORD64          qwAddr,
    PDWORD           pdwDisplacement,
    PIMAGEHLP_LINE64 Line64
);

/* shlwapi.lib */
LPCSTR PathFindFileNameA(LPCSTR pszPath);

/* cfgmgr32.lib */
DWORD CM_Register_Notification(
    PCM_NOTIFY_FILTER   pFilter,
    PVOID               pContext,
    PCM_NOTIFY_CALLBACK pCallback,
    PHCMNOTIFICATION    pNotifyContext
);
DWORD CM_Unregister_Notification(HCMNOTIFICATION NotifyContext);

/* shell32.lib */
BOOL Shell_NotifyIconA(DWORD dwMessage, PNOTIFYICONDATAA lpData);

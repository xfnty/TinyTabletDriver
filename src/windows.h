#pragma once

#if !defined(_WIN64)
    #error Only Win64 is supported.
#endif

typedef int BOOL;
typedef void *HWND;
typedef void *PVOID, *LPVOID;
typedef void *HANDLE;
typedef HANDLE HDEVINFO;
typedef char CHAR;
typedef long LONG;
typedef LONG NTSTATUS;
typedef __int64 LONG_PTR;
typedef unsigned long ULONG, *PULONG;
typedef unsigned __int64 ULONG_PTR;
typedef unsigned int DWORD, *PDWORD, *LPDWORD;
typedef unsigned char UCHAR, *PUCHAR, BYTE, *PBYTE, BOOLEAN;
typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef unsigned short WCHAR;
typedef const char *PCSTR, *LPCSTR;
typedef const WCHAR *PCWSTR;
typedef char *PSTR;
typedef char* va_list;
typedef unsigned __int64 size_t;
typedef PVOID _locale_t;
typedef USHORT USAGE, *PUSAGE;
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

#define WINAPI                             __stdcall
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
#define WAIT_ABANDONED                     0x00000080L
#define WAIT_OBJECT_0                      0x00000000L
#define WAIT_TIMEOUT                       0x00000102L
#define HIDP_STATUS_SUCCESS                ((NTSTATUS)(0x11 << 16))
#define HIDP_STATUS_INVALID_PREPARSED_DATA ((NTSTATUS)(((0xC) << 28) | (0x11 << 16) | 1))
#define GUID_DEVINTERFACE_HID                 (GUID){ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }

#define va_start(_list, _arg) ((void)__va_start(&(_list), (_arg)))
#define va_end(_list) ((void)((_list) = (va_list)0))

/* built-ins */
void __cdecl __va_start(va_list* , ...);

/* kernel32 */
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
HANDLE CreateFileA(
    LPCSTR lpFileName,
    DWORD  dwDesiredAccess,
    DWORD  dwShareMode,
    PVOID  lpSecurityAttributes,
    DWORD  dwCreationDisposition,
    DWORD  dwFlagsAndAttributes,
    HANDLE hTemplateFile
);
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
BOOL CloseHandle(HANDLE hObject);

/* vcruntime */
int __cdecl __stdio_common_vsnprintf_s(
    unsigned __int64 _Options,
    char*            _Buffer,
    size_t           _BufferCount,
    size_t           _MaxCount,
    char const*      _Format,
    _locale_t        _Locale,
    va_list          _ArgList
);

/* hid */
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

/* winusb */
BOOL WinUsb_Initialize(HANDLE DeviceHandle, PWINUSB_INTERFACE_HANDLE InterfaceHandle);
BOOL WinUsb_Free(WINUSB_INTERFACE_HANDLE InterfaceHandle);

/* setupapi */
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

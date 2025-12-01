#include "windows.h"

#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))

void _start(void);
static void WriteConsoleAFormat(HANDLE hConsole, const char *format, ...);
static HANDLE FindDevice(const GUID *guid, USHORT vendor, USHORT product);

void _start(void) {
    AttachConsole(ATTACH_PARENT_PROCESS);
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);

    HANDLE tablet = FindDevice(&GUID_DEVINTERFACE_HID, 1386, 891);
    if (tablet == INVALID_HANDLE_VALUE) {
        WriteConsoleAFormat(hcon, "Tablet not connected.\n");
        ExitProcess(1);
    }

    WINUSB_INTERFACE_HANDLE usb;
    SetLastError(0);
    if (!WinUsb_Initialize(tablet, &usb)) {
        WriteConsoleAFormat(hcon, "WinUSB failed with code %u\n", GetLastError());
    }

    OVERLAPPED ol = {0};

    for (int i = 0; i < 200; i++) {
        unsigned char buffer[10] = {0};

        DWORD bytes_read = 0;
        BOOL ok = ReadFile(tablet, buffer, sizeof(buffer), &bytes_read, &ol);
        DWORD err = GetLastError();
        if (!ok && err != ERROR_IO_PENDING) {
            WriteConsoleAFormat(hcon, "error %u\n", err);
            break;
        }

        GetOverlappedResult(tablet, &ol, &bytes_read, 1);

        unsigned char f = buffer[1];
        unsigned short *x = (unsigned short*)(buffer + 2);
        unsigned short *y = (unsigned short*)(buffer + 4);
        unsigned short *p = (unsigned short*)(buffer + 6);
        WriteConsoleAFormat(
            hcon,
            "T:%d 1:%d 2:%d %.03f %.03f %.02f\n",
            (f&1)!=0,
            (f&2)!=0,
            (f&4)!=0,
            *x / 21600.0f,
            *y / 13500.0f,
            *p / 2047.0f
        );
    }

    CloseHandle(tablet);
    ExitProcess(0);
}

void WriteConsoleAFormat(HANDLE hConsole, const char *format, ...) {
    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    char buffer[2048];
    va_list args;
    va_start(args, format);
    int l = __stdio_common_vsnprintf_s(
        0,
        buffer,
        sizeof(buffer),
        sizeof(buffer) - 1,
        format,
        0,
        args
    );
    va_end(args);

    WriteConsoleA(hConsole, buffer, l, 0, 0);
}

HANDLE FindDevice(const GUID *guid, USHORT vendor, USHORT product) {
    HANDLE dev = INVALID_HANDLE_VALUE;

    HDEVINFO devs = SetupDiGetClassDevsA(
        guid,
        0,
        0,
        DIGCF_DEVICEINTERFACE | DIGCF_PRESENT
    );
    if (devs == INVALID_HANDLE_VALUE)
        return INVALID_HANDLE_VALUE;

    for (int i = 0; ; i++) {
        SP_DEVICE_INTERFACE_DATA iface_data = { .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA) };
        SetupDiEnumDeviceInterfaces(devs, 0, guid, i, &iface_data);
        if (GetLastError() == ERROR_NO_MORE_ITEMS)
            break;

        char devdetailb[1024] = {0};
        PSP_DEVICE_INTERFACE_DETAIL_DATA_A devdetail = (void*)devdetailb;
        devdetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
        SetupDiGetDeviceInterfaceDetailA(devs, &iface_data, devdetail, 1023, 0, 0);

        dev = CreateFileA(
            devdetail->DevicePath,
            GENERIC_READ,
            0,
            0,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            0
        );
        if (dev == INVALID_HANDLE_VALUE)
            goto Continue;

        HIDD_ATTRIBUTES attrs = { .Size = sizeof(attrs) };
        if (!HidD_GetAttributes(dev, &attrs))
            goto Continue;

        if (attrs.VendorID == vendor && attrs.ProductID == product)
            break;

        Continue:
        CloseHandle(dev);
        dev = INVALID_HANDLE_VALUE;
    }

    return dev;
}

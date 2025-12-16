#ifndef _TABD_TABLETS_H
#define _TABD_TABLETS_H

#include "util.h"

#define TABLET_REPORT_POINTER_DOWN 0x01
#define TABLET_REPORT_BUTTON_DOWN(_n) (1 << (_n) + 1)

typedef struct {
    Vec2 point;
    float pressure;
    DWORD flags;
} TabletReport;

typedef struct {
    const WCHAR *name;
    USHORT vid, pid;
    Vec2 measurements;
    BYTE features[64];
    DWORD features_size;
    bool (*Parse)(BYTE *packet, DWORD size, TabletReport *report);
} TabletInfo;

static bool WacomCTL672PacketParser(BYTE *packet, DWORD size, TabletReport *report);

static const TabletInfo s_tablet_infos[] = {
    { L"Wacom CTL-672", 1386, 891, { 216, 135 }, { 0x02, 0x02 }, 2, WacomCTL672PacketParser },
};

bool FindTabletInfo(USHORT vid, USHORT pid, TabletInfo *info) {
    for (unsigned int i = 0; i < COUNTOF(s_tablet_infos); i++) {
        if (s_tablet_infos[i].vid == vid && s_tablet_infos[i].pid == pid) {
            *info = s_tablet_infos[i];
            return true;
        }
    }
    return false;
}

bool WacomCTL672PacketParser(BYTE *packet, DWORD size, TabletReport *report) {
    if (size != 10 || packet[0] != 0x02 || (packet[1] == 0x00 || packet[1] == 0x80))
        return false;

    *report = (TabletReport){
        .point.x  = *(USHORT*)(packet + 2) / (float)0x5460,
        .point.y  = *(USHORT*)(packet + 4) / (float)0x34BC,
        .pressure = *(USHORT*)(packet + 6) / 2047.0f,
        .flags = 
            (packet[1] & 0x01) ? (TABLET_REPORT_POINTER_DOWN) : (0) |
            (packet[1] & 0x02) ? (TABLET_REPORT_BUTTON_DOWN(0)) : (0) |
            (packet[1] & 0x04) ? (TABLET_REPORT_BUTTON_DOWN(1)) : (0),
    };

    return true;
}

#endif /* _TABD_TABLETS_H */

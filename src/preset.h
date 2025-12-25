#ifndef _TABD_PRESET_H
#define _TABD_PRESET_H

#include "util.h"
#include "tablet.h"

typedef struct {
    Vec2 center;
    Vec2 size;
    float rotation;
} ActiveArea; 

typedef enum {
    MODE_MOUSE,
    MODE_INK,
} OutputMode;

typedef struct {
    PCWSTR name;
    ActiveArea area;
    OutputMode mode;
    float pressure_sensitivity;
} Preset;

const Preset g_presets[] = {
    { L"Drawing", { {108, 67.5},      {216, 135},       0 }, MODE_INK,   1.15 },
    { L"Osu",     { {80.41049, 85.5}, {99, 55.66032}, -90 }, MODE_MOUSE, 0    },
};

/* Illustrations are available in docs/preset-transforms.excalidraw */
Vec2 MapTabletPointToScreen(const Preset *preset, const TabletInfo *tablet, Vec2 p) {
    /* convert normalized tablet coordinate to millimeters */
    p = (Vec2){ p.x * tablet->measurements.x, p.y * tablet->measurements.y };

    /* step 1: translate origin */
    p = (Vec2){ p.x - tablet->measurements.x / 2, p.y - tablet->measurements.y / 2 };

    /* step 2: translate preset space to tablet space */
    p = (Vec2){
        p.x + (tablet->measurements.x / 2) - preset->area.center.x,
        p.y + (tablet->measurements.y / 2) - preset->area.center.y
    };

    /* step 3: invert rotation */
    float a = -preset->area.rotation / 180 * 3.1415927f;
    p = (Vec2){ p.x*cos(a) - p.y*sin(a), p.x*sin(a) + p.y*cos(a) };

    /* step 4: scale preset space to tablet space */
    p = (Vec2){
        p.x * tablet->measurements.x / preset->area.size.x,
        p.y * tablet->measurements.y / preset->area.size.y
    };

    /* step 5: translate origin to top-left corner */
    p = (Vec2){ p.x + tablet->measurements.x / 2, p.y + tablet->measurements.y / 2 };

    /* convert from millimeters back to normalized coords */
    p = (Vec2){ p.x / tablet->measurements.x, p.y / tablet->measurements.y };
    return p;
}

#endif /* _TABD_PRESET_H */

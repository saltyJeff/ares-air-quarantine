#pragma once
#include <commondefs.h>
#pragma pack(push, 1)
typedef struct {
    float baroPress;
    float baroTemp;
    float atmoTemp;
    float atmoHumidity;
    Vector accel;
    Vector rot;
    Vector mag;
#ifdef BODYTUBE
    Pressures press;
#else
    Coords coords;
    uint32_t lastFix;
    uint32_t txTime;
#endif
    uint32_t logTime;
} TelemData;
#pragma pack(pop)
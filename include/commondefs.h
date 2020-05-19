#pragma once
#include <cstdint>
#include "Arduino.h"

typedef const char* string_t;
typedef unsigned long ulong;

inline ulong hzToMs(float hz) {
    return 1000 / hz;
}

#pragma pack(push, 1)
typedef struct {
    float x;
    float y;
    float z;
} Vector;
typedef struct {
    float lat;
    float lng;
    float alt;
} Coords;
typedef struct {
    uint16_t CC;
    uint16_t fuel;
    uint16_t OX;
    uint16_t press;
} Pressures;
#pragma pack(pop)
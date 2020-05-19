#pragma once
#include <Task.h>
#include <TinyGPS++.h>

class GPSTask: public Task {
public:
    GPSTask(Coords *coords, uint32_t *lastFix): Task("GPS", hzToMs(10)), 
        coords(coords), lastFix(lastFix), gpsSerial(digitalPinToPinName(4), digitalPinToPinName(3), NC, NC) {}
private:
    Coords *coords;
    uint32_t *lastFix;
    UART gpsSerial;
    TinyGPSPlus gps;
protected:
    void runExec() {
        ulong start = millis();
        char buf[256];
        // read for only up to 70ms
        while( (millis() - start) < 70 && gpsSerial.available() ) {
            size_t len = gpsSerial.readBytes(buf, 256);
            for(size_t i = 0; i < len; i++) {
                gps.encode(buf[i]);
            }
        }
        if(gps.location.isUpdated()) {
            coords->lng = gps.location.lng();
            coords->lat = gps.location.lat();
        }
        if(gps.altitude.isUpdated()) {
            coords->alt = gps.altitude.meters();
        }
        if(gps.date.isUpdated()) {
            *lastFix = gps.date.value();
        }
    }
};
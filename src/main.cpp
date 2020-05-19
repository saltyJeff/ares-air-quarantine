#include "tasks/AirPressureTask.h"
#include "tasks/IMUTask.h"
#include "tasks/TempHumidityTask.h"
#include "writers/SDWriter.h"
#ifdef BODYTUBE
#include "tasks/PTTask.h"
#else
#include "writers/RadioWriter.h"
#include "tasks/GPSTask.h"
#endif

int main() {
    // pre-init buses just in case
    Wire1.begin();
    SPI.begin();

    TelemData data;
    AirPressureTask airPres(&data.baroPress, &data.baroTemp);
    IMUTask imu(&data.accel, &data.rot, &data.mag);
    TempHumidityTask tempHumidity(&data.atmoTemp, &data.atmoHumidity);
    SDWriter sdOut(&data);
#ifdef BODYTUBE
    PTTask pts(&data.press);
    Task *tasks[] = {&airPres, &imu, &tempHumidity, &pts};
#else
    RadioWriter radio(&data);
    radio.enterMaxPower();
    GPSTask gps(&data.coords, &data.lastFix);
    Task *tasks[] = {&airPres, &imu, &tempHumidity, &gps, &radio};
#endif
    const int NUM_TASKS = sizeof(tasks) / sizeof(Task*);

    while(1) {
        bool updated = false;
        for(int i = 0; i < NUM_TASKS; i++) {
            if(!tasks[i]->shouldExec()) {
                continue;
            }
            updated = true;
            tasks[i]->exec();
        }
        if(updated) {
            sdOut.exec();
        }

        char str[512];
        if(Serial.available()) {
            char c = tolower(Serial.read());
            if(c != 'r') {
                continue;
            }
            for(int i = 0; i < NUM_TASKS; i++) {
                tasks[i]->statusString(str);
                Serial.println(str);
            }
            sdOut.statusString(str);
            Serial.println(str);
        }
#ifndef BODYTUBE
        if(Serial1.available()) {
            char c = tolower(Serial1.read());
            if(c != 'r') {
                continue;
            }
            for(int i = 0; i < NUM_TASKS; i++) {
                tasks[i]->statusString(str);
                radio.transmitString(str);
            }
            sdOut.statusString(str);
            radio.transmitString(str);
        }
#endif
    }
}
void handleUsbCmd(char c) {
    if(c == 's') {
        
    }
}
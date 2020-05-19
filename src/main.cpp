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

#include <functional>
typedef std::function<void(char *str)> StringResponder;
typedef std::function<void(TelemData *data)> TelemResponder;

char buf[256];

int main() {
    // pre-init buses just in case
    Wire1.begin();
    SPI.begin();

    TelemData data;
    AirPressureTask airPres(&data.baroPress, &data.baroTemp);
    IMUTask imu(&data.accel, &data.rot, &data.mag);
    TempHumidityTask tempHumidity(&data.atmoTemp, &data.atmoHumidity);
    SDWriter sdCard(&data);
#ifdef BODYTUBE
    PTTask pts(&data.press);
    Task *tasks[] = {&airPres, &imu, &tempHumidity, &pts, &sdCard};
#else
    RadioWriter radio(&data);
    radio.enterMaxPower();
    GPSTask gps(&data.coords, &data.lastFix);
    Task *tasks[] = {&airPres, &imu, &tempHumidity, &gps, &radio, &sdCard};
#endif
    const int NUM_TASKS = sizeof(tasks) / sizeof(Task*);

    while(1) {
        bool updated = false;
        for(int i = 0; i < NUM_TASKS - 1; i++) {
            if(!tasks[i]->shouldExec()) {
                continue;
            }
            updated = true;
            tasks[i]->exec();
        }
        // write to SD card if one of the fields was updated, or the timeout was reached
        if(updated || sdCard.shouldExec()) {
            sdCard.exec();
        }

        // do any command handling (use lambdas to prevent duplication)
        // 1 char commands because i'm saving the string lexing for CS131
        auto handleCommand = [&](Stream *inStream, StringResponder strResp, TelemResponder telemResp) {
            if(inStream->available()) {
                char c = tolower(inStream->read());
                switch (c) {
                case 'r': //R for REPORT
                    for(int i = 0; i < NUM_TASKS; i++) {
                        tasks[i]->statusString(buf);
                        strResp(buf);
                    }
                    break;
                case 'b': //B for BENCHMARK
                    for(int i = 0; i < NUM_TASKS; i++) {
                        tasks[i]->benchmark();
                        tasks[i]->statusString(buf);
                        strResp(buf);
                    }
                    break;
                case 'd': //D for DUMP DATA
                    telemResp(&data);
                    break;
                default:
                    break;
                }
            }
        };


        handleCommand(&Serial, [&](char *str) {
            Serial.println(str);
            sdCard.transmitString(str);
        }, [&](TelemData *data) {
            Serial.write((uint8_t*)data, sizeof(TelemData));
            sdCard.transmitTelem(data);
        });
        #ifndef BODYTUBE
        handleCommand(&Serial1, [&](char *str) {
            radio.transmitString(str);
            sdCard.transmitString(str);
        }, [&](TelemData *data) {
            radio.transmitTelem(data);
            sdCard.transmitTelem(data);
        });
        #endif
    }
}
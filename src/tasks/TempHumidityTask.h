#pragma once
#include <Task.h>
#include <Adafruit_HTS221.h>
#include <Wire.h>

// implemented for the HTS221
class TempHumidityTask: public Task {
public:
    TempHumidityTask(float *temp, float *humidity): Task("Temp&Humidity", hzToMs(12)), temp(temp), humidity(humidity) {
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!hts.begin_I2C(HTS221_I2CADDR_DEFAULT, &Wire1)) {
            status = NO_RESPONSE;
            return;
        }
        hts.setActive(true);
        hts.setDataRate(HTS221_RATE_12_5_HZ);
        if(hts.getDataRate() != HTS221_RATE_12_5_HZ) {
            status = BAD_RESPONSE;
            return;
        }
        status = OK;
    }
private:
    float *temp, *humidity;
    Adafruit_HTS221 hts;
protected:
    virtual void runExec() {
        sensors_event_t tempEvt;
        sensors_event_t humidityEvt;
        if(!hts.getEvent(&tempEvt, &humidityEvt)) {
            status = BAD_RESPONSE;
        }
        *temp = tempEvt.temperature;
        *humidity = humidityEvt.relative_humidity;
    }
};
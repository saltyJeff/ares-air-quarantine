#pragma once
#include <Task.h>
#include <HTS221Sensor.h>
#include <Wire.h>

// implemented for the HTS221
class TempHumidityTask: public Task {
public:
    TempHumidityTask(float *temp, float *humidity): Task("Temp&Humidity", hzToMs(12)), temp(temp), humidity(humidity), hts(&Wire1) {
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!atmoStatusOK(hts.Enable())) {
            return;
        }
        if(!atmoStatusOK(hts.SetODR(12.5))) {
            return;
        }
        float odr = 0;
        if(!atmoStatusOK(hts.GetODR(&odr))) {
            return;
        }
        if(odr != 12.5) {
            status = BAD_RESPONSE;
            return;
        }
        status = OK;
    }
private:
    float *temp, *humidity;
    HTS221Sensor hts;
    bool atmoStatusOK(HTS221StatusTypeDef atmoStatus) {
        if(atmoStatus == HTS221_STATUS_OK) {
            status = OK;
            return true;
        }
        else if(atmoStatus == HTS221_STATUS_TIMEOUT) {
            status = NO_RESPONSE;
        }
        else {
            status = BAD_RESPONSE;
        }
        return false;
    }
protected:
    virtual void runExec() {
        atmoStatusOK(hts.GetHumidity(humidity));
        atmoStatusOK(hts.GetTemperature(temp));
    }
};
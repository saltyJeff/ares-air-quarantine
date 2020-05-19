#pragma once
#include <Task.h>
#include <Wire.h>
#include <LPS22HBSensor.h>
// implemented for the LPS22HB
class AirPressureTask: public Task {
public:
    AirPressureTask(float *press, float *temp): Task("Altimeter", hzToMs(70)), press(press), temp(temp), baro(&Wire1) {
        Wire1.begin();
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!baroStatusOk(baro.Enable())) {
            return;
        }
        if(!baroStatusOk(baro.SetODR(75))) {
            return;
        }
        float odr = 0;
        if(!baroStatusOk(baro.GetODR(&odr))) {
            return;
        }
        if(odr != 75) {
            status = BAD_RESPONSE;
            return;
        }
        status = OK;
    }
private:
    float *press, *temp;
    LPS22HBSensor baro;
    bool baroStatusOk(LPS22HBStatusTypeDef baroStatus) {
        if(baroStatus == LPS22HB_STATUS_OK) {
            return true;
        }
        else if(baroStatus == LPS22HB_STATUS_TIMEOUT) {
            status = NO_RESPONSE;
        }
        else if(baroStatus == LPS22HB_STATUS_ERROR) {
            status = BAD_RESPONSE;
        }
        return false;
    }
protected:
    virtual void runExec() {
        baroStatusOk(baro.GetPressure(press));
        baroStatusOk(baro.GetTemperature(temp));
    }
};
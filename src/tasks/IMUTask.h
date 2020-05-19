#pragma once
#include <Task.h>
#include <Adafruit_LSM9DS1.h>
#include <Wire.h>

// implemented for the LSM9DS1
class IMUTask: public Task {
public:
    IMUTask(Vector *accel, Vector *rot, Vector *mag): 
        Task("IMU", hzToMs(200)), accel(accel), rot(rot), mag(mag), imu(&Wire1) {
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!imu.begin()) {
            status = NO_RESPONSE;
            return;
        }
        imu.setupAccel(imu.LSM9DS1_ACCELRANGE_16G);
        imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);
        imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
        status = OK;
    }
private:
    Vector *accel, *rot, *mag;
    Adafruit_LSM9DS1 imu;
protected:
    virtual void runExec() {
        imu.read();
        sensors_event_t a, m, g, temp;
        if(!imu.getEvent(&a, &m, &g, &temp)) {
            status = NO_RESPONSE;
            return;
        }
        accel->x = a.acceleration.x;
        accel->y = a.acceleration.y;
        accel->z = a.acceleration.z;
        rot->x = g.gyro.x;
        rot->y = g.gyro.y;
        rot->z = g.gyro.z;
        mag->x = m.magnetic.x;
        mag->y = m.magnetic.y;
        mag->z = m.magnetic.z;
    }
};
#pragma once
#include <Task.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Types.h>
#include <Wire.h>

// implemented for the LSM9DS1
class IMUTask: public Task {
public:
    IMUTask(Vector *accel, Vector *rot, Vector *mag): 
        Task("IMU", hzToMs(100)), accel(accel), rot(rot), mag(mag) {
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!imu.begin(LSM9DS1_AG_ADDR(1), LSM9DS1_AG_ADDR(1), Wire1)) {
            status = NO_RESPONSE;
            return;
        }
        imu.setAccelODR(accel_odr::XL_ODR_119);
        imu.setGyroODR(gyro_odr::G_ODR_119);
        imu.setMagODR(mag_odr::M_ODR_80);

        imu.setAccelScale(accel_scale::A_SCALE_16G);
        imu.setGyroScale(gyro_scale::G_SCALE_500DPS);
        imu.setMagScale(mag_scale::M_SCALE_4GS);
        status = OK;
    }
private:
    Vector *accel, *rot, *mag;
    LSM9DS1 imu;
protected:
    virtual void runExec() {
        if(imu.accelAvailable()) {
            imu.readAccel();
            accel->x = imu.ax;
            accel->y = imu.ay;
            accel->z = imu.az;
        }
        if(imu.gyroAvailable()) {
            imu.readGyro();
            rot->x = imu.gx;
            rot->y = imu.gy;
            rot->z = imu.gz;
        }
        if(imu.magAvailable()) {
            imu.readMag();
            mag->x = imu.mx;
            mag->y = imu.my;
            mag->z = imu.mz;
        }
    }
};
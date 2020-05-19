#pragma once
#include <Task.h>

class PTTask: public Task {
public:
    PTTask(Pressures *press): Task("Pressure Transducers", hzToMs(80)), 
        press(press) {}
private:
    Pressures *press;
protected:
    void runExec() {
        press->CC = analogRead(1);
        press->fuel = analogRead(1);
        press->OX = analogRead(1);
        press->press = analogRead(1);
    }
};
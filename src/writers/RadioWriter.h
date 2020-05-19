#pragma once
#include <PacketWriter.h>
#include <Task.h>
#include <TelemData.h>
class RadioWriter: public PacketWriter, public Task {
public:
    RadioWriter(TelemData *telem): Task("Radio", 300), telem(telem) {
        initAndChkStatus();
    }
    void enterStandby() {
        enterAt();
        // set transmit power to 27 dbM
        setAtAttr(4, 27);
        exitAt();
    }
    void enterMaxPower() {
        enterAt();
        // maximum legal TX is 30 dbM
        setAtAttr(4, 30);
        exitAt();
    }
private:
    TelemData *telem;
    void enterAt() {
        Serial1.println("+++");
        delay(1500);
    }
    void setAtAttr(uint16_t key, uint16_t val) {
        Serial1.print("ATS");
        Serial1.print(key);
        Serial1.print('=');
        Serial1.println(val);
    }
    void exitAt() {
        Serial1.println("AT&W");
        Serial1.println("ATZ");
    }
protected:
    virtual void write(uint8_t c) {
        Serial1.write(c);
    }
    virtual void runExec() {
        telem->txTime = millis();
        transmitTelem(telem);
    }
};
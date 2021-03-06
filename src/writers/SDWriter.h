#pragma once
#include <PacketWriter.h>
#include <Task.h>
#include <TelemData.h>
#include <SdFat.h>

class SDWriter: public PacketWriter, public Task {
public:
    SDWriter(TelemData *telem): Task("SD Card", hzToMs(0.75)), telem(telem) {
        initAndChkStatus();
    }
    virtual void initAndChkStatus() {
        if(!sd.begin(10, SD_SCK_MHZ(50))) {
            status = NO_RESPONSE;
            return;
        }
        char filename[128];
        // seek the next available filename
        // (probably a better way to do this deterministiclly)
        int i = 0;
        do {
            sprintf(filename, "rocketdata%d.bin", i);
            i++;
        } while(sd.exists(filename));
        if(!file.open(filename, O_CREAT | O_WRITE | O_TRUNC)) {
            status = BAD_RESPONSE;
            return;
        }
        status = OK;
    }
private:
    TelemData *telem;
    SdFat sd;
    SdBaseFile file;
protected:
    virtual void write(uint8_t c) {
        file.write(c);
    }
    virtual void runExec() {
        telem->logTime = millis();
        transmitTelem(telem);
    }
};
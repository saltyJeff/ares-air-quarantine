#pragma once
#include <commondefs.h>
#include <cstring>
#include <TelemData.h>
#include <Task.h>

typedef enum {
    TELEM = 0,
    STATUS = 1
} MSG_TYPE;

const uint8_t HDLC_DELIM = 0x7E;
const uint8_t HDLC_ESC = 0x7D;

class PacketWriter {
public:
    void transmitString(char *str) {
        transmitPkg(STATUS, (uint8_t*)str, strlen(str));
    }
    void transmitTelem(TelemData* telem) {
        transmitPkg(TELEM, (uint8_t*) telem, sizeof(TelemData));
    }
protected:
    virtual void write(uint8_t c) = 0;
    void writeEscaped(uint8_t c) {
        if(c == HDLC_DELIM || c == HDLC_ESC) {
            write(HDLC_ESC);
            write(c ^ 0x20);
        }
        else {
            write(c);
        }
    }
private:
    uint16_t calcChecksum(uint8_t *buf, int len);
    void transmitPkg(MSG_TYPE type, uint8_t* buf, int len);
};
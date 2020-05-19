#include <PacketWriter.h>

void PacketWriter::transmitPkg(MSG_TYPE type, uint8_t* buf, int len) {
    write(HDLC_DELIM);
    uint16_t checksum = calcChecksum(buf, len);
    uint8_t *checksumPtr = (uint8_t*) &checksum;
    write(checksumPtr[0]);
    write(checksumPtr[1]);
    for(int i = 0; i < len; i++) {
        uint8_t c = buf[i];
        if(c == HDLC_DELIM || c == HDLC_ESC) {
            write(HDLC_ESC);
            write(c ^ 0x20);
        }
        else {
            write(c);
        }
    }
}

// taken from https://github.com/brandondahler/retter/blob/master/Fletcher/fletcher16.c
uint16_t PacketWriter::calcChecksum(uint8_t *data, int bytes) {
    uint16_t sum1 = 0xff, sum2 = 0xff;
    while (bytes) {
            size_t tlen = bytes > 20 ? 20 : bytes;
            bytes -= tlen;
            do {
                    sum2 += sum1 += *data++;
            } while (--tlen);
            sum1 = (sum1 & 0xff) + (sum1 >> 8);
            sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }
    /* Second reduction step to reduce sums to 8 bits */
    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;
}
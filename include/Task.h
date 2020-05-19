#pragma once
#include <commondefs.h>

class PerfStats {
public:
    ulong min = -1;
    float average = -1;
    ulong max = -1;
    ulong samples = 0;
    void reset() {
        min = average = max = -1;
    }
    void update(ulong time) {
        samples++;
        if(time < min || samples == 1) {
            min = time;
        }
        if(time > max || samples == 1) {
            max = time;
        }
        if(samples == 1) {
            average = time;
        }
        else {
            average = average + (time - average) / samples;
        }
    }
};

typedef enum {
    UNKNOWN,
    OK,
    NO_RESPONSE,
    BAD_RESPONSE
} TaskStatus;

inline string_t statusDesc(TaskStatus status) {
    switch(status) {
    case UNKNOWN:
        return "UNKNOWN";
    case OK:
        return "OK";
    case NO_RESPONSE:
        return "NO RESPONSE";
    case BAD_RESPONSE:
        return "BAD RESPONSE";
    default:
        return "!INVALID!";
    }
}

class Task {
public:
    string_t name;
    ulong sampleRate;
    ulong lastSample;
    PerfStats stats;
    TaskStatus status = UNKNOWN;
    Task(string_t name, ulong sampleRate): name(name), sampleRate(sampleRate) {};

    void exec() {
        if(status != OK) {
            return;
        }
        ulong start = millis();
        runExec();
        ulong end = millis();
        ulong duration = end - start;
        stats.update(duration);
        lastSample = end;
    }
    bool shouldExec() {
        return sampleRate == 0 || millis() - lastSample >= sampleRate;
    }
    virtual void initAndChkStatus() {
        status = OK;
    }
    // edit if your benchmarking task is different (you will need to do your own performance measurement inside)
    virtual void benchmark() {
        stats.reset();
        for(int i = 0; i < 15; i++) {
            exec();
        }
    }
    void statusString(char *str) {
        sprintf(str, "%s: %s (avg=%d\tmin=%lu\tmax=%lu)", name, statusDesc(status), (int)stats.average, stats.min, stats.max);
    }
protected:
    virtual void runExec() = 0;
};
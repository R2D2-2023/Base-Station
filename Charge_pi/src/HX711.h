#ifndef HX711_H
#define HX711_H
#include <cstdint>
#include <sys/types.h>
#include <cstdio>
#include <sched.h>
#include <cstring>
#include <cstdlib>
#include <wiringPi.h>
#include <iostream>

class HX711{
private:
    long offset;
    int argc;
    int clock_pin;
    int data_pin;

public:
    HX711(int clock_pin, int data_pin):clock_pin(clock_pin),
                                       data_pin(data_pin){};
    void resetConverter();
    long readCnt();
    void setGain(int r);
    void setHighPri (void);
    static uint8_t sizeCVT(const int read);
    void powerDownHX711();
    void setupGPIO();

    void start(long _offset, int _argc = 0){
        offset = _offset;
        argc = _argc;
        setHighPri();
        setupGPIO();
        std::cout << "Scale start()" << std::endl;
    }
    long getAvg(int samples){
        long tmp_avg = 0;
        for(int i=0;i<samples;i++) {
            tmp_avg += readCnt();
        }
        return (tmp_avg/samples);
    }
};
#endif

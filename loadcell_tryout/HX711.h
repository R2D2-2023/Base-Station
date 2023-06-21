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
    int CLOCK_PIN;
    int DATA_PIN;

public:
    HX711(int clock_pin, int data_pin):CLOCK_PIN(clock_pin),
                                        DATA_PIN(data_pin){};
    void reset_converter();
    long read_cnt();
    void set_gain(int r);
    void setHighPri (void);
    static uint8_t sizecvt(const int read);
    void power_down_hx711();
    void setup_gpio();

    void start(long _offset, int _argc){
        offset = _offset;
        argc = _argc;
        setHighPri();
        setup_gpio();
        std::cout << "Scale start()" << std::endl;
    }
    long getAvg(int samples){
        long tmp_avg = 0;
        for(int i=0;i<samples;i++) {
            tmp_avg += read_cnt();
        }
        return (tmp_avg/samples);
    }

    int data_pin = DATA_PIN;
    int clock_pin = CLOCK_PIN;
};
#endif

/*
 gurov was here, use this code, or don't, whatever, I don't care. If you see a giant bug with a billion legs, please let me know so it can be squashed
gcc -o hx711wp hx711wp.c -lwiringPi
*/
#include "HX711.h"


void HX711::setHighPri()
{
    struct sched_param sched{} ;

    memset (&sched, 0, sizeof(sched)) ;

    sched.sched_priority = 10 ;
    if (sched_setscheduler (0, SCHED_FIFO, &sched))
        printf ("Warning: Unable to set high priority\n") ;
}

uint8_t HX711::sizecvt(const int read)
{
    /* digitalRead() and friends from wiringpi are defined as returning a value
       < 256. However, they are returned as int() types. This is a safety function */

    if (read > 255 || read < 0)
    {
        printf("Invalid data from wiringPi library\n");
        exit(EXIT_FAILURE);
    }
    return (uint8_t)read;
}


void HX711::power_down_hx711()
{
    digitalWrite(clock_pin, HIGH);
}

void HX711::setup_gpio()
{
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, INPUT);
    digitalWrite(clock_pin, LOW);
}


void HX711::reset_converter(void) {
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(40);
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(60);
}

void HX711::set_gain(int r) {
    int i;

// r = 0 - 128 gain ch a
// r = 1 - 32  gain ch b
// r = 2 - 63  gain ch a

    while( sizecvt(digitalRead(data_pin)) );

    for (i=0;i<24+r;i++) {
        digitalWrite(clock_pin, HIGH);
        delayMicroseconds(1);
        digitalWrite(clock_pin, LOW);
        delayMicroseconds(1);
    }
}


long HX711::read_cnt() {
    long count;
    int i;


    count = 0;


    while( sizecvt(digitalRead(data_pin)) );
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);

    for(i=0;i<24	; i++) {
        digitalWrite(clock_pin, HIGH);
        delayMicroseconds(1);
        count = count << 1;
        if ( sizecvt(digitalRead(data_pin)) > 0 )  { count++; }
        digitalWrite(clock_pin, LOW);
        delayMicroseconds(1);
    }

    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);

//  count = ~0x1800000 & count;
//  count = ~0x800000 & count;


    if (count & 0x800000) {
        count |= (long) ~0xffffff;
    }

// if things are broken this will show actual data


    if (argc < 2 ) {
        for (i=32;i;i--) {
//            printf("%d ", ((count-offset) & ( 1 << i )) > 0 );
        }

//        printf("n: %10d     -  ", count - offset);
//        printf("\n");
    }
    return (count - offset) / 100; // Divide by 100 to get a smaller number :)

}
#ifndef METER_H
#define METER_H

#include <Arduino.h>

class Meter
{
    private:
        static constexpr int CNT_PIN = 4;
        static Meter* instance;
        volatile int counter = 0;
        static void IRAM_ATTR handleInterrupt();

    public:
        volatile bool clicked = false;
        Meter();
        int read();
};

#endif
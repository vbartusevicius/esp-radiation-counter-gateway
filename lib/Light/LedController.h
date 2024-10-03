#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LedController
{
    private:
        unsigned long lastBlink = 0;
        unsigned const int blinkMicros = 1000;

    public:
        LedController();
        void run(bool clicked);
};

#endif
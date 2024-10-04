#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LedController
{
    private:
        unsigned long lastBlink = 0;
        unsigned const int blinkMicros = 1000;
        bool clicked = false;

    public:
        LedController();
        void run();
        void click();
};

#endif
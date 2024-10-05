#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LedController
{
    private:
        unsigned static const LED_ID = LED_BUILTIN;
        unsigned long lastBlink = 0;
        unsigned const long blinkDuration = 50;
        bool clicked = false;

    public:
        LedController();
        void run();
        void click();
};

#endif
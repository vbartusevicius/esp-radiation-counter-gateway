#ifndef CLICK_EVENT_H
#define CLICK_EVENT_H

#include <Arduino.h>
#include "Meter.h"
#include "LedController.h"

class ClickEvent
{
    public:
        static constexpr int CNT_PIN = 4;

        ClickEvent(Meter* meter, LedController* led);
        void handleInterrupt();
    
    private:
        Meter* meter;
        LedController* led;
};

#endif

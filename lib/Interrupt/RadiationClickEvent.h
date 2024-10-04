#ifndef RADIATION_CLICK_EVENT_H
#define RADIATION_CLICK_EVENT_H

#include <Arduino.h>
#include "Meter.h"
#include "LedController.h"

class RadiationClickEvent
{
    public:
        static constexpr int CNT_PIN = 4;

        RadiationClickEvent(Meter* meter, LedController* led);
        void handleInterrupt();
    
    private:
        Meter* meter;
        LedController* led;
};

#endif

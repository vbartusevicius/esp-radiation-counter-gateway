#ifndef BUTTON_CLICK_EVENT_H
#define BUTTON_CLICK_EVENT_H

#include <Arduino.h>
#include "LedController.h"

class ButtonClickEvent
{
    public:
        static constexpr int BTN_PIN = 0;
        int counter = 0;

        ButtonClickEvent();
        void handleInterrupt();
};

#endif

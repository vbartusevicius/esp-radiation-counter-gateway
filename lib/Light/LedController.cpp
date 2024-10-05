#include "LedController.h"

LedController::LedController()
{
    pinMode(LED_ID, OUTPUT);
}

void LedController::click()
{
    this->clicked = true;
}

void LedController::run()
{
    unsigned long now = millis();

    if ((this->lastBlink + LedController::blinkDuration) > now) {
        return;
    }

    digitalWrite(LED_ID, 1);

    if (this->clicked == true) {
        this->lastBlink = now;
        digitalWrite(LED_ID, 0);
        this->clicked = false;
    }
}

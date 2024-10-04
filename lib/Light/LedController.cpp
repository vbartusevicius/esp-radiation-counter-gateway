#include "LedController.h"

LedController::LedController()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void LedController::click()
{
    this->clicked = true;
}

void LedController::run()
{
    auto now = micros();
    if (this->lastBlink + LedController::blinkMicros > now) {
        return;
    }
    digitalWrite(LED_BUILTIN, 0);

    if (this->clicked == true) {
        this->lastBlink = now;
        digitalWrite(LED_BUILTIN, 1);
        this->clicked = false;
    }
}

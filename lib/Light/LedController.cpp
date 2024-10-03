#include "LedController.h"

LedController::LedController()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void LedController::run(bool clicked)
{
    auto now = micros();
    if (this->lastBlink + LedController::blinkMicros > now) {
        return;
    }
    digitalWrite(LED_BUILTIN, 0);

    if (clicked == true) {
        this->lastBlink = now;
        digitalWrite(LED_BUILTIN, 1);
    }
}

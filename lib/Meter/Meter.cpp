#include "Meter.h"

Meter::Meter()
{
    Meter::instance = this;

    pinMode(this->CNT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(this->CNT_PIN), handleInterrupt, CHANGE);
}

void IRAM_ATTR Meter::handleInterrupt()
{
    if (instance == nullptr) {
        return;
    }

    int pinState = digitalRead(instance->CNT_PIN);

    if (pinState == HIGH) {
        instance->counter++;
        instance->clicked = true;
    }
}

int Meter::read()
{
    noInterrupts();
    int clicks = this->counter;
    this->counter = 0;
    interrupts();
    return clicks;
}

Meter* Meter::instance = nullptr;

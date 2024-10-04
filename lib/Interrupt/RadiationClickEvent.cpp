#include "RadiationClickEvent.h"

RadiationClickEvent::RadiationClickEvent(Meter* meter, LedController* led)
{
    this->meter = meter;
    this->led = led;

    pinMode(this->CNT_PIN, INPUT);
}

void RadiationClickEvent::handleInterrupt()
{
    int pinState = digitalRead(this->CNT_PIN);

    if (pinState == HIGH) {
        this->meter->increment();
        this->led->click();
    }
}

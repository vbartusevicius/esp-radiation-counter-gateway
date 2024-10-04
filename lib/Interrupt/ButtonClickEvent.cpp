#include "ButtonClickEvent.h"

ButtonClickEvent::ButtonClickEvent()
{
    pinMode(this->BTN_PIN, INPUT);
}

void ButtonClickEvent::handleInterrupt()
{
    int pinState = digitalRead(this->BTN_PIN);

    if (pinState == LOW) {
        this->counter++;
    }
}

#include "Meter.h"

Meter::Meter()
{
}

void Meter::increment()
{
    this->counter++;
}

int Meter::read()
{
    int clicks = this->counter;
    this->counter = 0;
    return clicks;
}

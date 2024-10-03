#include "Aggregator.h"
#include "Parameter.h"

Aggregator::Aggregator(Storage* storage)
{
    this->storage = storage;
}

Result Aggregator::aggregate(int value)
{
    this->buffer.push_back(value);

    if (this->buffer.size() > Aggregator::MAX_SIZE) {
        this->buffer.erase(this->buffer.begin());
    }

    int cpm = 0;
    for (auto &element : this->buffer) {
        cpm += element;
    }

    float tubeFactor = this->storage->getParameter(Parameter::TUBE_CONVERSION_FACTOR, "120").toFloat();
    float dose = cpm / tubeFactor;

    return {cpm, dose};
}

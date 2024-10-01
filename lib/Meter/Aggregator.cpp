#include "Aggregator.h"
#include "Parameter.h"

Aggregator::Aggregator(Storage* storage)
{
    this->storage = storage;
}

float Aggregator::aggregate(float value)
{
    const int averageWindow = 100; // atol(this->storage->getParameter(Parameter::AVG_SAMPLE_COUNT, "10").c_str());
    
    if (this->buffer.size() + 1 > averageWindow) {
        this->buffer.clear();
    }
    if (this->buffer.size() >= averageWindow) {
        this->buffer.erase(this->buffer.begin());
    }
    this->buffer.push_back(value);

    float data = 0.0;

    for (auto &element : this->buffer) {
        data += element;
    }

    return data / this->buffer.size();
}
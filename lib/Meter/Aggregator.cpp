#include "Aggregator.h"
#include "Parameter.h"

Aggregator::Aggregator(Storage* storage)
{
    this->storage = storage;
}

vector<float> Aggregator::aggregate(Result result)
{
    this->spanPointer++;
    int spanSize = this->storage->getParameter(Parameter::DISPLAY_GRAPH_RESOLUTION, "6").toInt();
    // int spanSize = this->storage->getParameter(Parameter::DISPLAY_GRAPH_RESOLUTION, "600").toInt();

    this->processSpanBuffer(result.dose, spanSize);

    if (this->spanPointer < spanSize) {
        return this->totalBuffer;
    }

    this->spanPointer = 0;
    this->processTotalBuffer();

    return this->totalBuffer;
}

void Aggregator::processSpanBuffer(float value, int spanSize)
{
    this->spanBuffer.push_back(value);

    if (this->spanBuffer.size() > spanSize) {
        this->spanBuffer.erase(this->spanBuffer.begin());
    }
}

void Aggregator::processTotalBuffer()
{
    float data = 0.0;
    for (auto &element : this->spanBuffer) {
        data += element;
    }
    float spanDose = data / this->spanBuffer.size();

    this->totalBuffer.push_back(spanDose);

    if (this->totalBuffer.size() > Aggregator::BUFFER_SIZE) {
        this->totalBuffer.erase(this->totalBuffer.begin());
    }
}

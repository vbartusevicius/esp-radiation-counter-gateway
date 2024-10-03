#include "Aggregator.h"
#include "Parameter.h"

Aggregator::Aggregator(Storage* storage)
{
    this->storage = storage;
}

vector<float> Aggregator::aggregate(Result result)
{
    this->spanPointer++;
    int spanSize = this->storage->getParameter(Parameter::DISPLAY_GRAPH_RESOLUTION, "600").toInt();

    this->processSpanBuffer(result.dose, spanSize);
    this->processMinuteBuffer(result);

    if (this->spanPointer < spanSize) {
        return;
    }

    this->spanPointer = 0;
    this->processTotalBuffer();
}

void Aggregator::processMinuteBuffer(Result result)
{
    this->minuteBufferCpm.push_back(result.cpm);

    if (this->minuteBufferCpm.size() > 60) {
        this->minuteBufferCpm.erase(this->minuteBufferCpm.begin());
    }

    this->minuteBufferDose.push_back(result.dose);

    if (this->minuteBufferDose.size() > 60) {
        this->minuteBufferDose.erase(this->minuteBufferDose.begin());
    }
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

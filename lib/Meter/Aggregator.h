#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <queue>
#include "Result.cpp"
#include "Storage.h"

using namespace std;

class Aggregator
{
    private:
        static const int BUFFER_SIZE = 128; // number of cols on LCD
        int spanPointer = 0;
        Storage* storage;
        vector<float> spanBuffer;
        vector<float> totalBuffer;

        void processTotalBuffer();
        void processSpanBuffer(float value, int spanSize);
    
    public:
        Aggregator(Storage* storage);
        vector<float> aggregate(Result result);
};

#endif
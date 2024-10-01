#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <queue>
#include "Storage.h"

using namespace std;

class Aggregator
{
    private:
        vector<float> buffer;
        Storage* storage;
    
    public:
        Aggregator(Storage* storage);
        float aggregate(float value);

};

#endif
#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <queue>
#include "Storage.h"
#include "Result.cpp"

using namespace std;

class Aggregator
{
    private:
        static const int MAX_SIZE = 60;
        vector<int> buffer;
        Storage* storage;
    
    public:
        Aggregator(Storage* storage);
        Result aggregate(int value);
};

#endif
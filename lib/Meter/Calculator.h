#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <queue>
#include "Storage.h"
#include "Result.cpp"

using namespace std;

class Calculator
{
    private:
        static const int MAX_SIZE = 60;
        vector<int> buffer;
        Storage* storage;
    
    public:
        Calculator(Storage* storage);
        Result calculate(int value);
};

#endif
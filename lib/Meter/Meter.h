#ifndef METER_H
#define METER_H

class Meter
{
    private:
        int counter = 0;

    public:
        Meter();
        int read();
        void increment();
};

#endif
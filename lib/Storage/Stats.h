
#ifndef STATS_H
#define STATS_H

#include <Arduino.h>
#include <IPAddress.h>
#include <queue>
#include "Result.cpp"
#include "Storage.h"

using namespace std;

class Stats
{
    private:
        Storage* storage;
    
    public:
        char wifiSignal[16];
        char ipAddress[16];
        char network[32];
        char uptime[16];
        bool mqttConnected = false;

        int spanSize = 0;
        int cpm = 0;
        float dose = 0.0;
        vector<float> buffer;

    public:
        Stats(Storage* storage);
        void updateStats(
            bool mqttConnected,
            Result result,
            vector<float> buffer
        );
};

#endif
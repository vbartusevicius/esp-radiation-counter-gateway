
#ifndef STATS_H
#define STATS_H

#include <Arduino.h>
#include <IPAddress.h>
#include <queue>
#include "Result.cpp"

using namespace std;

class Stats
{
    public:
        String wifiSignal = "";
        String ipAddress = "";
        String network = "";
        String uptime = "";
        
        bool mqttConnected = false;
        int cpm = 0;
        float dose = 0.0;
        vector<float> buffer;

    public:
        Stats();
        void updateStats(
            bool mqttConnected,
            Result result,
            vector<float> buffer
        );
};

#endif
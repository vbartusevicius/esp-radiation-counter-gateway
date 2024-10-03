
#ifndef STATS_H
#define STATS_H

#include <Arduino.h>
#include <IPAddress.h>

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

    public:
        Stats();
        void updateStats(
            bool mqttConnected,
            int cpm,
            float dose
        );
};

#endif
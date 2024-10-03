#include "Stats.h"
#include "TimeHelper.h"
#include <WiFi.h>

Stats::Stats() {}

void Stats::updateStats(
    bool mqttConnected,
    int cpm,
    float dose
)
{
    this->mqttConnected = mqttConnected;
    this->cpm = cpm;
    this->dose = dose;

    this->ipAddress = String(WiFi.localIP().toString());
    this->network = String(WiFi.SSID());
    this->wifiSignal = String(WiFi.RSSI());

    char buffer[32];
    TimeHelper::getUptime(buffer);
    this->uptime = String(buffer);
}

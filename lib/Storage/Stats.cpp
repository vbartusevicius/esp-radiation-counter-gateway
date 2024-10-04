#include "Stats.h"
#include "TimeHelper.h"
#include <WiFi.h>

Stats::Stats() {}

void Stats::updateStats(
    bool mqttConnected,
    Result result,
    vector<float> buffer
)
{
    this->mqttConnected = mqttConnected;
    this->cpm = result.cpm;
    this->dose = result.dose;
    this->buffer = buffer;

    this->ipAddress = String(WiFi.localIP().toString());
    this->network = String(WiFi.SSID());
    this->wifiSignal = String(WiFi.RSSI());

    char uptimeBuf[32];
    TimeHelper::getUptime(uptimeBuf);
    this->uptime = String(uptimeBuf);
}

#include "Stats.h"
#include "TimeHelper.h"
#include "Parameter.h"
#include <WiFi.h>

Stats::Stats(Storage* storage) 
{
    this->storage = storage;
}

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
    //todo: restore meaningful default
    this->spanSize = this->storage->getParameter(Parameter::DISPLAY_GRAPH_RESOLUTION, "600").toInt();

    this->ipAddress = String(WiFi.localIP().toString());
    this->network = String(WiFi.SSID());
    this->wifiSignal = String(WiFi.RSSI());

    char uptimeBuf[32];
    TimeHelper::getUptime(uptimeBuf);
    this->uptime = String(uptimeBuf);
}

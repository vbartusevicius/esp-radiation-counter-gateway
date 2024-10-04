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
) {
    this->mqttConnected = mqttConnected;
    this->cpm = result.cpm;
    this->dose = result.dose;
    this->buffer = buffer;
    this->spanSize = this->storage->getParameter(Parameter::DISPLAY_GRAPH_RESOLUTION, "600").toInt();

    int32_t rssi = WiFi.RSSI();
    snprintf(this->wifiSignal, sizeof(this->wifiSignal), "%d", rssi);

    snprintf(this->ipAddress, sizeof(this->ipAddress), "%s", WiFi.localIP().toString().c_str());

    snprintf(this->network, sizeof(this->network), "%s", WiFi.SSID().c_str());

    TimeHelper::getUptime(this->uptime);
}

#ifndef MY_MQTT_CLIENT_H
#define MY_MQTT_CLIENT_H

#include <MQTT.h>
#include "Storage.h"
#include "Logger.h"

class MqttClient
{
    private:
        Storage* storage;
        Logger* logger;
        MQTTClient client;
        unsigned long lastReconnectAttempt;
        unsigned const int reconnectPause = 10000;

    public:
        MqttClient(Storage* storage, Logger* logger);
        void begin();
        bool connect();
        bool run();
        void sendMetrics(int cpm, float dose);

    private:
        void publishHomeAssistantAutoconfig();
};

#endif
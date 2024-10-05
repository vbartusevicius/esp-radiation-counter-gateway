#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "MqttClient.h"
#include "Parameter.h"
#include "ChipId.h"

extern WiFiClient network;

MqttClient::MqttClient(Storage* storage, Logger* logger)
{
    this->storage = storage;
    this->logger = logger;

    this->lastReconnectAttempt = 0;
}

bool MqttClient::connect()
{
    String username = this->storage->getParameter(Parameter::MQTT_USER);
    String password = this->storage->getParameter(Parameter::MQTT_PASS);
    String device = this->storage->getParameter(Parameter::MQTT_DEVICE);

    std::function<bool()> connection;

    if (username == "" && password == "") {
        connection = [device, this]() -> bool { 
            return client.connect(device.c_str()); 
        };
    } else {
        connection = [device, username, password, this]() -> bool {
            return client.connect(device.c_str(), username.c_str(), password.c_str());
        };
    }

    auto result = connection();

    if (result) {
        this->logger->info("Conected to MQTT.");
    } else {
        this->logger->warning("Failed to connect to MQTT.");
        this->lastReconnectAttempt = millis();
    }

    return result;
}

void MqttClient::begin()
{
    client.begin(
        this->storage->getParameter(Parameter::MQTT_HOST).c_str(),
        atoi(this->storage->getParameter(Parameter::MQTT_PORT).c_str()),
        network
    );

    this->connect();
    this->publishHomeAssistantAutoconfig();
}

void MqttClient::publishHomeAssistantAutoconfig()
{
    JsonDocument doc;
    String deviceId = this->storage->getParameter(Parameter::MQTT_DEVICE) + "_" + ChipId::getChipId();
    String stateTopic = this->storage->getParameter(Parameter::MQTT_TOPIC_MEASUREMENT);

    JsonDocument deviceDoc;
    JsonObject device = deviceDoc.to<JsonObject>();
    device["name"] = "ESP radiation counter";
    device["identifiers"][0] = deviceId;
    device["manufacturer"] = "VB";
    device["model"] = "ESP radiation counter v1";

    JsonDocument cpmDoc;
    cpmDoc["name"] = "CPM sensor";
    cpmDoc["state_topic"] = stateTopic;
    cpmDoc["value_template"] = "{{ value_json.cpm | int }}";
    cpmDoc["unit_of_measurement"] = "CPM";
    cpmDoc["unique_id"] = deviceId + String("_cpm_sensor");
    cpmDoc["device"] = device;

    JsonDocument doseDoc;
    doseDoc["name"] = "Dose sensor";
    doseDoc["state_topic"] = stateTopic;
    doseDoc["value_template"] = "{{ (value_json.dose | float) | round(2) }}";
    doseDoc["unit_of_measurement"] = "µSv/h";
    doseDoc["unique_id"] = deviceId + String("_dose_sensor");
    doseDoc["device"] = device;

    char buffer[512];

    serializeJson(cpmDoc, buffer);
    client.publish(String("homeassistant/sensor/") + String(cpmDoc["unique_id"]) + String("/config"), buffer, true, 1);

    serializeJson(doseDoc, buffer);
    client.publish(String("homeassistant/sensor/") + String(doseDoc["unique_id"]) + String("/config"), buffer, true, 1);
}

bool MqttClient::run()
{
    client.loop();
    delay(10);

    if (client.connected()) {
        return true;
    }
    
    this->logger->warning("Connection to MQTT lost, reconnecting.");

    if (this->lastReconnectAttempt + this->reconnectPause < millis()) {
        client.disconnect();
        return this->connect();
    }
    auto error = client.lastError();
    auto returnCode = client.returnCode();
    this->logger->warning("Last MQTT error: " + String(error));
    this->logger->warning("Last MQTT return code: " + String(returnCode));

    return false;
}

void MqttClient::sendMetrics(int cpm, float dose)
{
    JsonDocument doc;
    String json;

    doc["cpm"] = cpm;
    doc["dose"] = dose;
    serializeJson(doc, json);

    auto ok = client.publish(this->storage->getParameter(Parameter::MQTT_TOPIC_MEASUREMENT), json);

    if (!ok) {
        this->logger->warning("Failed to publish to MQTT: " + json);
    }
}

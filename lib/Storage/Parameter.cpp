#include "Parameter.h"

using std::vector;

char* Parameter::MQTT_HOST = "mqtt_host";
char* Parameter::MQTT_PORT = "mqtt_port";
char* Parameter::MQTT_USER = "mqtt_user";
char* Parameter::MQTT_PASS = "mqtt_pass";
char* Parameter::MQTT_DEVICE = "mqtt_device";
char* Parameter::MQTT_TOPIC_MEASUREMENT = "mqtt_topic_measurement";
char* Parameter::TUBE_CONVERSION_FACTOR = "tube_conversion_factor";

vector<char*> Parameter::REQUIRED_PARAMS = {
    Parameter::MQTT_HOST,
    Parameter::MQTT_PORT,
    Parameter::MQTT_DEVICE
};
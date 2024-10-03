#ifndef PARAMETER_H
#define PARAMETER_H

#include <queue>
using std::vector;

class Parameter
{
    public:
        static char* MQTT_HOST;
        static char* MQTT_PORT;
        static char* MQTT_USER;
        static char* MQTT_PASS;
        static char* MQTT_DEVICE;
        static char* MQTT_TOPIC_MEASUREMENT;
        static char* TUBE_CONVERSION_FACTOR;
        static char* DISPLAY_GRAPH_RESOLUTION;

        static vector<char*> REQUIRED_PARAMS;
};

#endif

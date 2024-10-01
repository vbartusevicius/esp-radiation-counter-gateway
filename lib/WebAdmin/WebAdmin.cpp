#include "WebAdmin.h"
#include "Parameter.h"

extern WebAdmin* admin;

WebAdmin::WebAdmin(Storage* storage, Logger* logger, ResetCallback resetCallback)
{
    this->storage = storage;
    this->logger = logger;
    this->resetCallback = resetCallback;
}

char* WebAdmin::SUBMIT = "submit";
char* WebAdmin::RESET = "reset";

void WebAdmin::begin()
{
    String labelStyle = "width: 100%; text-align: left; font-weight: 500;";

    ESPUI.addControl(ControlType::Separator, "Sensor settings");


    ESPUI.addControl(ControlType::Separator, "MQTT settings");
    this->addControl(
        ControlType::Text, 
        "Host", 
        this->storage->getParameter(Parameter::MQTT_HOST),
        Control::noParent,
        Parameter::MQTT_HOST
    );
    this->addControl(
        ControlType::Number, 
        "Port", 
        this->storage->getParameter(Parameter::MQTT_PORT),
        Control::noParent,
        Parameter::MQTT_PORT
    );
    this->addControl(
        ControlType::Text, 
        "Username", 
        this->storage->getParameter(Parameter::MQTT_USER),
        Control::noParent,
        Parameter::MQTT_USER
    );
    this->addControl(
        ControlType::Password, 
        "Password", 
        this->storage->getParameter(Parameter::MQTT_PASS),
        Control::noParent,
        Parameter::MQTT_PASS
    );
    String deviceName = this->storage->getParameter(Parameter::MQTT_DEVICE, "changeme");
    auto deviceNameId = this->addControl(
        ControlType::Text, 
        "Device name", 
        deviceName,
        Control::noParent,
        Parameter::MQTT_DEVICE
    );

    this->radiationMeasurementTopicId = this->addControl(
        ControlType::Label, 
        "Radiation measurement topic",
        this->getMeasurementTopic(deviceName), 
        deviceNameId
    );
    ESPUI.setElementStyle(this->radiationMeasurementTopicId, labelStyle);

    ESPUI.addControl(ControlType::Separator, "General");

    auto submitId = this->addControl(
        ControlType::Button,
        "Actions",
        "Save & restart",
        Control::noParent,
        WebAdmin::SUBMIT
    );
    this->addControl(
        ControlType::Button,
        "Reset device",
        "Reset device",
        submitId,
        WebAdmin::RESET
    );
    ESPUI.getControl(submitId)->color = ControlColor::Carrot;

    this->statsId = this->addControl(
        ControlType::Label, 
        "Stats", 
        "", 
        Control::noParent
    );

    this->logId = this->addControl(
        ControlType::Label, 
        "Log", 
        "", 
        Control::noParent
    );

    ESPUI.setElementStyle(this->statsId, labelStyle);
    ESPUI.setElementStyle(this->logId, labelStyle);

    ESPUI.setPanelWide(deviceNameId, true);
    ESPUI.setPanelWide(this->statsId, true);
    ESPUI.setPanelWide(submitId, true);
    ESPUI.setPanelWide(this->logId, true);

    ESPUI.begin("ESP radiation counter");
}

int WebAdmin::addControl(ControlType controlType, const char* label, const String& value, int parent, char* name)
{
    ControlColor color = ControlColor::None;
    if (name != nullptr) {
        auto callback = [](Control* sender, int eventName, void* userData) { admin->handleCallback(sender, eventName, userData); };

        return ESPUI.addControl(
            controlType, 
            label, 
            value, 
            color, 
            parent,
            callback,
            name
        );
    }

    return ESPUI.addControl(
        controlType, 
        label, 
        value, 
        color, 
        parent
    );
}

void WebAdmin::handleCallback(Control* sender, int eventName, void* userData)
{
    if (userData == nullptr) {
        return;
    }
    if (userData == Parameter::MQTT_DEVICE) {
        this->updateTopics(sender->value);
    }
    if (userData == WebAdmin::SUBMIT) {
        if (eventName == B_DOWN) {
            return;
        }
        ESP.restart();
        return;
    }
    if (userData == WebAdmin::RESET) {
        if (eventName == B_DOWN) {
            return;
        }
        this->resetCallback();
        return;
    }

    this->storage->saveParameter((char *) userData, sender->value);
}

void WebAdmin::updateTopics(String value)
{
    auto measurementControl = ESPUI.getControl(this->radiationMeasurementTopicId);
    auto measurementTopic = this->getMeasurementTopic(value);
    measurementControl->value = "Radiation topic: " + measurementTopic; 

    ESPUI.updateControl(measurementControl);

    this->storage->saveParameter(Parameter::MQTT_TOPIC_MEASUREMENT, measurementTopic);
}

String WebAdmin::getMeasurementTopic(String deviceName)
{
    return deviceName + "/stat/radiation";
}

void WebAdmin::run(Stats* stats)
{
    this->updateStats(stats);
    this->updateLog();
}

void WebAdmin::updateStats(Stats* stats)
{
    char buffer[512];

    snprintf(
        buffer,
        sizeof(buffer),
        "Uptime: %s<br>Network: %s<br>IP address: %s<br>Signal: %sdBm<br>Dose: %s",
        stats->uptime.c_str(),
        stats->network.c_str(),
        stats->ipAddress.c_str(),
        stats->wifiSignal.c_str(),
        stats->relativeDistance.c_str()
    );
    String data = buffer;

    auto statsControl = ESPUI.getControl(this->statsId);
    statsControl->value = data;

    ESPUI.updateControl(statsControl);
}

void WebAdmin::updateLog()
{
    String data;

    for (auto &element : this->logger->getBuffer()) {
        data += element + "<br>";
    }

    auto log = ESPUI.getControl(this->logId);
    log->value = data;

    ESPUI.updateControl(log);
}
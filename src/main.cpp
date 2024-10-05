// #include "WifiConnector.h"

#include <Arduino.h>
// #include <ArduinoOTA.h>
// #include <ESP8266WiFi.h>
#include <TaskManagerIO.h>
#include <BasicInterruptAbstraction.h>
// #include <ExecWithParameter.h>

#include "Logger.h"
#include "LedController.h"
// #include "WebAdmin.h"
// #include "MqttClient.h"
#include "Meter.h"
#include "Calculator.h"
#include "Storage.h"
#include "Stats.h"
#include "Display.h"
#include "Aggregator.h"
#include "RadiationClickEvent.h"
#include "ButtonClickEvent.h"

// WiFiClient network;
Display display;

bool mqttConnected = false;
bool wifiConnected = false;

Logger* logger;
// WifiConnector* wifi;
LedController* led;
// WebAdmin* admin;
// MqttClient* mqtt;
Meter* meter;
Calculator* calculator;
Storage* storage;
Stats* stats;
Aggregator* aggregator;
RadiationClickEvent* radiationClickEvent;
ButtonClickEvent* buttonClickEvent;

// void resetCallback() {
//     wifi->resetSettings();
//     // storage->reset();

//     delay(2000);
//     ESP.restart();
// }
BasicArduinoInterruptAbstraction interruptAbstraction;
void onInterrupt(uint8_t interruptNumber) {
    if (interruptNumber == digitalPinToInterrupt(RadiationClickEvent::CNT_PIN)) {
        radiationClickEvent->handleInterrupt();
    }
    if (interruptNumber == 255) {
        buttonClickEvent->handleInterrupt();
    }
}

void setup()
{
    Serial.begin(9600);
    // ArduinoOTA.setPort(8266);
    // ArduinoOTA.begin();

    while (!Serial && !Serial.available()) {}

    logger = new Logger(&Serial, "System");
    meter = new Meter();
    led = new LedController();
    radiationClickEvent = new RadiationClickEvent(meter, led);
    buttonClickEvent = new ButtonClickEvent();

    taskManager.setInterruptCallback(onInterrupt);
    taskManager.addInterrupt(&interruptAbstraction, digitalPinToInterrupt(RadiationClickEvent::CNT_PIN), CHANGE);
    taskManager.addInterrupt(&interruptAbstraction, digitalPinToInterrupt(ButtonClickEvent::BTN_PIN), CHANGE);

    storage = new Storage();
    stats = new Stats(storage);
    calculator = new Calculator(storage);
    aggregator = new Aggregator(storage);
    // wifi = new WifiConnector(logger);
    // mqtt = new MqttClient(storage, logger);
    // admin = new WebAdmin(storage, logger, &resetCallback);
    // meter = new Meter(logger);

    // wifiConnected = wifi->begin();
    // admin->begin();
    // mqtt->begin();

    // taskManager.schedule(repeatMillis(500), [] { wifi->run(); });
    taskManager.schedule(repeatSeconds(1), [] {
        auto result = calculator->calculate(meter->read());
        auto buffer = aggregator->aggregate(result);
        stats->updateStats(
            mqttConnected,
            result,
            buffer
        );
     });

    if (!wifiConnected) {
        // display.displayFirstStep("test");
        // display.displayFirstStep(wifi->getAppName());
        // return;
    }
    // if (storage->isEmpty()) {
    //     display.displaySecondStep(WiFi.localIP().toString().c_str());
    //     return;
    // }
    taskManager.schedule(repeatMicros(100), [] {
        led->run();
    });
    taskManager.schedule(repeatSeconds(1), [] {
        display.run(stats, buttonClickEvent->counter % 2); // number of pages
    });
    // taskManager.schedule(repeatMillis(500), [] { mqttConnected = mqtt->run(); });
}

void loop()
{
    // ArduinoOTA.handle();
    taskManager.runLoop();
}

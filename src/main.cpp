// #include "WifiConnector.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
// #include <WiFi.h>
#include <TaskManagerIO.h>
#include <BasicInterruptAbstraction.h>
// #include <ExecWithParameter.h>

// #include "Logger.h"
#include "LedController.h"
// #include "WebAdmin.h"
// #include "MqttClient.h"
#include "Meter.h"
#include "Calculator.h"
#include "Storage.h"
#include "Stats.h"
#include "Display.h"
#include "Aggregator.h"
#include "ClickEvent.h"

// WiFiClient network;
Display display;

bool mqttConnected = false;
bool wifiConnected = false;

// Logger* logger;
// WifiConnector* wifi;
LedController* led;
// WebAdmin* admin;
// MqttClient* mqtt;
Meter* meter;
Calculator* calculator;
Storage* storage;
Stats* stats;
Aggregator* aggregator;
ClickEvent* clickEvent;

// void resetCallback() {
//     wifi->resetSettings();
//     // storage->reset();

//     delay(2000);
//     ESP.restart();
// }
BasicArduinoInterruptAbstraction interruptAbstraction;
void onInterrupt(uint8_t interruptNumber) {
    clickEvent->handleInterrupt();
}

void setup()
{
    Serial.begin(9600);
    // ArduinoOTA.setPort(8266);
    // ArduinoOTA.begin();

    while (!Serial && !Serial.available()) {}

    meter = new Meter();
    led = new LedController();
    clickEvent = new ClickEvent(meter, led);

    taskManager.setInterruptCallback(onInterrupt);
    taskManager.addInterrupt(&interruptAbstraction, digitalPinToInterrupt(ClickEvent::CNT_PIN), CHANGE);

    storage = new Storage();
    stats = new Stats(storage);
    calculator = new Calculator(storage);
    aggregator = new Aggregator(storage);
    // logger = new Logger(&Serial, "System");
    // wifi = new WifiConnector(logger);
    // mqtt = new MqttClient(storage, logger);
    // admin = new WebAdmin(storage, logger, &resetCallback);
    // meter = new Meter(logger);

    // wifiConnected = wifi->begin();
    // storage->begin();
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
    // taskManager.schedule(repeatSeconds(1), [] {
    //     Serial.printf("CPM: %d, uSv/h: %.2f\n", stats->cpm, stats->dose);
    //  });
    taskManager.schedule(repeatMicros(10), [] {
        led->run();
    });
    taskManager.schedule(repeatSeconds(2), [] {
        display.run(stats);
    });
    // taskManager.schedule(repeatMillis(500), [] { mqttConnected = mqtt->run(); });
}

void loop()
{
    // ArduinoOTA.handle();
    taskManager.runLoop();
}

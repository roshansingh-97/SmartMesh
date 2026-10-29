#include <Arduino.h>
#include "Application/SmartMeshApp.h"
#include "Input/KeypadHandler.h"

SmartMeshApp app;
KeypadHandler keypadHandler;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n--- SMARTMESH OS BOOTING ---");

    // SmartMeshApp owns and initializes Storage, ESP-NOW and BLE.
    keypadHandler.begin();
    app.begin();

    Serial.println("[BLE] BLE is initialized by SmartMeshApp.");
    Serial.println("System initialized.");
}

void loop() {
    // SmartMeshApp is the ONLY consumer of the BLE and ESP-NOW queues.
    app.update();

    char key = keypadHandler.getKey();
    if (key != NO_KEY && key != '\0') {
        app.handleKey(key);
    }

    // Serial Monitor remains available for key/debug input.
    if (Serial.available() > 0) {
        char serialKey = Serial.read();
        if (serialKey != '\r' && serialKey != '\n') {
            app.handleKey(serialKey);
        }
    }

    delay(20);
}

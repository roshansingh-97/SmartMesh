#include <Arduino.h>
#include "Application/SmartMeshApp.h"
#include "Input/KeypadHandler.h"

SmartMeshApp app;
KeypadHandler keypadHandler;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 2000);

    Serial.println(F("[SmartMesh] Booting System (Bluetooth Mode)..."));

    keypadHandler.begin();
    app.begin();

    Serial.println(F("[SmartMesh] Ready."));
}

void loop() {
    char key = keypadHandler.getKey();
    if (key) {
        app.handleKeyPress(key);
    }

    app.update();
    delay(10);
}
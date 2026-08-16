#include <Arduino.h>
#include "Application/SmartMeshApp.h"
#include "input/KeypadHandler.h"

SmartMeshApp app;
KeypadHandler keypadHandler;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n--- SMARTMESH OS BOOTING ---");

    keypadHandler.begin();
    app.begin();

    Serial.println("System initialized.");
    Serial.println("Serial Debugging Active. Type keys (A, B, C, D, 0-9, *, #) to test inputs.");
}

void loop() {
    // 1. Update UI / Application State
    app.update();

    // 2. Read Physical Matrix Keypad
    char key = keypadHandler.getKey();
    if (key != NO_KEY && key != '\0') {
        app.handleKey(key);
    }

    // 3. Read Serial Monitor (USB / Serial Debugging)
    if (Serial.available() > 0) {
        char serialKey = Serial.read();
        
        // Ignore newline and carriage return characters from terminal Enter key
        if (serialKey != '\r' && serialKey != '\n') {
            Serial.print("[SERIAL] Processing key: ");
            Serial.println(serialKey);
            
            app.handleKey(serialKey);
        }
    }

    delay(20);
}
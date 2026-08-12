#include <Arduino.h>
#include "Application/SmartMeshApp.h"

SmartMeshApp app;

void setup() {
    app.begin();
}

void loop() {
    app.update();
}
#ifndef SMARTMESH_APP_H
#define SMARTMESH_APP_H

#include <Arduino.h>
#include "../Config.h"
#include "../Communication/CommunicationManager.h"
#include "../Radio/MeshRadio.h"
#include "../UI/DisplayManager.h"
#include "../Input/KeypadHandler.h"
#include "../Power/BatteryMonitor.h"
#include "../Storage/Storage.h"

class SmartMeshApp {
private:
    MeshRadio radio;
    CommunicationManager comms;
    DisplayManager display;
    KeypadHandler keypad;

    SystemState currentState;
    uint8_t menuSelection;

public:
    SmartMeshApp();

    void begin();
    void update();
};

#endif
#ifndef SMARTMESH_APP_H
#define SMARTMESH_APP_H

#include <Arduino.h>
#include "../Config.h"
#include "Events.h"
#include "EventQueue.h"
#include "../Communication/CommunicationManager.h"
#include "../Radio/MeshRadio.h"
#include "../UI/DisplayManager.h"
#include "../UI/UIManager.h"
#include "../Input/KeypadHandler.h"
#include "../Power/BatteryMonitor.h"
#include "../Storage/Storage.h"

class SmartMeshApp {
private:
    MeshRadio radio;
    CommunicationManager comms;
    DisplayManager display;
    UIManager uiManager;
    KeypadHandler keypad;

    EventQueue eventQueue;

    uint32_t lastBatteryCheckMs;

    void updateSubsystems();
    void processEvents();
    void dispatchEvent(const Event& event);

public:
    SmartMeshApp();

    void begin();
    void update();

    // Event Producer Interface
    void postEvent(const Event& event);
};

#endif
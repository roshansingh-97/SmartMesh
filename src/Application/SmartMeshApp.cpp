#include "SmartMeshApp.h"

SmartMeshApp::SmartMeshApp() : lastBatteryCheckMs(0) {}

void SmartMeshApp::begin() {
    Serial.begin(115200);

    Storage.begin();
    Battery.begin();
    display.begin();
    keypad.begin();
    radio.begin();
    comms.begin(&radio);
    uiManager.begin();

    // Initial Screen Render
    uiManager.draw(display);
}

void SmartMeshApp::postEvent(const Event& event) {
    if (!eventQueue.push(event)) {
        Serial.println("[WARN] EventQueue Full! Dropped event.");
    }
}

void SmartMeshApp::updateSubsystems() {
    keypad.update();
    comms.process();
    uiManager.update();

    // 1. Scan Keypad and Push Key Event
    char key = keypad.scanKeypad();
    if (key != '\0') {
        Event keyEvent(EventType::KEY_PRESSED);
        keyEvent.payload.keyData.key = key;
        postEvent(keyEvent);
    }

    // 2. Battery Monitoring Check
    if (millis() - lastBatteryCheckMs >= 10000) {
        lastBatteryCheckMs = millis();
        uint8_t level = Battery.getPercentage();
        if (level <= 15) {
            Event batEvent(EventType::BATTERY_LOW);
            batEvent.payload.batteryData.percentage = level;
            postEvent(batEvent);
        }
    }
}

void SmartMeshApp::dispatchEvent(const Event& event) {
    switch (event.type) {
        case EventType::KEY_PRESSED:
            // Forward input to active screen via UIManager
            uiManager.handleInput(event.payload.keyData.key);
            // Request UI redraw on user keypress
            uiManager.draw(display);
            break;

        case EventType::MESSAGE_RECEIVED:
            Serial.printf("[APP] New message from node %d\n", event.payload.msgData.senderId);
            // Future Phase 7: Storage.saveMessage(...)
            // Future Phase 8: uiManager.refreshInboxScreen()
            break;

        case EventType::BATTERY_LOW:
            Serial.printf("[WARN] Low Battery Event: %d%%\n", event.payload.batteryData.percentage);
            break;

        default:
            break;
    }
}

void SmartMeshApp::processEvents() {
    Event event;
    while (eventQueue.pop(event)) {
        dispatchEvent(event);
    }
}

void SmartMeshApp::update() {
    updateSubsystems();
    processEvents();
    vTaskDelay(pdMS_TO_TICKS(10));
}
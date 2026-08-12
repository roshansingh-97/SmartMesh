#include "SmartMeshApp.h"

SmartMeshApp::SmartMeshApp() : currentState(STATE_MENU), menuSelection(0) {}

void SmartMeshApp::begin() {
    Serial.begin(115200);

    Storage.begin();
    Battery.begin();
    display.begin();
    keypad.begin();
    radio.begin();

    comms.begin(&radio);

    display.renderMenu(menuSelection);
}

void SmartMeshApp::update() {
    // 1. Tick key input & power manager
    keypad.update();

    // 2. Tick communication manager pipeline
    comms.process();

    // 3. Handle UI key events
    char key = keypad.scanKeypad();

    switch (currentState) {
        case STATE_MENU:
            if (key == '2' || key == 'B') {
                menuSelection = (menuSelection + 1) % 4;
                display.renderMenu(menuSelection);
            } else if (key == '8' || key == 'A') {
                menuSelection = (menuSelection == 0) ? 3 : menuSelection - 1;
                display.renderMenu(menuSelection);
            } else if (key == '#' || key == 'D') {
                if (menuSelection == 0) {
                    keypad.clearMessage();
                    currentState = STATE_COMPOSE;
                    display.renderComposeScreen(keypad.getMessage());
                }
            }
            break;

        case STATE_COMPOSE:
            display.renderComposeScreen(keypad.getMessage());

            if (key == 'B') {
                currentState = STATE_MENU;
                display.renderMenu(menuSelection);
            } else if (key == '#') {
                String msgText = keypad.getMessage();
                if (msgText.length() > 0) {
                    // Send message via CommunicationManager
                    comms.sendMessage(0xFF, msgText.c_str(), MSG_TYPE_TEXT);
                    
                    display.renderStatusScreen("Queued!", "Sending in background...");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    keypad.clearMessage();
                    currentState = STATE_MENU;
                    display.renderMenu(menuSelection);
                }
            }
            break;

        case STATE_INBOX:
            if (key == '*' || key == 'B') {
                currentState = STATE_MENU;
                display.renderMenu(menuSelection);
            }
            break;
    }

    vTaskDelay(pdMS_TO_TICKS(15));
}
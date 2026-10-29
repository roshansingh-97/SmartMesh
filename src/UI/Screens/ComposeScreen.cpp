#include "ComposeScreen.h"
#include "../UIManager.h"
#include "../../Storage/Storage.h"
#include "../../Application/SmartMeshApp.h"

ComposeScreen::ComposeScreen() {}

void ComposeScreen::initialize() {
    textInput.clear();
}

void ComposeScreen::onEnter() {
    textInput.clear();
}

void ComposeScreen::update() {
    textInput.update();
}

void ComposeScreen::draw(DisplayManager& display) {
    display.renderComposeScreen(
        textInput.getModeString(),
        textInput.getBuffer(),
        textInput.getCursorPosition(),
        textInput.getLength(),
        TextInput::MAX_MSG_LEN
    );
}

void ComposeScreen::handleInput(char key, UIManager& uiManager) {
    // Exit back to Home Screen (Key C)
    if (key == 'C') {
        uiManager.navigateTo(uiManager.getHomeScreen());
        return;
    }

    // Send Message (Key D)
    if (key == 'D') {
        textInput.commitPendingChar();

        const char* msgText = textInput.getBuffer();
        if (strlen(msgText) > 0) {
            if (appOwner != nullptr) {
                // A keypad-composed message is sent directly to the paired phone over BLE.
                appOwner->sendComposedMessage(msgText);
            } else {
                // Fallback if this screen is used outside SmartMeshApp.
                Storage.saveSentMessage(0x434C, msgText);
                Serial.println("[KEYPAD->BLE] ERROR: app callback is not configured.");
            }
        }

        textInput.clear();
        uiManager.navigateTo(uiManager.getHomeScreen());
        return;
    }

    // Pass key press to Multi-Tap Engine
    textInput.handleKeyPress(key);
}
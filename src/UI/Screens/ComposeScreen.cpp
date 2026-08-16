#include "ComposeScreen.h"
#include "../UIManager.h"
#include "../../Storage/Storage.h"

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
            // 1. Save to Sent Messages (0xFFFF = Broadcast Target ID)
            Storage.saveSentMessage(0xFFFF, msgText);

            // 2. Loopback Mode Test (Saves to Inbox if enabled)
            UserSettings settings = Storage.getSettings();
            if (settings.loopbackMode) {
                Storage.saveInboxMessage(0x1234, msgText);
            }
        }

        textInput.clear();
        uiManager.navigateTo(uiManager.getHomeScreen());
        return;
    }

    // Pass key press to Multi-Tap Engine
    textInput.handleKeyPress(key);
}
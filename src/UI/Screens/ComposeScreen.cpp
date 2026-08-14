#include "ComposeScreen.h"
#include "../UIManager.h"
#include "../DisplayManager.h"

ComposeScreen::ComposeScreen() {}

void ComposeScreen::initialize() {
    textInput.begin();
}

void ComposeScreen::onEnter() {
    // Clear buffer whenever entering compose screen
    textInput.clear();
}

void ComposeScreen::update() {
    // Drive the 800ms multi-tap commit timer
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

void ComposeScreen::handleInput(char key, UIManager& ui) {
    // 'C' = Back / Cancel
    if (key == 'C') {
        textInput.clear();
        ui.changeScreen(ui.getHomeScreen());
        return;
    }

    // 'D' = Send Message
    if (key == 'D') {
        textInput.commitPendingChar();
        if (textInput.getLength() > 0) {
            // Future Phase 8: Emit MESSAGE_SEND_REQUEST event to CommunicationManager
            Serial.printf("[COMPOSE] Message ready to send: %s\n", textInput.getBuffer());
            textInput.clear();
            ui.changeScreen(ui.getHomeScreen());
        }
        return;
    }

    // Pass all other keys ('0'-'9', '*', '#', 'A', 'B') to TextInput engine
    textInput.handleKeyPress(key);
}
#include "SmartMeshApp.h"

void SmartMeshApp::begin() {
    // Initialize Display Hardware
    displayManager.begin();

    // Initialize Screens
    homeScreen.initialize();
    composeScreen.initialize();
    inboxScreen.initialize();
    sentScreen.initialize();
    settingsScreen.initialize();

    // Register Screens with UIManager
    uiManager.setHomeScreen(&homeScreen);
    uiManager.setComposeScreen(&composeScreen);
    uiManager.setInboxScreen(&inboxScreen);
    uiManager.setSentScreen(&sentScreen);
    uiManager.setSettingsScreen(&settingsScreen);

    // Start UI Engine
    uiManager.begin();
}

void SmartMeshApp::handleKeyPress(char key) {
    if (key != '\0') {
        uiManager.handleInput(key);
    }
}

void SmartMeshApp::update() {
    uiManager.update();

    displayManager.clear();
    uiManager.draw(displayManager);
    displayManager.sendBuffer();
}
#include "HomeScreen.h"
#include "../UIManager.h"
#include "../DisplayManager.h"

HomeScreen::HomeScreen() : currentState(STATE_SPLASH), selectedIndex(0), hasBooted(false) {}

void HomeScreen::initialize() {
    currentState = STATE_SPLASH;
    selectedIndex = 0;
    hasBooted = false;
}

void HomeScreen::onEnter() {
    // If already booted, skip splash and go straight to menu when returning via back button
    if (hasBooted) {
        currentState = STATE_MENU;
    } else {
        currentState = STATE_SPLASH;
    }
    selectedIndex = 0;
}

void HomeScreen::draw(DisplayManager& display) {
    display.clear();
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);

    if (currentState == STATE_SPLASH) {
        u8g2.drawStr(14, 22, "SmartMesh v1.0");
        u8g2.drawStr(10, 48, "Press [D] to Enter");
        display.sendBuffer();
    } else {
        // Pass "Main Menu" as the title
        display.renderMenuList("Main Menu", menuItems, MENU_COUNT, selectedIndex);
    }
}

void HomeScreen::handleInput(char key, UIManager& uiManager) {
    if (currentState == STATE_SPLASH) {
        if (key == 'D' || key == 'd' || key == '#' || key == '\r' || key == '\n') {
            currentState = STATE_MENU;
            hasBooted = true; // Mark splash as passed
        }
        return;
    }

    if (key == 'A' || key == 'a') {
        if (selectedIndex == 0) {
            selectedIndex = MENU_COUNT - 1;
        } else {
            selectedIndex--;
        }
    } 
    else if (key == 'B' || key == 'b') {
        selectedIndex = (selectedIndex + 1) % MENU_COUNT;
    } 
    else if (key == 'D' || key == 'd' || key == '#' || key == '\r' || key == '\n') {
        switch (selectedIndex) {
            case 0:
                if (uiManager.getComposeScreen()) 
                    uiManager.navigateTo(uiManager.getComposeScreen());
                break;
            case 1:
                if (uiManager.getInboxScreen()) 
                    uiManager.navigateTo(uiManager.getInboxScreen());
                break;
            case 2:
                if (uiManager.getSentScreen()) 
                    uiManager.navigateTo(uiManager.getSentScreen());
                break;
            case 3:
                if (uiManager.getContactsScreen()) 
                    uiManager.navigateTo(uiManager.getContactsScreen());
                break;
            case 4:
                if (uiManager.getSettingsScreen()) 
                    uiManager.navigateTo(uiManager.getSettingsScreen());
                break;
            case 5:
                if (uiManager.getAboutScreen()) 
                    uiManager.navigateTo(uiManager.getAboutScreen());
                break;
        }
    }
}
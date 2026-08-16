#include "HomeScreen.h"
#include "../UIManager.h"
#include "../DisplayManager.h"

HomeScreen::HomeScreen() : selectedIndex(0) {}

void HomeScreen::initialize() {
    selectedIndex = 0;
}

void HomeScreen::onEnter() {
    selectedIndex = 0;
}

void HomeScreen::draw(DisplayManager& display) {
    display.renderMenuList("SmartMesh OS", menuItems, MENU_COUNT, selectedIndex);
}

void HomeScreen::handleInput(char key, UIManager& uiManager) {
    // UP Navigation (Key A or 'a')
    if (key == 'A' || key == 'a') {
        if (selectedIndex == 0) {
            selectedIndex = MENU_COUNT - 1;
        } else {
            selectedIndex--;
        }
    } 
    // DOWN Navigation (Key B or 'b')
    else if (key == 'B' || key == 'b') {
        selectedIndex = (selectedIndex + 1) % MENU_COUNT;
    } 
    // SELECT / ENTER (Key D, 'd', '#', or Enter)
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
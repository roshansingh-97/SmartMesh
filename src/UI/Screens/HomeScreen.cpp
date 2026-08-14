#include "HomeScreen.h"
#include "../UIManager.h"
#include "../DisplayManager.h"

HomeScreen::HomeScreen() : selectedIndex(0) {}

void HomeScreen::initialize() {
    selectedIndex = 0;
}

void HomeScreen::onEnter() {
    // Reset selection focus when returning to Home
    selectedIndex = 0;
}

void HomeScreen::draw(DisplayManager& display) {
    display.renderMenuList("SmartMesh OS", menuItems, MENU_COUNT, selectedIndex);
}

void HomeScreen::handleInput(char key, UIManager& ui) {
    // Directional scrolling (2/B for Down, 8/A for Up)
    if (key == '2' || key == 'B') {
        selectedIndex = (selectedIndex + 1) % MENU_COUNT;
    } else if (key == '8' || key == 'A') {
        selectedIndex = (selectedIndex == 0) ? (MENU_COUNT - 1) : (selectedIndex - 1);
    } 
    // Quick number keys or Select key (#/D)
    else if (key == '1' || (selectedIndex == 0 && (key == '#' || key == 'D'))) {
        ui.changeScreen(ui.getComposeScreen());
    } else if (key == '2' || (selectedIndex == 1 && (key == '#' || key == 'D'))) {
        ui.changeScreen(ui.getInboxScreen());
    } else if (key == '3' || (selectedIndex == 2 && (key == '#' || key == 'D'))) {
        ui.changeScreen(ui.getSettingsScreen());
    }
}
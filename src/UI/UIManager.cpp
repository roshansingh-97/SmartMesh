#include "UIManager.h"
#include "Screens/Screen.h"

void UIManager::begin() {
    display.begin();
}

void UIManager::update() {
    if (currentScreen) {
        currentScreen->update();
        currentScreen->draw(display);
    }
}

void UIManager::navigateTo(Screen* screen) {
    if (screen) {
        currentScreen = screen;
        currentScreen->onEnter();
    }
}

void UIManager::handleInput(char key) {
    if (currentScreen) {
        currentScreen->handleInput(key, *this);
    }
}
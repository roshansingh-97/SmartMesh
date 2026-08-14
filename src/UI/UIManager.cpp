#include "UIManager.h"
#include "Screen.h"

UIManager::UIManager() {}

void UIManager::begin() {
    if (homeScreen) {
        currentScreen = homeScreen;
        currentScreen->onEnter();
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

void UIManager::update() {
    // UI update loop
}

void UIManager::draw(DisplayManager& display) {
    if (currentScreen) {
        currentScreen->draw(display);
    }
}
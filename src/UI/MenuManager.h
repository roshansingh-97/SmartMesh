#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <Arduino.h>
#include "DisplayManager.h"

// Screen States across the Terminal OS
enum ScreenState {
    SCREEN_MAIN_MENU,
    SCREEN_COMPOSE,
    SCREEN_INBOX,
    SCREEN_SENT,
    SCREEN_CONTACTS,
    SCREEN_SETTINGS,
    SCREEN_ABOUT
};

struct MenuItem {
    const char* title;
    ScreenState targetScreen;
};

class MenuManager {
private:
    ScreenState currentScreen;
    uint8_t selectedIndex;
    uint8_t totalItems;

    static const uint8_t MAX_MENU_ITEMS = 6;
    MenuItem mainMenuItems[MAX_MENU_ITEMS];

    void navigateNext();
    void navigatePrevious();

public:
    MenuManager();

    void begin();
    
    // Event Handler for Navigation Keys
    void handleKeyPress(char key);

    // Render hook passed to DisplayManager
    void render(DisplayManager& display);

    // State Accessors
    ScreenState getCurrentScreen() const { return currentScreen; }
    void setScreen(ScreenState screen);
};

#endif
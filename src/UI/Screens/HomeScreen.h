#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <Arduino.h>
#include "Screen.h"

class HomeScreen : public Screen {
private:
    enum HomeState {
        STATE_SPLASH,
        STATE_MENU
    };
    HomeState currentState;
    uint8_t selectedIndex;
    bool hasBooted; // Tracks if splash screen was already passed
    static const uint8_t MENU_COUNT = 6;
    const char* menuItems[MENU_COUNT] = {
        "Compose",
        "Inbox",
        "Sent",
        "Contacts",
        "Settings",
        "About"
    };

public:
    HomeScreen();
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& uiManager) override;
};

#endif // HOME_SCREEN_H
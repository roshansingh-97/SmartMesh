#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "../Screen.h"

class HomeScreen : public Screen {
private:
    uint8_t selectedIndex;
    static const uint8_t MENU_COUNT = 3;
    const char* menuItems[MENU_COUNT] = {
        "1. Compose",
        "2. Inbox",
        "3. Settings"
    };

public:
    HomeScreen();

    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& ui) override;
};

#endif
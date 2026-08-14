#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "DisplayManager.h"

class Screen;

class UIManager {
private:
    Screen* currentScreen = nullptr;
    Screen* homeScreen = nullptr;
    Screen* composeScreen = nullptr;
    Screen* inboxScreen = nullptr;
    Screen* sentScreen = nullptr;
    Screen* settingsScreen = nullptr;

public:
    UIManager();

    void begin();
    void handleInput(char key);
    void update();
    void draw(DisplayManager& display);

    // Screen Navigation
    void navigateTo(Screen* screen);
    void changeScreen(Screen* screen) { navigateTo(screen); }

    // Screen Getters
    Screen* getHomeScreen() const { return homeScreen; }
    Screen* getComposeScreen() const { return composeScreen; }
    Screen* getInboxScreen() const { return inboxScreen; }
    Screen* getSentScreen() const { return sentScreen; }
    Screen* getSettingsScreen() const { return settingsScreen; }

    // Screen Setters
    void setHomeScreen(Screen* s) { homeScreen = s; if (!currentScreen) currentScreen = s; }
    void setComposeScreen(Screen* s) { composeScreen = s; }
    void setInboxScreen(Screen* s) { inboxScreen = s; }
    void setSentScreen(Screen* s) { sentScreen = s; }
    void setSettingsScreen(Screen* s) { settingsScreen = s; }
};

#endif // UI_MANAGER_H
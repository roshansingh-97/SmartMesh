#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include "DisplayManager.h"

// Forward declaration
class Screen;

class UIManager {
private:
    DisplayManager display;
    Screen* currentScreen = nullptr;

    Screen* homeScreen = nullptr;
    Screen* composeScreen = nullptr;
    Screen* inboxScreen = nullptr;
    Screen* sentScreen = nullptr;
    Screen* settingsScreen = nullptr;
    Screen* aboutScreen = nullptr;
    Screen* contactsScreen = nullptr;

public:
    void begin();
    void update();
    void navigateTo(Screen* screen);
    void handleInput(char key);

    // Getters & Setters
    void setHomeScreen(Screen* s) { homeScreen = s; }
    Screen* getHomeScreen() { return homeScreen; }

    void setComposeScreen(Screen* s) { composeScreen = s; }
    Screen* getComposeScreen() { return composeScreen; }

    void setInboxScreen(Screen* s) { inboxScreen = s; }
    Screen* getInboxScreen() { return inboxScreen; }

    void setSentScreen(Screen* s) { sentScreen = s; }
    Screen* getSentScreen() { return sentScreen; }

    void setSettingsScreen(Screen* s) { settingsScreen = s; }
    Screen* getSettingsScreen() { return settingsScreen; }

    void setAboutScreen(Screen* s) { aboutScreen = s; }
    Screen* getAboutScreen() { return aboutScreen; }

    void setContactsScreen(Screen* s) { contactsScreen = s; }
    Screen* getContactsScreen() { return contactsScreen; }

    DisplayManager& getDisplay() { return display; }
};

#endif // UI_MANAGER_H
#include "MenuManager.h"

MenuManager::MenuManager() 
    : currentScreen(SCREEN_MAIN_MENU), selectedIndex(0), totalItems(6) {
    
    mainMenuItems[0] = {"1. Compose",  SCREEN_COMPOSE};
    mainMenuItems[1] = {"2. Inbox",    SCREEN_INBOX};
    mainMenuItems[2] = {"3. Sent",     SCREEN_SENT};
    mainMenuItems[3] = {"4. Contacts", SCREEN_CONTACTS};
    mainMenuItems[4] = {"5. Settings", SCREEN_SETTINGS};
    mainMenuItems[5] = {"6. About",    SCREEN_ABOUT};
}

void MenuManager::begin() {
    currentScreen = SCREEN_MAIN_MENU;
    selectedIndex = 0;
}

void MenuManager::navigateNext() {
    selectedIndex = (selectedIndex + 1) % totalItems;
}

void MenuManager::navigatePrevious() {
    selectedIndex = (selectedIndex == 0) ? (totalItems - 1) : (selectedIndex - 1);
}

void MenuManager::setScreen(ScreenState screen) {
    currentScreen = screen;
    selectedIndex = 0; // Reset menu selection when returning to menu
}

void MenuManager::handleKeyPress(char key) {
    if (currentScreen == SCREEN_MAIN_MENU) {
        // Navigation Controls
        if (key == '2' || key == 'B') {
            navigateNext();
        } else if (key == '8' || key == 'A') {
            navigatePrevious();
        } else if (key == '#' || key == 'D') {
            // Select current option
            currentScreen = mainMenuItems[selectedIndex].targetScreen;
        }
    } else {
        // Global Back Button ('*' returns to Main Menu from any active screen)
        if (key == '*') {
            setScreen(SCREEN_MAIN_MENU);
        }
    }
}

void MenuManager::render(DisplayManager& display) {
    switch (currentScreen) {
        case SCREEN_MAIN_MENU: {
            // Extract string labels for DisplayManager
            const char* labels[MAX_MENU_ITEMS];
            for (uint8_t i = 0; i < totalItems; i++) {
                labels[i] = mainMenuItems[i].title;
            }
            // Delegate pixel drawing to DisplayManager
            display.renderMenuList("SmartMesh OS", labels, totalItems, selectedIndex);
            break;
        }
        case SCREEN_COMPOSE:
            display.renderStatusScreen("Compose", "Press * to Back");
            break;
        case SCREEN_INBOX:
            display.renderStatusScreen("Inbox", "No New Messages");
            break;
        case SCREEN_SENT:
            display.renderStatusScreen("Sent Items", "No Sent Messages");
            break;
        case SCREEN_CONTACTS:
            display.renderStatusScreen("Contacts", "Node List Empty");
            break;
        case SCREEN_SETTINGS:
            display.renderStatusScreen("Settings", "System Config");
            break;
        case SCREEN_ABOUT:
            display.renderStatusScreen("SmartMesh v1.0", "Node ID: 0x01");
            break;
    }
}
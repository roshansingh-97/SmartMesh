#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "../Screen.h"
#include "../DisplayManager.h"
#include "../../Storage/Storage.h"

class UIManager;

class SettingsScreen : public Screen {
private:
    int selectedOption = 0;
    static const int TOTAL_OPTIONS = 3;
    UserSettings settings;

public:
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager &display) override;
    void handleInput(char key, UIManager &uiManager) override;
};

#endif
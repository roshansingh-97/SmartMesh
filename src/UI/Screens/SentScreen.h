#ifndef SENT_SCREEN_H
#define SENT_SCREEN_H

#include "Screen.h" // Fixed: relative include path within same directory
#include "../DisplayManager.h"
#include "../../Storage/Storage.h"

class UIManager;

class SentScreen : public Screen {
private:
    int selectedIndex = 0;
    int scrollOffset = 0;
    bool inDetailView = false;
    static const int VISIBLE_ROWS = 3;

public:
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager &display) override;
    void handleInput(char key, UIManager &uiManager) override;
};

#endif
#ifndef INBOX_SCREEN_H
#define INBOX_SCREEN_H

#include "Screen.h"
#include "../DisplayManager.h"
#include "../../Storage/Storage.h"

class UIManager;

class InboxScreen : public Screen {
private:
    int selectedIndex = 0;
    int scrollOffset = 0;
    bool inDetailView = false;
    static const int VISIBLE_ROWS = 3;

public:
    InboxScreen();
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager &display) override;
    void handleInput(char key, UIManager &uiManager) override;
};

#endif // INBOX_SCREEN_H
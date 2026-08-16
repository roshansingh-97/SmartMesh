#ifndef CONTACTS_SCREEN_H
#define CONTACTS_SCREEN_H

#include "Screen.h"
#include "../DisplayManager.h"
#include "../../Storage/Storage.h"

class UIManager;

class ContactsScreen : public Screen {
private:
    uint8_t selectedIndex = 0;
    static const uint8_t CONTACT_COUNT = 3;
    uint16_t contactNodes[3] = {0x1234, 0x5678, 0xFFFF};

public:
    ContactsScreen();
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& uiManager) override;
};

#endif // CONTACTS_SCREEN_H
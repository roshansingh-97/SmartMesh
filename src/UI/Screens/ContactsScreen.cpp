#include "ContactsScreen.h"
#include "../UIManager.h"

ContactsScreen::ContactsScreen() : selectedIndex(0) {}

void ContactsScreen::initialize() {
    selectedIndex = 0;
}

void ContactsScreen::onEnter() {
    selectedIndex = 0;
}

void ContactsScreen::draw(DisplayManager& display) {
    display.clear();
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    u8g2.drawStr(0, 10, "--- CONTACTS ---");

    for (uint8_t i = 0; i < CONTACT_COUNT; i++) {
        char contactName[16];
        Storage.getContactName(contactNodes[i], contactName, sizeof(contactName));

        char buf[32];
        snprintf(buf, sizeof(buf), "%c%04X: %s", 
                 (i == selectedIndex) ? '>' : ' ', 
                 contactNodes[i], 
                 contactName);

        u8g2.drawStr(0, 24 + (i * 12), buf);
    }

    // Bottom Footer
    u8g2.drawStr(0, 62, "[A/B]Nav [C/*] Back");
    display.sendBuffer();
}

void ContactsScreen::handleInput(char key, UIManager& uiManager) {
    if (key == 'C' || key == '*') {
        uiManager.navigateTo(uiManager.getHomeScreen());
        return;
    }

    if (key == 'A') {
        if (selectedIndex > 0) selectedIndex--;
    } else if (key == 'B') {
        if (selectedIndex < CONTACT_COUNT - 1) selectedIndex++;
    }
}
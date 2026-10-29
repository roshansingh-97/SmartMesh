#include "SettingsScreen.h"
#include "../UIManager.h"

void SettingsScreen::initialize() {
    selectedOption = 0;
    settings = Storage.getSettings();
}

void SettingsScreen::onEnter() {
    selectedOption = 0;
    settings = Storage.getSettings();
}

void SettingsScreen::handleInput(char key, UIManager &uiManager) {
    if (key == 'C') { // Single button check
        uiManager.navigateTo(uiManager.getHomeScreen());
        return;
    }

    if (key == 'A') {
        if (selectedOption > 0) selectedOption--;
    }
    else if (key == 'B') {
        if (selectedOption < TOTAL_OPTIONS - 1) selectedOption++;
    }
    else if (key == 'D' || key == '#') {
        switch (selectedOption) {
            case 0:
                settings.silentMode = !settings.silentMode;
                Storage.saveSettings(settings);
                break;

            case 1:
                settings.loopbackMode = !settings.loopbackMode;
                Storage.saveSettings(settings);
                break;

            case 2:
                Storage.clearInbox();
                Storage.clearSent();
                break;
        }
    }
}

void SettingsScreen::draw(DisplayManager &display) {
    display.clear();
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "--- SETTINGS ---");

    char buf[32];

    snprintf(buf, sizeof(buf), "%cSilent: %s",
             (selectedOption == 0) ? '>' : ' ',
             settings.silentMode ? "ON" : "OFF");
    u8g2.drawStr(0, 24, buf);

    snprintf(buf, sizeof(buf), "%cLoopback: %s",
             (selectedOption == 1) ? '>' : ' ',
             settings.loopbackMode ? "ON" : "OFF");
    u8g2.drawStr(0, 36, buf);

    snprintf(buf, sizeof(buf), "%cClear All Messages",
             (selectedOption == 2) ? '>' : ' ');
    u8g2.drawStr(0, 48, buf);

    u8g2.drawStr(0, 62, "D: Toggle  C: Back");
    display.sendBuffer();
}
#include "AboutScreen.h"
#include "../UIManager.h"

AboutScreen::AboutScreen() {}

void AboutScreen::initialize() {}
void AboutScreen::onEnter() {}

void AboutScreen::draw(DisplayManager& display) {
    display.clear();
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    u8g2.drawStr(0, 10, "--- ABOUT DEVICE ---");
    u8g2.drawStr(0, 24, "OS: SmartMesh v1.0");
    u8g2.drawStr(0, 36, "Node ID: 0x1234");
    u8g2.drawStr(0, 48, "Status: Online");
    
    // Bottom Footer
    u8g2.drawStr(0, 62, "[C/*] Back");
    display.sendBuffer();
}

void AboutScreen::handleInput(char key, UIManager& uiManager) {
    if (key == 'C' || key == '*') {
        uiManager.navigateTo(uiManager.getHomeScreen());
    }
}
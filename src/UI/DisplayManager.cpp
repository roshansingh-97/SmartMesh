#include "DisplayManager.h"

DisplayManager::DisplayManager() 
    : u8g2(U8G2_R0, OLED_SCL, OLED_SDA, U8X8_PIN_NONE) {
}

void DisplayManager::begin() {
    u8g2.begin();
}

void DisplayManager::clear() {
    u8g2.clearBuffer();
}

void DisplayManager::sendBuffer() {
    u8g2.sendBuffer();
}

void DisplayManager::drawHeader(const char* title, uint8_t batteryLevel) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, title);
}

void DisplayManager::renderMenu(uint8_t selectedIndex) {}

void DisplayManager::renderMenuList(const char* title, const char** items, uint8_t count, uint8_t selectedIndex) {}

void DisplayManager::renderComposeScreen(const String& text) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Compose:");
    u8g2.drawStr(0, 30, text.c_str());
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    char titleBuf[32];
    snprintf(titleBuf, sizeof(titleBuf), "To: %s", recipient);
    u8g2.drawStr(0, 10, titleBuf);
    
    u8g2.drawStr(0, 30, text);
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex, uint8_t activeField, const uint8_t& mode) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);

    char titleBuf[32];
    snprintf(titleBuf, sizeof(titleBuf), "To: %s", recipient);
    u8g2.drawStr(0, 10, titleBuf);

    u8g2.drawStr(0, 30, text);
    u8g2.sendBuffer();
}

void DisplayManager::renderInboxScreen(uint8_t count, const char* summary) {}

void DisplayManager::renderStatusScreen(const char* status, const char* detail) {}
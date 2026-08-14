#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "../Config.h"

class DisplayManager {
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;

public:
    DisplayManager();

    void begin();
    void clear();
    void sendBuffer();

    U8G2 &getU8g2() { return u8g2; }

    // Screen Render Methods
    void drawHeader(const char* title, uint8_t batteryLevel = 100);
    void renderMenu(uint8_t selectedIndex);
    void renderMenuList(const char* title, const char** items, uint8_t count, uint8_t selectedIndex);

    // Compose screen overloads
    void renderComposeScreen(const String& text);
    void renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex);
    void renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex, uint8_t activeField, const uint8_t& mode);

    void renderInboxScreen(uint8_t count, const char* summary);
    void renderStatusScreen(const char* status, const char* detail);
};

#endif // DISPLAY_MANAGER_H
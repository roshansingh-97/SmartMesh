#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "Config.h"

class DisplayManager {
private:
    // Swapped from SSD1306 to SH1106 to fix right-edge memory wrap/line artifacts
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

public:
    DisplayManager();
    void begin();
    void clear();
    void sendBuffer();
    
    U8G2& getU8g2() { return u8g2; }

    void drawHeader(const char* title, uint8_t batteryLevel);
    void renderMenuList(const char* title, const char** items, uint8_t count, uint8_t selectedIndex);
    
    void renderComposeScreen(const String& text);
    void renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex);
    void renderComposeScreen(const char* mode, const char* buffer, uint8_t cursor, uint8_t len, uint8_t maxLen);
    
    void renderInboxScreen(uint8_t count, const char* summary);
    void renderStatusScreen(const char* status, const char* detail);
};

#endif // DISPLAY_MANAGER_H
#pragma once
#include <U8g2lib.h>
#include "Config.h"

class DisplayManager {
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

public:
    DisplayManager();
    void begin();
    void drawHeader(const char* title, uint8_t batPct = 100);
    void renderMenu(uint8_t selectedIndex);
    void renderComposeScreen(const String& typedText);
    void renderInboxScreen(uint8_t senderID, const char* message);
    void renderStatusScreen(const char* line1, const char* line2);
};
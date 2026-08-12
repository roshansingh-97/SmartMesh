#include "DisplayManager.h"

DisplayManager::DisplayManager() 
    : u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA) {}

void DisplayManager::begin() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
}

void DisplayManager::drawHeader(const char* title, uint8_t batPct) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, title);
    
    // Draw Battery Percentage
    char batStr[10];
    snprintf(batStr, sizeof(batStr), "%d%%", batPct);
    u8g2.drawStr(100, 10, batStr);
    u8g2.drawHLine(0, 12, 128);
}

void DisplayManager::renderMenu(uint8_t selectedIndex) {
    u8g2.clearBuffer();
    drawHeader("SmartMesh", 95);

    const char* items[] = {"1. Compose", "2. Inbox", "3. Sent", "4. Contacts", "5. Settings"};
    
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t idx = selectedIndex + i;
        if (idx < 5) {
            if (i == 0) u8g2.drawStr(0, 26 + (i * 12), "> ");
            u8g2.drawStr(12, 26 + (i * 12), items[idx]);
        }
    }
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const String& typedText) {
    u8g2.clearBuffer();
    drawHeader("Compose", 95);
    
    u8g2.drawStr(0, 25, "Text:");
    u8g2.drawStr(0, 40, typedText.c_str());
    
    u8g2.drawStr(0, 62, "[#:Send  *:Back]");
    u8g2.sendBuffer();
}
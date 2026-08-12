#include "DisplayManager.h"
#include "../Power/BatteryMonitor.h"

DisplayManager::DisplayManager() 
    : u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA) {}

void DisplayManager::begin() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
}

void DisplayManager::drawHeader(const char* title, uint8_t batPct) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, title);
    
    char batStr[10];
    snprintf(batStr, sizeof(batStr), "%d%%", batPct);
    u8g2.drawStr(100, 10, batStr);
    u8g2.drawHLine(0, 12, 128);
}

void DisplayManager::renderMenu(uint8_t selectedIndex) {
    u8g2.clearBuffer();
    drawHeader("SmartMesh", Battery.getPercentage());

    const char* items[] = {"1. Compose", "2. Inbox", "3. Sent", "4. Contacts", "5. Settings"};

    for (uint8_t i = 0; i < 4; i++) {
        if (i < 5) {
            if (i == selectedIndex) {
                u8g2.drawStr(0, 26 + (i * 12), ">");
            }
            u8g2.drawStr(12, 26 + (i * 12), items[i]);
        }
    }
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const String& typedText) {
    u8g2.clearBuffer();
    drawHeader("Compose", Battery.getPercentage());
    
    const uint8_t maxCharsPerLine = 20;
    uint8_t lineY = 24;

    if (typedText.length() == 0) {
        u8g2.drawStr(0, lineY, "Type message...");
    } else {
        for (uint16_t i = 0; i < typedText.length(); i += maxCharsPerLine) {
            String line = typedText.substring(i, i + maxCharsPerLine);
            u8g2.drawStr(0, lineY, line.c_str());
            lineY += 11;
            if (lineY > 50) break;
        }
    }
    
    u8g2.drawStr(0, 62, "[#:Send *:Del B:Exit]");
    u8g2.sendBuffer();
}

void DisplayManager::renderInboxScreen(uint8_t senderID, const char* message) {
    u8g2.clearBuffer();
    
    char title[16];
    snprintf(title, sizeof(title), "From: Node %d", senderID);
    drawHeader(title, Battery.getPercentage());

    const uint8_t maxCharsPerLine = 20;
    uint8_t lineY = 24;
    String text = String(message);

    for (uint16_t i = 0; i < text.length(); i += maxCharsPerLine) {
        String line = text.substring(i, i + maxCharsPerLine);
        u8g2.drawStr(0, lineY, line.c_str());
        lineY += 11;
        if (lineY > 50) break;
    }

    u8g2.drawStr(0, 62, "[*:Back]");
    u8g2.sendBuffer();
}

void DisplayManager::renderStatusScreen(const char* line1, const char* line2) {
    u8g2.clearBuffer();
    drawHeader("Status", Battery.getPercentage());
    u8g2.drawStr(0, 30, line1);
    u8g2.drawStr(0, 45, line2);
    u8g2.sendBuffer();
}
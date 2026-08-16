#include "DisplayManager.h"

// Hardware I2C constructor
DisplayManager::DisplayManager() 
    : u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA) {
}

void DisplayManager::begin() {
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "SmartMesh Booting...");
    u8g2.sendBuffer();
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

// --------------------------------------------------
// Generic Menu / List Screen (With Auto-Scrolling)
// --------------------------------------------------
void DisplayManager::renderMenuList(const char* title, const char** items, uint8_t count, uint8_t selectedIndex) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, title);
    
    // Calculate scrolling window (shows 3 items at a time)[cite: 9]
    uint8_t topIndex = 0;
    if (selectedIndex >= 3) {
        topIndex = selectedIndex - 2;
    }

    // Render 3 visible items from current scroll position[cite: 9]
    for (uint8_t i = 0; i < 3 && (topIndex + i) < count; i++) {
        uint8_t itemIdx = topIndex + i;
        char buf[32];
        snprintf(buf, sizeof(buf), "%s %s", (itemIdx == selectedIndex) ? ">" : " ", items[itemIdx]);
        u8g2.drawStr(0, 24 + (i * 12), buf);
    }
    
    u8g2.drawStr(0, 62, "[A/B]Nav [D]Sel [C]Exit");
    u8g2.sendBuffer();
}

// --------------------------------------------------
// Compose Screen Overloads[cite: 9]
// --------------------------------------------------
void DisplayManager::renderComposeScreen(const String& text) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Compose:");
    u8g2.drawStr(0, 28, text.c_str());
    
    u8g2.drawStr(0, 62, "[*]Mode [#]Del [D]Send");
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const char* recipient, const char* text, uint8_t cursorIndex) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    
    char titleBuf[32];
    snprintf(titleBuf, sizeof(titleBuf), "To: %s", recipient);
    u8g2.drawStr(0, 10, titleBuf);
    
    u8g2.drawStr(0, 28, text);
    
    u8g2.drawStr(0, 62, "[*]Mode [#]Del [D]Send");
    u8g2.sendBuffer();
}

void DisplayManager::renderComposeScreen(const char* mode, const char* buffer, uint8_t cursor, uint8_t len, uint8_t maxLen) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);

    char headerBuf[32];
    snprintf(headerBuf, sizeof(headerBuf), "COMPOSE [%s] %d/%d", mode, len, maxLen);
    u8g2.drawStr(0, 10, headerBuf);

    u8g2.drawStr(0, 28, buffer);

    int cursorX = cursor * 6;
    u8g2.drawHLine(cursorX, 30, 6);

    u8g2.drawStr(0, 62, "[*]Mode [#]Del [D]Send");
    u8g2.sendBuffer();
}

// --------------------------------------------------
// Inbox Screen[cite: 9]
// --------------------------------------------------
void DisplayManager::renderInboxScreen(uint8_t count, const char* summary) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "--- INBOX ---");
    u8g2.drawStr(0, 28, summary);
    
    u8g2.drawStr(0, 62, "[D]View  [C]Back");
    u8g2.sendBuffer();
}

// --------------------------------------------------
// Status Screen[cite: 9]
// --------------------------------------------------
void DisplayManager::renderStatusScreen(const char* status, const char* detail) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, status);
    u8g2.drawStr(0, 28, detail);
    
    u8g2.drawStr(0, 62, "[C]Back");
    u8g2.sendBuffer();
}
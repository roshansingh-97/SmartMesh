#include "SentScreen.h"
#include "../UIManager.h"

void SentScreen::initialize() {
    selectedIndex = 0;
    scrollOffset = 0;
    inDetailView = false;
}

void SentScreen::onEnter() {
    selectedIndex = 0;
    scrollOffset = 0;
    inDetailView = false;
}

void SentScreen::handleInput(char key, UIManager &uiManager) {
    int count = Storage.getSentCount();

    if (key == 'C' || key == '*') {
        if (inDetailView) {
            inDetailView = false;
        } else {
            // Return to previous screen or home screen instance
        }
        return;
    }

    if (inDetailView) return;
    if (count == 0) return;

    if (key == 'A' || key == '2') {
        if (selectedIndex > 0) {
            selectedIndex--;
            if (selectedIndex < scrollOffset) {
                scrollOffset = selectedIndex;
            }
        }
    }
    else if (key == 'B' || key == '8') {
        if (selectedIndex < count - 1) {
            selectedIndex++;
            if (selectedIndex >= scrollOffset + VISIBLE_ROWS) {
                scrollOffset = selectedIndex - VISIBLE_ROWS + 1;
            }
        }
    }
    else if (key == 'D' || key == '5' || key == '#') {
        inDetailView = true;
    }
}

void SentScreen::draw(DisplayManager &display) {
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);
    int count = Storage.getSentCount();

    if (inDetailView) {
        StoredMessage msg;
        if (Storage.getSentMessage(selectedIndex, msg)) {
            u8g2.drawStr(0, 10, "--- SENT DETAIL ---");

            char toBuf[32];
            snprintf(toBuf, sizeof(toBuf), "To: 0x%04X", (unsigned int)msg.senderOrTargetId);
            u8g2.drawStr(0, 22, toBuf);

            u8g2.drawStr(0, 36, msg.text);
            u8g2.drawStr(0, 62, "[C] Back");
        }
        return;
    }

    u8g2.drawStr(0, 10, "--- SENT ---");

    if (count == 0) {
        u8g2.drawStr(10, 35, "No Sent Messages");
        u8g2.drawStr(0, 62, "[C] Back");
        return;
    }

    int y = 24;
    for (int i = scrollOffset; i < count && (i - scrollOffset) < VISIBLE_ROWS; i++) {
        StoredMessage msg;
        if (Storage.getSentMessage(i, msg)) {
            char rowBuf[32];
            snprintf(rowBuf, sizeof(rowBuf), "%cTo %04X: %.8s",
                     (i == selectedIndex) ? '>' : ' ',
                     (unsigned int)msg.senderOrTargetId,
                     msg.text);

            u8g2.drawStr(0, y, rowBuf);
            y += 12;
        }
    }

    if (scrollOffset > 0) u8g2.drawStr(120, 22, "^");
    if (scrollOffset + VISIBLE_ROWS < count) u8g2.drawStr(120, 50, "v");

    u8g2.drawStr(0, 62, "[D] View  [C] Back");
}
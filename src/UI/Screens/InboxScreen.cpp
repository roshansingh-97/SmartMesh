#include "InboxScreen.h"
#include "../UIManager.h"

InboxScreen::InboxScreen() {}

void InboxScreen::initialize() {
    selectedIndex = 0;
    scrollOffset = 0;
    inDetailView = false;
}

void InboxScreen::onEnter() {
    selectedIndex = 0;
    scrollOffset = 0;
    inDetailView = false;
}

void InboxScreen::handleInput(char key, UIManager &uiManager) {
    int count = Storage.getInboxCount();

    // BACK ACTION: Exit detail view OR return to Home Screen
    if (key == 'C' || key == '*') {
        if (inDetailView) {
            inDetailView = false;
        } else {
            uiManager.navigateTo(uiManager.getHomeScreen());
        }
        return;
    }

    if (inDetailView) return;
    if (count == 0) return;

    // UP Navigation (Key A)
    if (key == 'A') {
        if (selectedIndex > 0) {
            selectedIndex--;
            if (selectedIndex < scrollOffset) {
                scrollOffset = selectedIndex;
            }
        }
    }
    // DOWN Navigation (Key B)
    else if (key == 'B') {
        if (selectedIndex < count - 1) {
            selectedIndex++;
            if (selectedIndex >= scrollOffset + VISIBLE_ROWS) {
                scrollOffset = selectedIndex - VISIBLE_ROWS + 1;
            }
        }
    }
    // VIEW MESSAGE (Key D or #)
    else if (key == 'D' || key == '#') {
        inDetailView = true;
        Storage.markInboxRead(selectedIndex);
    }
}

void InboxScreen::draw(DisplayManager &display) {
    display.clear();
    U8G2 &u8g2 = display.getU8g2();
    u8g2.setFont(u8g2_font_6x10_tf);
    int count = Storage.getInboxCount();

    // 1. DETAIL VIEW
    if (inDetailView) {
        StoredMessage msg;
        if (Storage.getInboxMessage(selectedIndex, msg)) {
            u8g2.drawStr(0, 10, "--- INBOX DETAIL ---");

            char fromBuf[32];
            snprintf(fromBuf, sizeof(fromBuf), "From: 0x%04X", (unsigned int)msg.senderOrTargetId);
            u8g2.drawStr(0, 22, fromBuf);

            u8g2.drawStr(0, 36, msg.text);
            u8g2.drawStr(0, 62, "[C/*] Back");
        }
        display.sendBuffer();
        return;
    }

    // 2. LIST VIEW
    u8g2.drawStr(0, 10, "--- INBOX ---");

    if (count == 0) {
        u8g2.drawStr(10, 35, "No Messages");
        u8g2.drawStr(0, 62, "[C/*] Back");
        display.sendBuffer();
        return;
    }

    int y = 24;
    for (int i = scrollOffset; i < count && (i - scrollOffset) < VISIBLE_ROWS; i++) {
        StoredMessage msg;
        if (Storage.getInboxMessage(i, msg)) {
            char rowBuf[32];
            // Adds '*' for unread messages
            snprintf(rowBuf, sizeof(rowBuf), "%c%s%04X: %.8s",
                     (i == selectedIndex) ? '>' : ' ',
                     msg.isRead ? " " : "*",
                     (unsigned int)msg.senderOrTargetId,
                     msg.text);

            u8g2.drawStr(0, y, rowBuf);
            y += 12;
        }
    }

    // Scroll indicators
    if (scrollOffset > 0) u8g2.drawStr(120, 22, "^");
    if (scrollOffset + VISIBLE_ROWS < count) u8g2.drawStr(120, 50, "v");

    u8g2.drawStr(0, 62, "[D] View  [C/*] Back");
    display.sendBuffer();
}
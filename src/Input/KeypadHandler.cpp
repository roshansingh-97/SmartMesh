#include "KeypadHandler.h"

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4};
byte colPins[COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

const char* keyMap[] = {
    " 0",      // Key 0
    ".,?!1",   // Key 1
    "ABC2",    // Key 2
    "DEF3",    // Key 3
    "GHI4",    // Key 4
    "JKL5",    // Key 5
    "MNO6",    // Key 6
    "PQRS7",   // Key 7
    "TUV8",    // Key 8
    "WXYZ9"    // Key 9
};

KeypadHandler::KeypadHandler() 
    : customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS),
      lastRawKey(NO_KEY), lastPressTime(0), tapIndex(0), 
      currentMessage(""), activePreview(false) {}

void KeypadHandler::begin() {
    // Initialized via constructor
}

char KeypadHandler::scanKeypad() {
    return customKeypad.getKey();
}

void KeypadHandler::commitCurrentChar() {
    if (activePreview && lastRawKey >= '0' && lastRawKey <= '9') {
        char finalized = keyMap[lastRawKey - '0'][tapIndex];
        currentMessage += finalized;
        activePreview = false;
        lastRawKey = NO_KEY;
        tapIndex = 0;
    }
}

void KeypadHandler::update() {
    char rawKey = customKeypad.getKey();

    if (activePreview && (millis() - lastPressTime > MULTI_TAP_TIMEOUT)) {
        commitCurrentChar();
    }

    if (rawKey == NO_KEY) return;

    if (rawKey == '*' || rawKey == 'A') {
        if (activePreview) {
            activePreview = false;
            lastRawKey = NO_KEY;
        } else if (currentMessage.length() > 0) {
            currentMessage.remove(currentMessage.length() - 1);
        }
        return;
    }

    if (rawKey < '0' || rawKey > '9') {
        commitCurrentChar();
        return;
    }

    if (rawKey == lastRawKey && activePreview) {
        uint8_t len = strlen(keyMap[rawKey - '0']);
        tapIndex = (tapIndex + 1) % len;
    } else {
        commitCurrentChar();
        lastRawKey = rawKey;
        tapIndex = 0;
        activePreview = true;
    }

    lastPressTime = millis();
}
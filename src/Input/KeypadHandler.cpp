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

// Nokia Multi-Tap Mapping Arrays (Cycle order: Letter -> Letter -> Letter -> Number)
const char* keyMap[] = {
    "1",       // 0 (unused)
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
      lastRawKey(NO_KEY), lastPressTime(0), tapIndex(0) {}

void KeypadHandler::begin() {
    // Keypad setup initialized via constructor
}

char KeypadHandler::scanKeypad() {
    return customKeypad.getKey();
}

char KeypadHandler::processMultiTap(char rawKey, bool &isCharacterFinalized) {
    isCharacterFinalized = false;

    if (rawKey == NO_KEY) {
        // Check if timeout reached for multi-tap cycling
        if (lastRawKey != NO_KEY && (millis() - lastPressTime > MULTI_TAP_TIMEOUT)) {
            isCharacterFinalized = true;
            char finalizedChar = keyMap[lastRawKey - '0'][tapIndex];
            lastRawKey = NO_KEY;
            tapIndex = 0;
            return finalizedChar;
        }
        return '\0';
    }

    // Direct system/navigation keys (A, B, C, D, *, #)
    if (rawKey < '0' || rawKey > '9') {
        lastRawKey = NO_KEY;
        isCharacterFinalized = true;
        return rawKey;
    }

    // Number keys (0-9) Multi-tap logic
    if (rawKey == lastRawKey) {
        uint8_t len = strlen(keyMap[rawKey - '0']);
        tapIndex = (tapIndex + 1) % len; // Cycle to next character
    } else {
        if (lastRawKey != NO_KEY) {
            // New key pressed before timeout -> Finalize previous key
            isCharacterFinalized = true;
        }
        lastRawKey = rawKey;
        tapIndex = 0;
    }

    lastPressTime = millis();
    return keyMap[rawKey - '0'][tapIndex];
}
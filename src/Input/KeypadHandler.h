#pragma once
#include <Keypad.h>
#include "Config.h"

class KeypadHandler {
private:
    Keypad customKeypad;
    char lastRawKey;
    unsigned long lastPressTime;
    uint8_t tapIndex;
    static const uint16_t MULTI_TAP_TIMEOUT = 1000; // 1 second timeout

public:
    KeypadHandler();
    void begin();
    char scanKeypad();
    char processMultiTap(char rawKey, bool &isCharacterFinalized);
};
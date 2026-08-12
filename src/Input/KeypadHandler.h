#ifndef KEYPAD_HANDLER_H
#define KEYPAD_HANDLER_H

#include <Arduino.h>
#include <Keypad.h>
#include "Config.h"

class KeypadHandler {
private:
    Keypad customKeypad;
    char lastRawKey;
    unsigned long lastPressTime;
    uint8_t tapIndex;
    static const uint16_t MULTI_TAP_TIMEOUT = 1000;

    String currentMessage;
    bool activePreview;

    void commitCurrentChar();

public:
    KeypadHandler();
    void begin();
    
    char scanKeypad(); // Added back to scan keys directly
    void update();

    String getMessage() const { return currentMessage; }
    void clearMessage() { currentMessage = ""; activePreview = false; }
};

#endif
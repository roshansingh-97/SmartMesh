#ifndef KEYPAD_HANDLER_H
#define KEYPAD_HANDLER_H

#include <Arduino.h>
#include <Keypad.h>

class KeypadHandler {
private:
    Keypad customKeypad;

public:
    KeypadHandler();
    void begin();
    char getKey(); // Exposes key reading to main loop
};

#endif // KEYPAD_HANDLER_H
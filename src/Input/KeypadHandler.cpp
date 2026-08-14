#include "KeypadHandler.h"

// Keypad layout definition
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// ESP32 GPIO pin mapping
byte rowPins[ROWS] = {13, 12, 14, 27}; 
byte colPins[COLS] = {26, 25, 33, 32}; 

KeypadHandler::KeypadHandler() 
    : customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {
}

void KeypadHandler::begin() {
    // Keypad initialization if required by hardware
}

char KeypadHandler::getKey() {
    return customKeypad.getKey();
}
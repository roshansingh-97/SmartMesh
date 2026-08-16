#include "KeypadHandler.h"
#include "../Config.h" // Added Config.h include

// Keypad layout definition
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Use pin definitions from Config.h (GPIO 18 for R2)
byte rowPins[ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4}; 
byte colPins[COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4}; 

KeypadHandler::KeypadHandler() 
    : customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {
}

void KeypadHandler::begin() {
    // Keypad initialization if required by hardware
}

char KeypadHandler::getKey() {
    return customKeypad.getKey();
}
#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <Arduino.h>

enum class InputMode {
    LOWERCASE,  // "abc"
    UPPERCASE,  // "ABC"
    NUMERIC     // "123"
};

class TextInput {
public:
    static const uint8_t MAX_MSG_LEN = 140;
    static const uint16_t TAP_TIMEOUT_MS = 800;

private:
    char buffer[MAX_MSG_LEN + 1];
    uint8_t cursor;
    uint8_t length;

    InputMode mode;

    // Multi-tap state
    char lastKey;
    uint8_t tapIndex;
    uint32_t lastTapMs;
    bool pendingChar;

    char getCandidateChar(char key, uint8_t tapIdx) const;
    void cycleMode();

public:
    TextInput();

    void begin();
    void update(); // Must be called regularly in UI update loop to handle the 800ms commit timeout

    void handleKeyPress(char key);
    
    void commitPendingChar();
    void backspace();
    void moveCursorLeft();
    void moveCursorRight();
    void clear();

    // Accessors
    const char* getBuffer() const { return buffer; }
    uint8_t getCursorPosition() const { return cursor; }
    uint8_t getLength() const { return length; }
    InputMode getMode() const { return mode; }
    const char* getModeString() const;
    bool isPending() const { return pendingChar; }
};

#endif
#include "TextInput.h"

// Multi-tap lookup tables
static const char* MAP_LOWER[] = {
    " 0",        // 0
    ".,!?1",     // 1
    "abc2",      // 2
    "def3",      // 3
    "ghi4",      // 4
    "jkl5",      // 5
    "mno6",      // 6
    "pqrs7",     // 7
    "tuv8",      // 8
    "wxyz9"      // 9
};

static const char* MAP_UPPER[] = {
    " 0",        // 0
    ".,!?1",     // 1
    "ABC2",      // 2
    "DEF3",      // 3
    "GHI4",      // 4
    "JKL5",      // 5
    "MNO6",      // 6
    "PQRS7",     // 7
    "TUV8",      // 8
    "WXYZ9"      // 9
};

TextInput::TextInput() : cursor(0), length(0), mode(InputMode::LOWERCASE),
                         lastKey('\0'), tapIndex(0), lastTapMs(0), pendingChar(false) {
    memset(buffer, 0, sizeof(buffer));
}

void TextInput::begin() {
    clear();
}

void TextInput::clear() {
    memset(buffer, 0, sizeof(buffer));
    cursor = 0;
    length = 0;
    lastKey = '\0';
    tapIndex = 0;
    lastTapMs = 0;
    pendingChar = false;
    mode = InputMode::LOWERCASE;
}

const char* TextInput::getModeString() const {
    switch (mode) {
        case InputMode::LOWERCASE: return "abc";
        case InputMode::UPPERCASE: return "ABC";
        case InputMode::NUMERIC:   return "123";
        default:                   return "abc";
    }
}

void TextInput::cycleMode() {
    commitPendingChar();
    switch (mode) {
        case InputMode::LOWERCASE: mode = InputMode::UPPERCASE; break;
        case InputMode::UPPERCASE: mode = InputMode::NUMERIC;   break;
        case InputMode::NUMERIC:   mode = InputMode::LOWERCASE; break;
    }
}

char TextInput::getCandidateChar(char key, uint8_t tapIdx) const {
    if (key < '0' || key > '9') return '\0';
    uint8_t digit = key - '0';

    const char* map = (mode == InputMode::UPPERCASE) ? MAP_UPPER[digit] : MAP_LOWER[digit];
    uint8_t mapLen = strlen(map);

    return map[tapIdx % mapLen];
}

void TextInput::commitPendingChar() {
    if (!pendingChar) return;

    pendingChar = false;
    lastKey = '\0';
    tapIndex = 0;

    // Advance cursor after committing
    if (cursor < MAX_MSG_LEN) {
        cursor++;
        if (cursor > length) {
            length = cursor;
        }
    }
    buffer[length] = '\0';
}

void TextInput::update() {
    // Check 800ms multi-tap commit timeout
    if (pendingChar && (millis() - lastTapMs >= TAP_TIMEOUT_MS)) {
        commitPendingChar();
    }
}

void TextInput::handleKeyPress(char key) {
    // 1. Mode Cycling (*)
    if (key == '*') {
        cycleMode();
        return;
    }

    // 2. Backspace (#)
    if (key == '#') {
        backspace();
        return;
    }

    // 3. Optional Cursor Movements
    if (key == 'A') {
        moveCursorLeft();
        return;
    }
    if (key == 'B') {
        moveCursorRight();
        return;
    }

    // 4. Number / Character Keys ('0' through '9')
    if (key >= '0' && key <= '9') {
        if (mode == InputMode::NUMERIC) {
            commitPendingChar();
            if (length < MAX_MSG_LEN) {
                // Insert character at cursor position
                memmove(&buffer[cursor + 1], &buffer[cursor], length - cursor + 1);
                buffer[cursor] = key;
                cursor++;
                length++;
            }
            return;
        }

        // Multi-tap logic for LOWERCASE and UPPERCASE
        if (pendingChar && key == lastKey) {
            // Cycle through characters on the same key
            tapIndex++;
            char c = getCandidateChar(key, tapIndex);
            buffer[cursor] = c;
            lastTapMs = millis();
        } else {
            // New key pressed
            commitPendingChar();

            if (length < MAX_MSG_LEN) {
                lastKey = key;
                tapIndex = 0;
                pendingChar = true;
                lastTapMs = millis();

                char c = getCandidateChar(key, tapIndex);
                // Shift text right if inserting in the middle
                memmove(&buffer[cursor + 1], &buffer[cursor], length - cursor + 1);
                buffer[cursor] = c;
                length++;
            }
        }
    }
}

void TextInput::backspace() {
    if (pendingChar) {
        // Cancel the current multi-tap candidate without deleting previous text
        pendingChar = false;
        memmove(&buffer[cursor], &buffer[cursor + 1], length - cursor);
        length--;
        buffer[length] = '\0';
        lastKey = '\0';
        return;
    }

    if (cursor > 0) {
        // Delete character to the left of the cursor
        memmove(&buffer[cursor - 1], &buffer[cursor], length - cursor + 1);
        cursor--;
        length--;
        buffer[length] = '\0';
    }
}

void TextInput::moveCursorLeft() {
    commitPendingChar();
    if (cursor > 0) {
        cursor--;
    }
}

void TextInput::moveCursorRight() {
    commitPendingChar();
    if (cursor < length) {
        cursor++;
    }
}
#ifndef COMPOSE_SCREEN_H
#define COMPOSE_SCREEN_H

#include <Arduino.h>
#include "Screen.h"
#include "../../input/TextInput.h" // Fixed: points to src/input/TextInput.h

class ComposeScreen : public Screen {
private:
    TextInput textInput;

public:
    ComposeScreen();
    void initialize() override;
    void onEnter() override;
    void update() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& uiManager) override;
};

#endif // COMPOSE_SCREEN_H
#ifndef COMPOSE_SCREEN_H
#define COMPOSE_SCREEN_H

#include "../Screen.h"
#include "../../Input/TextInput.h"

class ComposeScreen : public Screen {
private:
    TextInput textInput;

public:
    ComposeScreen();

    void initialize() override;
    void onEnter() override;
    void update() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& ui) override;

    TextInput& getTextInput() { return textInput; }
};

#endif
#ifndef COMPOSE_SCREEN_H
#define COMPOSE_SCREEN_H

#include <Arduino.h>
#include "Screen.h"
#include "../../Input/TextInput.h"

class SmartMeshApp;

class ComposeScreen : public Screen {
private:
    TextInput textInput;
    SmartMeshApp* appOwner = nullptr;

public:
    ComposeScreen();
    void setAppOwner(SmartMeshApp* owner) { appOwner = owner; }
    void initialize() override;
    void onEnter() override;
    void update() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& uiManager) override;
};

#endif // COMPOSE_SCREEN_H

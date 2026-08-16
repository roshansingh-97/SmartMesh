#ifndef ABOUT_SCREEN_H
#define ABOUT_SCREEN_H

#include <Arduino.h>
#include "Screen.h"

class AboutScreen : public Screen {
public:
    AboutScreen();
    void initialize() override;
    void onEnter() override;
    void draw(DisplayManager& display) override;
    void handleInput(char key, UIManager& uiManager) override;
};

#endif // ABOUT_SCREEN_H
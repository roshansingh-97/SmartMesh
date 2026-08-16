#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

class DisplayManager;
class UIManager;

class Screen {
public:
    virtual ~Screen() {}
    virtual void initialize() {}
    virtual void onEnter() {}
    virtual void update() {}
    virtual void draw(DisplayManager& display) = 0;
    virtual void handleInput(char key, UIManager& uiManager) = 0;
};

#endif // SCREEN_H
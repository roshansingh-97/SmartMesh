#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

class UIManager;
class DisplayManager;

class Screen {
public:
    virtual ~Screen() {}

    virtual void initialize() {}
    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void draw(DisplayManager& display) = 0;
    virtual void update() {}
    virtual void handleInput(char key, UIManager& ui) = 0;
};

#endif
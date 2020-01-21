#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "button.h"

class Screen {
protected:
    std::vector<Button> buttons;
    void drawButton(Button& btn, int color, int textSize);
    void drawFanStatus();

public:
    void renderButtons(bool forced = false);
    void processTouch(int x, int y);

//virtual:
    virtual String getName() = 0;
    virtual void render() = 0;
    virtual void update() {};
    virtual void renderButton(Button& btn) = 0;
    virtual void renderButtonPressed(Button& btn) = 0;
    virtual void handleButton(Button& btn) = 0;
    virtual ~Screen() = default;
};

#endif
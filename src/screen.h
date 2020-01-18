#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "button.h"

class Screen {
protected:
    std::vector<Button> buttons;

public:
    virtual String getName();
    virtual void render();
    virtual void renderButton(Button& btn);
    virtual void renderButtonPressed(Button& btn);
    virtual void handleButton(Button& btn);

    void renderButtons(bool forced = false);
    void processTouch(int x, int y);
};

#endif
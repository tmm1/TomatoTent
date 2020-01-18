#ifndef FANSCREEN_H
#define FANSCREEN_H

#include "../screen.h"

class FanScreen: public Screen {
public:
    String getName() { return "fanScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button &btn);
};
#endif

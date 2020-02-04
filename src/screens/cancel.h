#ifndef CANCELSCREEN_H
#define CANCELSCREEN_H

#include "../screen.h"

class CancelScreen : public Screen {
    void drawDay();

public:
    String getName() { return "cancelScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button& btn);
};
#endif

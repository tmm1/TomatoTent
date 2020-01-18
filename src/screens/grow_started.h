#ifndef GROWSTARTEDSCREEN_H
#define GROWSTARTEDSCREEN_H

#include "../screen.h"

class GrowStartedScreen: public Screen {
public:
    String getName() { return "growStartedScreen"; }
    void render();
    void renderButton(Button& btn) {}
    void renderButtonPressed(Button& btn) {}
    void handleButton(Button &btn) {}
};
#endif

#ifndef CANCELCONFIRMSCREEN_H
#define CANCELCONFIRMSCREEN_H

#include "../screen.h"

class CancelConfirmScreen: public Screen {
public:
    String getName() { return "cancelConfirmScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button &btn);
};
#endif

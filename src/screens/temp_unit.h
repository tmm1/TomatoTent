#ifndef TEMPUNITSCREEN_H
#define TEMPUNITSCREEN_H

#include "../screen.h"

class TempUnitScreen: public Screen {
public:
    String getName() { return "tempUnitScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button &btn);
};
#endif

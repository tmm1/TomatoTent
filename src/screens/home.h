#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../screen.h"

class HomeScreen: public Screen {
public:
    String getName() { return "homeScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button &btn);
};
#endif
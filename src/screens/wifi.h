#ifndef WIFISCREEN_H
#define WIFISCREEN_H

#include "../screen.h"

class WifiScreen: public Screen {
public:
    String getName() { return "wifiScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button &btn);
};
#endif

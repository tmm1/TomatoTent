#ifndef WIFISPLASHSCREEN_H
#define WIFISPLASHSCREEN_H

#include "../screen.h"

class WifiSplashScreen : public Screen {
public:
    String getName() { return "WifiSplashScreen"; }
    void render();
    void renderButton(Button& btn) {}
    void renderButtonPressed(Button& btn) {}
    void handleButton(Button& btn) {}
};
#endif

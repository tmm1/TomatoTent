#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Particle.h>
#include <Arduino.h>

#include "button.h"
#include "screen.h"
#include "screens/home.h"

class ScreenManager {
public:
    std::unique_ptr<Screen> current = std::make_unique<HomeScreen>();
    String currentScreen = "homeScreen";

    ScreenManager() {};

    void renderButtons(bool forced = false);

    void homeScreen();
    void growStartedScreen();
    void cancelScreen();
    void cancelConfirmationScreen();
    void timerScreen();
    void wifiScreen();
    void wifiSplashScreen();
    void fanScreen();
    void tempUnitScreen();
};
#endif

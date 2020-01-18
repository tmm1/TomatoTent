#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Particle.h>
#include <Arduino.h>

#include <button.h>

class ScreenManager {

    void clearButtons();

public:
    Button buttons[6];
    String currentScreen = "homeScreen";

    ScreenManager() {};

    void renderButtons(bool forced = false);

    void homeScreen();
    void growStartedScreen();
    void cancelScreen();
    void cancelConfirmationScreen();
    void timerScreen();
    void wifiScreen();
    void wifiSetupScreen();
    void fanScreen();
    void tempUnitScreen();
};
#endif

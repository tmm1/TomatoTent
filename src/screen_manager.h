#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Particle.h>
#include <Arduino.h>

#include "button.h"
#include "screen.h"
#include "screens/home.h"

enum redrawMarker {
    TEMPERATURE = 1,
    HUMIDITY = 2,
    WATER_LEVEL = 4,
    TIMER = 8,
    FAN = 16,
    DAY = 32,
    DIMMED = 64
};

class ScreenManager {
private:
    int redrawMarkers;

    void render();

public:
    std::unique_ptr<Screen> current = std::make_unique<HomeScreen>();

    ScreenManager() {};

    void renderButtons(bool forced = false);
    void markNeedsRedraw(redrawMarker m);
    bool wasNeedsRedraw(redrawMarker m);

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

#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../screen.h"

class HomeScreen : public Screen {
private:
    void drawTemperature();
    void drawHumidity();
    void drawWaterLevel();
    void drawDayCounter();
    void drawTimerStatus();
    void drawClock();

public:
    String getName() { return "homeScreen"; }
    void render();
    void update();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button& btn);
};
#endif
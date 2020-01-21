#ifndef TIMERSCREEN_H
#define TIMERSCREEN_H

#include "../screen.h"

class TimerScreen : public Screen {
public:
    String getName() { return "timerScreen"; }
    void render();
    void renderButton(Button& btn);
    void renderButtonPressed(Button& btn);
    void handleButton(Button& btn);

private:
    void renderDayDuration(int dayDuration);
};
#endif

#include <Particle.h>
#include <Arduino.h>
#include "screen.h"

void Screen::renderButtons(bool forced)
{
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        Button& btn = *it;
        if (forced) {
            this->renderButton(btn);
        } else if (btn.getStatus() == "pressed") {
            btn.setStatus("none");
            this->renderButton(btn);
        }
    }
}

void Screen::processTouch(int x, int y)
{
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        Button& btn = *it;
        if (!btn.isPressed(x, y)) {
            continue;
        }
        if (btn.getStatus() != "none") {
            continue;
        }

        btn.setStatus("pressed");
        this->renderButtonPressed(btn);
        this->handleButton(btn);
    }
}
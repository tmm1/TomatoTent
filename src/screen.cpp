#include <Particle.h>
#include <Arduino.h>
#include "screen.h"

void Screen::renderButtons(bool forced)
{
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        Button& btn = *it;
        if (btn.getStatus() == "pressed") {
            this->renderButtonPressed(btn);
            btn.setStatus("none");
        } else if (forced) {
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

        this->handleButton(btn);
    }
}
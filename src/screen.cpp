#include <Particle.h>
#include <Arduino.h>
#include "screen.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;

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

void Screen::drawButton(Button& btn, int color, int textSize)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
    tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, color);

    tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(textSize);
    tft.print(buttonText);
}

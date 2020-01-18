#include "button.h"
#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include "icons.h"
#include "systemStatus.h"

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;

Button::Button() {}
Button::Button(String name, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, String buttonText, uint16_t textOffsetLeft, uint16_t textOffsetTop)
    : name { name }
    , x0 { x0 }
    , y0 { y0 }
    , w { w }
    , h { h }
    , buttonText { buttonText }
    , textOffsetLeft { textOffsetLeft }
    , textOffsetTop { textOffsetTop }
{
    this->pressed = false;
    this->currentStatus = "none";
    this->render();
}

void Button::render()
{
}

void Button::renderPressed()
{
}

bool Button::isPressed(int x, int y)
{
    if ((x > this->x0 && x < (this->x0 + this->w)) && (y > this->y0 && (y < (this->y0 + this->h)))) {
        this->renderPressed();
        return true;
    }

    if (this->pressed) {
        this->setStatus("none");
    }

    return false;
}
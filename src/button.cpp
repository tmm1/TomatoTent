#include "button.h"

Button::Button(String name, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, String buttonText, uint16_t textOffsetLeft, uint16_t textOffsetTop)
    : name { name }
    , x0 { x0 }
    , x1 { x0 + w }
    , y0 { y0 }
    , y1 { y0 + h }
    , w { w }
    , h { h }
    , buttonText { buttonText }
    , textOffsetLeft { textOffsetLeft }
    , textOffsetTop { textOffsetTop }
{
    this->currentStatus = "none";
}

bool Button::isPressed(int x, int y)
{
    return (x > x0 && x < x1) && (y > y0 && y < y1);
}
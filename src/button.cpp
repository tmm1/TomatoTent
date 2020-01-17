#include "button.h"

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
};

void Button::render()
{
    if (this->getName() == "startGrowBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);

        tft.print(buttonText);
    }

    if (this->getName() == "dayCounterBtn") {
    }

    if (this->getName() == "cancelScreenOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);

        tft.print(buttonText);
    }

    if (this->getName() == "terminateBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_RED);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);

        tft.print(buttonText);
    }

    if (this->getName() == "terminateYesBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_RED);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);

        tft.print(buttonText);
    }

    if (this->getName() == "terminateNoBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);

        tft.print(buttonText);
    }

    if (this->getName() == "timerBtn") {
    }

    if (this->getName() == "timerUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);
    }

    if (this->getName() == "timerDownBtn") {
        tft.fillTriangle(240, 170, 260, 130, 280, 170, ILI9341_RED);
        tft.drawTriangle(240, 170, 260, 130, 280, 170, ILI9341_LIGHTGREY);
    }

    if (this->getName() == "timerOkBtn") {

        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }

    if (this->getName() == "wifiBtn") {

        tft.drawBitmap(289, 5, iconWifi_24x24, 24, 24, ILI9341_DARKGREY);
    }

    if (this->getName() == "wifiOnBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }

    if (this->getName() == "wifiOffBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }

    if (this->getName() == "wifiOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }

    if (this->getName() == "fanBtn") {
    }

    if (this->getName() == "fanAutoBtn") {

        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (systemStatus.getFanAutoMode()) {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);
    }

    if (this->getName() == "fanManualBtn") {

        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (!systemStatus.getFanAutoMode()) {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);
    }

    if (this->getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);
    }

    if (this->getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_RED);
        tft.drawTriangle(240, 110, 260, 150, 280, 110, ILI9341_LIGHTGREY);
    }

    if (this->getName() == "fanOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }

    if (this->getName() == "tempBtn") {
    }

    if (this->getName() == "tempFahrenheitBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (systemStatus.getTempUnit() == 'F') {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);
    }

    if (this->getName() == "tempCelsiusBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (systemStatus.getTempUnit() == 'C') {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);
    }

    if (this->getName() == "tempUnitOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }
}

void Button::renderPressed()
{

    if (this->getName() == "startGrowBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_RED);
    }

    if (this->getName() == "timerUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_WHITE);
    }
    if (this->getName() == "timerDownBtn") {
        tft.fillTriangle(240, 170, 260, 130, 280, 170, ILI9341_WHITE);
    }

    if (this->getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_WHITE);
    }

    if (this->getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_WHITE);
    }
}
String Button::getName()
{
    return this->name;
};
uint16_t Button::getx0()
{
    return this->x0;
};
uint16_t Button::gety0()
{
    return this->y0;
};
uint16_t Button::getW()
{
    return this->w;
};
uint16_t Button::getH()
{
    return this->h;
};
String Button::getButtonText()
{
    return this->buttonText;
};
uint16_t Button::getTextOffsetLeft()
{
    return this->textOffsetLeft;
};
uint16_t Button::getTextOffsetTop()
{
    return this->textOffsetTop;
};

bool Button::isPressed(int x, int y)
{

    if ((x > this->getx0() && x < (this->getx0() + this->getW())) && (y > this->gety0() && (y < (this->gety0() + this->getH())))) {

        this->renderPressed();
        return true;

    } else {
        if (this->pressed) {
            this->setStatus("none");
        }
        return false;
    }
}

void Button::setStatus(String status)
{
    this->currentStatus = status;
}

String Button::getStatus()
{
    return this->currentStatus;
}
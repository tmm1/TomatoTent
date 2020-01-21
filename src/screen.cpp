#include <Particle.h>
#include <Arduino.h>
#include "screen.h"
#include "icons.h"
#include "systemStatus.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
extern SystemStatus systemStatus;
extern Tent tent;

Screen::Screen(): tft { screenManager.tft }
{
}

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
        break;
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

void Screen::drawFanStatus()
{
    //write status to screen
    tft.fillRect(200, 10, 56, 35, ILI9341_BLACK);

    tft.setCursor(210, 10);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);

    tft.print(String(String::format("%.0f", systemStatus.getFanSpeed() + 5)));

    tft.print("%");

    tft.setTextSize(1);
    if (systemStatus.getFanAutoMode()) {
        tft.setCursor(200, 30);
        tft.print("automatic");
    } else {
        tft.setCursor(210, 30);
        tft.print("manual");
    }
}

void Screen::update()
{
    if (screenManager.wasNeedsRedraw(DIMMED)) {
        if (tent.getGrowLightStatus() == "LOW") {
            drawDimmedIndicator();
        } else {
            hideDimmedIndicator();
        }
    }
}

void Screen::drawDimmedIndicator()
{
    tft.fillRoundRect(0, 220, 320, 25, 5, ILI9341_RED);

    tft.setCursor(120, 222);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Dimmed (" + String(tent.dimTimeout) + "m)");

    tft.drawBitmap(97, 222, iconBulb_16x16, 16, 16, ILI9341_WHITE);
}

void Screen::hideDimmedIndicator()
{
    tft.fillRoundRect(0, 220, 320, 25, 5, ILI9341_BLACK);
}

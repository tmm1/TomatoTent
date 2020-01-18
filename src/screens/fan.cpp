#include "fan.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;

void FanScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.drawBitmap(20, 4, fan_36, 36, 36, ILI9341_WHITE);

    tft.setCursor(60, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Speed");

    buttons.push_back(Button("fanAutoBtn", 20, 65, 150, 38, "Automatic", 20, 11));
    buttons.push_back(Button("fanManualBtn", 20, 115, 150, 38, "Manual", 40, 11));
    buttons.push_back(Button("fanUpBtn", 240, 50, 40, 40, "", 0, 0));
    buttons.push_back(Button("fanDownBtn", 240, 110, 40, 40, "", 0, 0));
    buttons.push_back(Button("fanOkBtn", 20, 180, 250, 38, "Ok", 110, 8));

    renderButtons(true);

    systemStatus.drawFanSpeed();
}

void FanScreen::renderButton(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "fanAutoBtn") {
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

    } else if (btn.getName() == "fanManualBtn") {
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

    } else if (btn.getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_RED);
        tft.drawTriangle(240, 110, 260, 150, 280, 110, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "fanOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }
}

void FanScreen::renderButtonPressed(Button& btn)
{
    if (btn.getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_WHITE);

    } else if (btn.getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_WHITE);
    }
}

void FanScreen::handleButton(Button& btn)
{
    if (btn.getName() == "fanAutoBtn") {
        systemStatus.setFanAutoMode(true);
        renderButtons(true);
        systemStatus.check_fan();

    } else if (btn.getName() == "fanManualBtn") {
        systemStatus.setFanAutoMode(false);
        renderButtons(true);
        systemStatus.check_fan();

    } else if (btn.getName() == "fanUpBtn") {
        float fanSpeedPercent = systemStatus.getFanSpeed();
        systemStatus.setFanAutoMode(false);
        renderButtons(true);

        if (fanSpeedPercent < 100) {
            fanSpeedPercent += 5;
            systemStatus.setFanSpeed(fanSpeedPercent);
            systemStatus.check_fan();
        }

    } else if (btn.getName() == "fanDownBtn") {
        float fanSpeedPercent = systemStatus.getFanSpeed();
        systemStatus.setFanAutoMode(false);
        renderButtons(true);

        if (fanSpeedPercent > 0) {
            fanSpeedPercent -= 5;
            systemStatus.setFanSpeed(fanSpeedPercent);
            systemStatus.check_fan();
        }

    } else if (btn.getName() == "fanOkBtn") {
        screenManager.homeScreen();
    }
}

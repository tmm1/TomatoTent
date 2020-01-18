#include "timer.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;

void TimerScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setCursor(20, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Light Timer");

    int dayDuration = systemStatus.getDayDuration();

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(10, 70);
    tft.print(String(dayDuration / 60) + " Hours ON");

    int nightDuration = (24 * 60) - dayDuration;
    tft.setCursor(10, 140);
    tft.print(String(nightDuration / 60) + " Hours OFF");

    buttons.push_back(Button("timerUpBtn", 240, 50, 40, 40, "", 0, 0));
    buttons.push_back(Button("timerDownBtn", 240, 130, 40, 40, "", 0, 0));
    buttons.push_back(Button("timerOkBtn", 20, 180, 250, 38, "Ok", 110, 8));

    renderButtons(true);

    if (tent.getGrowLightStatus() == "LOW") {
        tent.drawDimmedIndicator();
    }
}

void TimerScreen::renderButton(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "timerUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "timerDownBtn") {
        tft.fillTriangle(240, 170, 260, 130, 280, 170, ILI9341_RED);
        tft.drawTriangle(240, 170, 260, 130, 280, 170, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "timerOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
    }
}

void TimerScreen::renderButtonPressed(Button& btn)
{
}

void TimerScreen::renderDayDuration(int dayDuration)
{
}

void TimerScreen::handleButton(Button& btn)
{
    if (btn.getName() == "timerUpBtn") {
        int dayDuration = systemStatus.getDayDuration() + 60;

        if (dayDuration > 1440) {
            dayDuration = 60;
        }

        systemStatus.setDayDuration(dayDuration);

        tft.fillRect(10, 70, 200, 22, ILI9341_BLACK);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.setCursor(10, 70);
        tft.print(String(dayDuration / 60) + " Hours ON");

        tft.setCursor(10, 140);
        tft.fillRect(10, 140, 215, 22, ILI9341_BLACK);
        int nightDuration = (24 * 60) - dayDuration;
        tft.print(String(nightDuration / 60) + " Hours OFF");

    } else if (btn.getName() == "timerDownBtn") {
        int dayDuration = systemStatus.getDayDuration() - 60;

        if (dayDuration <= 0) {
            dayDuration = 1440;
        }

        systemStatus.setDayDuration(dayDuration);

        tft.fillRect(10, 70, 200, 22, ILI9341_BLACK);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.setCursor(10, 70);
        tft.print(String(dayDuration / 60) + " Hours ON");

        tft.setCursor(10, 140);
        tft.fillRect(10, 140, 215, 22, ILI9341_BLACK);
        int nightDuration = (24 * 60) - dayDuration;
        tft.print(String(nightDuration / 60) + " Hours OFF");

    } else if (btn.getName() == "timerOkBtn") {
        screenManager.homeScreen();
    }
}

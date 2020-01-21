#include "timer.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"

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
    renderDayDuration(dayDuration);

    buttons.push_back(Button("timerUpBtn", 240, 50, 40, 40, "", 0, 0));
    buttons.push_back(Button("timerDownBtn", 240, 130, 40, 40, "", 0, 0));
    buttons.push_back(Button("timerOkBtn", 20, 180, 250, 38, "Ok", 110, 8));

    renderButtons(true);
}

void TimerScreen::renderButton(Button& btn)
{
    if (btn.getName() == "timerUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "timerDownBtn") {
        tft.fillTriangle(240, 170, 260, 130, 280, 170, ILI9341_RED);
        tft.drawTriangle(240, 170, 260, 130, 280, 170, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "timerOkBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);
    }
}

void TimerScreen::renderButtonPressed(Button& btn)
{
    if (btn.getName() == "timerUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_WHITE);
    } else if (btn.getName() == "timerDownBtn") {
        tft.fillTriangle(240, 170, 260, 130, 280, 170, ILI9341_WHITE);
    }
}

void TimerScreen::renderDayDuration(int dayDuration)
{
    tft.fillRect(10, 70, 200, 22, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 70);
    tft.print(String(dayDuration / 60) + " Hours ON");

    tft.setCursor(10, 140);
    tft.fillRect(10, 140, 215, 22, ILI9341_BLACK);
    int nightDuration = (24 * 60) - dayDuration;
    tft.print(String(nightDuration / 60) + " Hours OFF");
}

void TimerScreen::handleButton(Button& btn)
{
    if (btn.getName() == "timerUpBtn") {
        int dayDuration = systemStatus.getDayDuration() + 60;
        if (dayDuration > 1440) {
            dayDuration = 60;
        }
        systemStatus.setDayDuration(dayDuration);
        renderDayDuration(dayDuration);

    } else if (btn.getName() == "timerDownBtn") {
        int dayDuration = systemStatus.getDayDuration() - 60;
        if (dayDuration <= 0) {
            dayDuration = 1440;
        }
        systemStatus.setDayDuration(dayDuration);
        renderDayDuration(dayDuration);

    } else if (btn.getName() == "timerOkBtn") {
        screenManager.homeScreen();
    }
}

#include "home.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;
extern Timer minutelyTicker, draw_temp_home;

void HomeScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);
    buttons.clear();
    buttons.push_back(Button("wifiBtn", 260, 0, 60, 30, "", 18, 8));

    if (systemStatus.getDayCount() == -1) {

        tft.setTextColor(ILI9341_LIGHTGREY);
        tft.setTextSize(2);

        tft.setCursor(126, 15);
        tft.print("Hi,");

        tft.setCursor(75, 40);
        tft.print("I'm Tomatotent.");

        tft.drawBitmap(110, 65, tomato_filled_100, 100, 100, ILI9341_RED);

        buttons.push_back(Button("startGrowBtn", 35, 180, 250, 38, "Start a Grow", 18, 8));

    } else { // a grow is in progress

        tft.drawRect(20, 180, 250, 38, ILI9341_BLACK);

        systemStatus.drawDayCounter();

        buttons.push_back(Button("dayCounterBtn", 20, 180, 250, 38, "", 18, 8));
        buttons.push_back(Button("timerBtn", 10, 10, 115, 25, "", 18, 8));
        buttons.push_back(Button("fanBtn", 145, 10, 115, 35, "", 18, 8));
        buttons.push_back(Button("tempBtn", 50, 55, 115, 35, "", 18, 8));

        tft.drawBitmap(165, 4, fan_36, 36, 36, ILI9341_WHITE);

        systemStatus.drawTimerStatus();
        systemStatus.check_fan();
        tent.drawStats(systemStatus.getTempUnit());

        if (tent.getGrowLightStatus() == "LOW") {
            tent.drawDimmedIndicator();
        }
    }

    renderButtons(true);
}

void HomeScreen::renderButton(Button& btn)
{
    if (btn.getName() == "startGrowBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);

    } else if (btn.getName() == "wifiBtn") {
        tft.drawBitmap(289, 5, iconWifi_24x24, 24, 24, ILI9341_DARKGREY);
    }
}

void HomeScreen::renderButtonPressed(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "startGrowBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_RED);
    }
}

void HomeScreen::handleButton(Button& btn)
{
    if (btn.getName() == "startGrowBtn") {
        tent.growLight("HIGH");
        systemStatus.setDayCount(1);

        screenManager.growStartedScreen();

        delay(3000);

        screenManager.homeScreen();

        tent.doCheckStats(); //First time right away
        draw_temp_home.start();

        systemStatus.countMinute(); // First time on new grow
        minutelyTicker.start();

    } else if (btn.getName() == "wifiBtn") {
        screenManager.wifiScreen();

    } else if (btn.getName() == "dayCounterBtn") {
        screenManager.cancelScreen();

    } else if (btn.getName() == "timerBtn") {
        screenManager.timerScreen();

    } else if (btn.getName() == "fanBtn") {
        screenManager.fanScreen();

    } else if (btn.getName() == "tempBtn") {
        screenManager.tempUnitScreen();
    }
}
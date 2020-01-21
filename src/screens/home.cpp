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
        tft.drawBitmap(165, 4, fan_36, 36, 36, ILI9341_WHITE);

        drawDayCounter();
        drawTemperature();
        drawHumidity();
        drawWaterLevel();
        drawTimerStatus();
        drawFanStatus();

        buttons.push_back(Button("dayCounterBtn", 20, 180, 250, 38, "", 18, 8));
        buttons.push_back(Button("timerBtn", 10, 10, 115, 25, "", 18, 8));
        buttons.push_back(Button("fanBtn", 145, 10, 115, 35, "", 18, 8));
        buttons.push_back(Button("tempBtn", 50, 55, 115, 35, "", 18, 8));
    }

    renderButtons(true);
}

void HomeScreen::update()
{
    if (screenManager.wasNeedsRedraw(TIMER))
        drawTimerStatus();
    if (screenManager.wasNeedsRedraw(TEMPERATURE))
        drawTemperature();
    if (screenManager.wasNeedsRedraw(HUMIDITY))
        drawHumidity();
    if (screenManager.wasNeedsRedraw(WATER_LEVEL))
        drawWaterLevel();
    if (screenManager.wasNeedsRedraw(FAN))
        drawFanStatus();
    if (screenManager.wasNeedsRedraw(DAY))
        drawDayCounter();

    Screen::update();
}

void HomeScreen::drawTemperature()
{
    char tempUnit = systemStatus.getTempUnit();

    tft.fillRect(50, 60, 141, 25, ILI9341_BLACK);
    tft.setCursor(50, 60);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);

    tft.print(String::format("%.1f", temp));
    tft.setTextSize(2);

    if (tempUnit == 'F') {
        tft.print(" F");
    } else {
        tft.print(" C");
    }
}

void HomeScreen::drawHumidity()
{
    tft.fillRect(50, 110, 141, 25, ILI9341_BLACK);
    tft.setCursor(50, 110);
    tft.setTextColor(ILI9341_PINK);
    tft.setTextSize(3);

    tft.print(String::format("%.1f", hum));

    tft.setTextSize(2);
    tft.print(" %");
}

void HomeScreen::drawWaterLevel()
{
    const float waterLevelBoxHeight = 150;
    const int waterLevelBoxTop = 60;

    int waterLevelHeight = floor((waterLevelBoxHeight / 100) * waterLevel);

    int waterLevelTop = (waterLevelBoxHeight - waterLevelHeight) + waterLevelBoxTop - 1;

    //icon
    tft.drawBitmap(280, 30, iconWateringCan_24x24, 24, 24, ILI9341_GREEN);

    //outside box
    tft.drawRect(280, waterLevelBoxTop, 25, waterLevelBoxHeight, ILI9341_DARKGREY);

    //reset the box
    tft.fillRect(281, waterLevelBoxTop + 1, 23, waterLevelBoxHeight - 2, ILI9341_BLACK);

    //draw current water level
    tft.fillRect(281, waterLevelTop, 23, waterLevelHeight, ILI9341_BLUE);
}

void HomeScreen::drawTimerStatus()
{
    int hoursLeft;
    int minutesLeft;

    if (systemStatus.isDay()) {
        tft.setTextColor(ILI9341_YELLOW);
        hoursLeft = floor((systemStatus.getDayDuration() - systemStatus.getMinutesInPhotoperiod()) / 60);
        minutesLeft = (systemStatus.getDayDuration() - systemStatus.getMinutesInPhotoperiod()) % 60;
    } else {
        tft.setTextColor(ILI9341_BLUE);
        hoursLeft = floor((((24 * 60) - systemStatus.getDayDuration()) - systemStatus.getMinutesInPhotoperiod()) / 60);
        minutesLeft = (((24 * 60) - systemStatus.getDayDuration()) - systemStatus.getMinutesInPhotoperiod()) % 60;
    }

    if (hoursLeft < 0 || minutesLeft < 0) {

        systemStatus.countMinute();

    } else {

        tft.fillRect(5, 5, 137, 37, ILI9341_BLACK);

        tft.setCursor(50, 10);
        tft.setTextSize(2);

        tft.print(String(hoursLeft));
        tft.setTextSize(1);
        tft.print("hrs ");
        tft.setTextSize(2);
        tft.print("" + String(minutesLeft));
        tft.setTextSize(1);
        tft.print("min");

        tft.setCursor(53, 31);
        tft.setTextSize(1);
        if (systemStatus.isDay()) {
            tft.drawBitmap(7, 5, sun_36, 36, 36, ILI9341_YELLOW);
            tft.print("until sunset");
        } else {
            tft.drawBitmap(7, 5, moon_and_stars_36, 36, 36, ILI9341_BLUE);
            tft.print("until sunrise");
        }
    }
}

void HomeScreen::drawDayCounter()
{
    tft.fillRect(130, 180, 80, 25, ILI9341_BLACK);

    tft.setCursor(70, 180);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);

    tft.print("Day " + String(systemStatus.getDayCount()));
}

void HomeScreen::drawClock()
{
    String currentTime = Time.format(Time.now(), "%l:%M %P %S");

    tft.fillRect(190, 7, 140, 18, ILI9341_BLACK);

    tft.setCursor(190, 7);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    tft.print(currentTime);
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
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h;

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
#include "screen.h"

Screen::Screen() {}

void Screen::homeScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "homeScreen";

    if (systemStatus.getDayCount() == -1) {

        tft.setTextColor(ILI9341_LIGHTGREY);
        tft.setTextSize(2);

        tft.setCursor(126, 15);
        tft.print("Hi,");

        tft.setCursor(75, 40);
        tft.print("I'm Tomatotent.");

        tft.drawBitmap(110, 65, tomato_filled_100, 100, 100, ILI9341_RED);

        Button startGrowBtn("startGrowBtn", 35, 180, 250, 38, "Start a Grow", 18, 8);
        buttons[0] = startGrowBtn;

    } else { // a grow is in progress

        tft.drawRect(20, 180, 250, 38, ILI9341_BLACK);

        systemStatus.drawDayCounter();

        Button dayCounterBtn("dayCounterBtn", 20, 180, 250, 38, "", 18, 8);
        buttons[1] = dayCounterBtn;

        Button timerBtn("timerBtn", 10, 10, 115, 25, "", 18, 8);
        buttons[2] = timerBtn;

        Button fanBtn("fanBtn", 145, 10, 115, 35, "", 18, 8);
        buttons[3] = fanBtn;

        Button tempBtn("tempBtn", 50, 55, 115, 35, "", 18, 8);
        buttons[4] = tempBtn;

        tft.drawBitmap(165, 4, fan_36, 36, 36, ILI9341_WHITE);

        systemStatus.drawTimerStatus();
        systemStatus.check_fan();
        tent.drawStats(systemStatus.getTempUnit());

        if (tent.getGrowLightStatus() == "LOW") {
            tent.drawDimmedIndicator();
        }
    }
    Button wifiBtn("wifiBtn", 260, 0, 60, 30, "", 18, 8);
    buttons[5] = wifiBtn;
}

void Screen::growStartedScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "growStartedScreen";

    tft.fillScreen(ILI9341_OLIVE);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);

    tft.setCursor(50, 30);
    tft.print("Your Grow");
    tft.setCursor(60, 90);
    tft.print("has started!");

    tft.drawBitmap(124, 160, plant_filled_72x72, 72, 72, ILI9341_WHITE);
}

void Screen::cancelScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "cancelScreen";

    tft.setCursor(20, 70);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Your grow is " + String(systemStatus.getDayCount() - 1) + " days old.");

    tft.setCursor(20, 100);
    tft.print("Most Cannabis is ready");

    tft.setCursor(20, 120);
    tft.print("after about 110 days.");

    /* Include if you want to know your free memory */
    //tft.setTextSize(1);
    //tft.setCursor(50,140);
    //uint32_t freemem = System.freeMemory();
    //tft.print(freemem);

    Button cancelScreenOkBtn("cancelScreenOkBtn", 20, 180, 250, 38, "Ok", 110, 8);
    buttons[0] = cancelScreenOkBtn;

    Button terminateBtn("terminateBtn", 120, 10, 185, 28, "Terminate Grow", 10, 7);
    buttons[1] = terminateBtn;

    if (tent.getGrowLightStatus() == "LOW") {
        tent.drawDimmedIndicator();
    }
}

void Screen::cancelConfirmationScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "cancelConfirmationScreen";

    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(3);
    tft.print("Really Terminate?");

    Button terminateYesBtn("terminateYesBtn", 70, 120, 180, 28, "Yes", 78, 7);
    buttons[0] = terminateYesBtn;

    Button terminateNoBtn("terminateNoBtn", 70, 180, 180, 38, "No", 75, 8);
    buttons[1] = terminateNoBtn;
}

void Screen::timerScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "timerScreen";

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

    Button timerUpBtn("timerUpBtn", 240, 50, 40, 40, "", 0, 0);
    buttons[0] = timerUpBtn;

    Button timerDownBtn("timerDownBtn", 240, 130, 40, 40, "", 0, 0);
    buttons[1] = timerDownBtn;

    Button timerOkBtn("timerOkBtn", 20, 180, 250, 38, "Ok", 110, 8);
    buttons[2] = timerOkBtn;

    if (tent.getGrowLightStatus() == "LOW") {
        tent.drawDimmedIndicator();
    }
}

void Screen::wifiScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "wifiScreen";

    Button wifiOnBtn("wifiOnBtn", 20, 40, 250, 38, "On", 110, 8);
    buttons[0] = wifiOnBtn;

    Button wifiOffBtn("wifiOffBtn", 20, 100, 250, 38, "Off", 110, 8);
    buttons[1] = wifiOffBtn;

    Button wifiOkBtn("wifiOkBtn", 20, 180, 250, 38, "Ok", 110, 8);
    buttons[2] = wifiOkBtn;
}

void Screen::wifiSetupScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "wifiScreen";

    tft.drawBitmap(18, 5, iconWifi_24x24, 24, 24, ILI9341_WHITE);

    tft.setCursor(50, 10);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);
    tft.print("WiFi Setup");

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setCursor(20, 50);
    tft.print("Use a SmartPhone, Tablet or Computer");
    tft.setCursor(20, 70);
    tft.print("Go to WiFi settings, scan for Networks.");
    tft.setCursor(20, 90);
    tft.print("Connect to the Network called TomatoTent-xxxx.");
    tft.setCursor(20, 110);
    tft.print("Open a Web Browser (Safari, Chrome, IE).");
    tft.setCursor(20, 130);
    tft.print("Navigate to http://192.168.0.1");
    tft.setCursor(20, 150);
    tft.print("Follow the instructions there.");

    tft.setCursor(20, 200);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_GREEN);
    tft.print("community.tomatotent.de");
}
void Screen::fanScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "fanScreen";

    tft.drawBitmap(20, 4, fan_36, 36, 36, ILI9341_WHITE);

    tft.setCursor(60, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Speed");

    Button fanAutoBtn("fanAutoBtn", 20, 65, 150, 38, "Automatic", 20, 11);
    buttons[0] = fanAutoBtn;

    Button fanManualBtn("fanManualBtn", 20, 115, 150, 38, "Manual", 40, 11);
    buttons[1] = fanManualBtn;

    Button fanUpBtn("fanUpBtn", 240, 50, 40, 40, "", 0, 0);
    buttons[2] = fanUpBtn;

    Button fanDownBtn("fanDownBtn", 240, 110, 40, 40, "", 0, 0);
    buttons[3] = fanDownBtn;

    Button fanOkBtn("fanOkBtn", 20, 180, 250, 38, "Ok", 110, 8);
    buttons[4] = fanOkBtn;

    systemStatus.drawFanSpeed();
}

void Screen::tempUnitScreen(Button* buttons, String& currentScreen)
{

    this->clear(buttons);
    currentScreen = "tempUnitScreen";

    tft.drawBitmap(20, 4, thermometer_36, 36, 36, ILI9341_WHITE);

    tft.setCursor(60, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Temperature Unit");

    Button tempFahrenheitBtn("tempFahrenheitBtn", 80, 70, 150, 38, "Fahrenheit", 20, 11);
    buttons[0] = tempFahrenheitBtn;

    Button tempCelsiusBtn("tempCelsiusBtn", 80, 140, 150, 38, "Celsius", 40, 11);
    buttons[1] = tempCelsiusBtn;
}

void Screen::clear(Button* buttons)
{

    Button blankBtn("blankBtn", -1, -1, -1, -1, "", -1, -1);

    tft.fillScreen(ILI9341_BLACK);

    buttons[0] = blankBtn;
    buttons[1] = blankBtn;
    buttons[2] = blankBtn;
    buttons[3] = blankBtn;
    buttons[4] = blankBtn;
    buttons[5] = blankBtn;
}

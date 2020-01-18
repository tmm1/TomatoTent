#include "screen_manager.h"
#include "systemStatus.h"
#include "tent.h"
#include "screens/cancel.h"
#include "screens/cancel_confirm.h"

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;
extern float fanSpeed;
extern float fanSpeedPercent;

void ScreenManager::homeScreen()
{
    current = new HomeScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::growStartedScreen()
{
    this->clearButtons();
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

void ScreenManager::cancelScreen()
{
    current = new CancelScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::cancelConfirmationScreen()
{
    current = new CancelConfirmScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::timerScreen()
{
    this->clearButtons();
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

void ScreenManager::wifiScreen()
{
    this->clearButtons();
    currentScreen = "wifiScreen";

    Button wifiOnBtn("wifiOnBtn", 20, 40, 250, 38, "On", 110, 8);
    buttons[0] = wifiOnBtn;

    Button wifiOffBtn("wifiOffBtn", 20, 100, 250, 38, "Off", 110, 8);
    buttons[1] = wifiOffBtn;

    Button wifiOkBtn("wifiOkBtn", 20, 180, 250, 38, "Ok", 110, 8);
    buttons[2] = wifiOkBtn;
}

void ScreenManager::wifiSetupScreen()
{
    this->clearButtons();
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

void ScreenManager::fanScreen()
{
    this->clearButtons();
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

void ScreenManager::tempUnitScreen()
{
    this->clearButtons();
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

void ScreenManager::renderButtons(bool forced)
{
    if (!current)
        return;
    current->renderButtons(forced);
}

void ScreenManager::clearButtons()
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

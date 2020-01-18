#include "screen_manager.h"
#include "systemStatus.h"
#include "tent.h"
#include "screens/cancel.h"
#include "screens/cancel_confirm.h"
#include "screens/timer.h"
#include "screens/fan.h"
#include "screens/temp_unit.h"
#include "screens/grow_started.h"
#include "screens/wifi_splash.h"

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
    current = new GrowStartedScreen();
    currentScreen = current->getName();
    current->render();
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
    current = new TimerScreen();
    currentScreen = current->getName();
    current->render();
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

void ScreenManager::wifiSplashScreen()
{
    current = new WifiSplashScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::fanScreen()
{
    current = new FanScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::tempUnitScreen()
{
    current = new TempUnitScreen();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::renderButtons(bool forced)
{
    if (current) {
        current->renderButtons(forced);
    }
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

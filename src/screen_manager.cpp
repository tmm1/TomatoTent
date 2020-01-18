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
#include "screens/wifi.h"

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;
extern float fanSpeed;
extern float fanSpeedPercent;

void ScreenManager::homeScreen()
{
    current = std::make_unique<HomeScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::growStartedScreen()
{
    current = std::make_unique<GrowStartedScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::cancelScreen()
{
    current = std::make_unique<CancelScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::cancelConfirmationScreen()
{
    current = std::make_unique<CancelConfirmScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::timerScreen()
{
    current = std::make_unique<TimerScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::wifiScreen()
{
    current = std::make_unique<WifiScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::wifiSplashScreen()
{
    current = std::make_unique<WifiSplashScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::fanScreen()
{
    current = std::make_unique<FanScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::tempUnitScreen()
{
    current = std::make_unique<TempUnitScreen>();
    currentScreen = current->getName();
    current->render();
}

void ScreenManager::renderButtons(bool forced)
{
    if (current) {
        current->renderButtons(forced);
    }
}

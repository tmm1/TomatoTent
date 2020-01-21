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
    current->render();
}

void ScreenManager::growStartedScreen()
{
    current = std::make_unique<GrowStartedScreen>();
    current->render();
}

void ScreenManager::cancelScreen()
{
    current = std::make_unique<CancelScreen>();
    current->render();
}

void ScreenManager::cancelConfirmationScreen()
{
    current = std::make_unique<CancelConfirmScreen>();
    current->render();
}

void ScreenManager::timerScreen()
{
    current = std::make_unique<TimerScreen>();
    current->render();
}

void ScreenManager::wifiScreen()
{
    current = std::make_unique<WifiScreen>();
    current->render();
}

void ScreenManager::wifiSplashScreen()
{
    current = std::make_unique<WifiSplashScreen>();
    current->render();
}

void ScreenManager::fanScreen()
{
    current = std::make_unique<FanScreen>();
    current->render();
}

void ScreenManager::tempUnitScreen()
{
    current = std::make_unique<TempUnitScreen>();
    current->render();
}

void ScreenManager::renderButtons(bool forced)
{
    if (current) {
        current->renderButtons(forced);
    }
}

void ScreenManager::markDirty(dirtyMarker m)
{
    dirtyMarkers |= (int)m;
}

bool ScreenManager::wasDirty(dirtyMarker m)
{
    bool ret = dirtyMarkers & m;
    dirtyMarkers &= ~m;
    return ret;
}

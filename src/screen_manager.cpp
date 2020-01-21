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

void ScreenManager::render()
{
    if (current) {
        markNeedsRedraw(DIMMED);
        current->render();
    }
}

void ScreenManager::homeScreen()
{
    current.reset(new HomeScreen());
    render();
}

void ScreenManager::growStartedScreen()
{
    current.reset(new GrowStartedScreen());
    render();
}

void ScreenManager::cancelScreen()
{
    current.reset(new CancelScreen());
    render();
}

void ScreenManager::cancelConfirmationScreen()
{
    current.reset(new CancelConfirmScreen());
    render();
}

void ScreenManager::timerScreen()
{
    current.reset(new TimerScreen());
    render();
}

void ScreenManager::wifiScreen()
{
    current.reset(new WifiScreen());
    render();
}

void ScreenManager::wifiSplashScreen()
{
    current.reset(new WifiSplashScreen());
    render();
}

void ScreenManager::fanScreen()
{
    current.reset(new FanScreen());
    render();
}

void ScreenManager::tempUnitScreen()
{
    current.reset(new TempUnitScreen());
    render();
}

void ScreenManager::renderButtons(bool forced)
{
    if (current) {
        current->renderButtons(forced);
    }
}

void ScreenManager::markNeedsRedraw(redrawMarker m)
{
    redrawMarkers |= (int)m;
}

bool ScreenManager::wasNeedsRedraw(redrawMarker m)
{
    bool ret = redrawMarkers & m;
    redrawMarkers &= ~m;
    return ret;
}

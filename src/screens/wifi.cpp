#include "wifi.h"
#include "icons.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
extern Tent tent;

void WifiScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    buttons.push_back(Button("wifiOnBtn", 20, 40, 250, 38, "On", 110, 8));
    buttons.push_back(Button("wifiOffBtn", 20, 100, 250, 38, "Off", 110, 8));
    buttons.push_back(Button("wifiOkBtn", 20, 180, 250, 38, "Ok", 110, 8));

    renderButtons(true);
}

void WifiScreen::renderButton(Button& btn)
{
    if (btn.getName() == "wifiOnBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);

    } else if (btn.getName() == "wifiOffBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);

    } else if (btn.getName() == "wifiOkBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);
    }
}

void WifiScreen::renderButtonPressed(Button& btn)
{
}

void WifiScreen::handleButton(Button& btn)
{
    if (btn.getName() == "wifiOnBtn") {
        Particle.connect();
        screenManager.homeScreen();

    } else if (btn.getName() == "wifiOffBtn") {
        Particle.disconnect();
        WiFi.off();
        screenManager.homeScreen();

    } else if (btn.getName() == "wifiOkBtn") {
        screenManager.homeScreen();
    }
}

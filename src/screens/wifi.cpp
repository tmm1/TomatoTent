#include "wifi.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
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
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "wifiOnBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);

    } else if (btn.getName() == "wifiOffBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);

    } else if (btn.getName() == "wifiOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);
        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(buttonText);
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

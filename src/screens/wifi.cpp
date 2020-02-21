#include "wifi.h"
#include "icons.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
extern Tent tent;
extern uint16_t __system_product_version;

void WifiScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.drawBitmap(20, 14, iconWifi_24x24, 24, 24, ILI9341_WHITE);

    tft.setCursor(60, 18);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("WiFi");

    tft.setCursor(60, 38);
    tft.setTextSize(1);
    if (WiFi.ready())
        tft.print(String::format("Connected: %s (%d%%)", WiFi.SSID(), (int)WiFi.RSSI().getStrength()));
    else
        tft.print("Disconnected");

    tft.setCursor(18, 57);
    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.print("https://my.tomatotent.com/graphs/");

    tft.setCursor(18, 71);
    tft.setTextSize(2);
    tft.print(System.deviceID().c_str());

    tft.setCursor(55, 210);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_LIGHTGREY);

    tft.print(String::format("TomatoTent v%d", __system_product_version));

    if (WiFi.ready())
        tft.print(String::format(" on http://%s", WiFi.localIP().toString().c_str()));

    buttons.push_back(Button("wifiOnBtn", 30, 100, 125, 38, "On", 50, 8));
    buttons.push_back(Button("wifiOffBtn", 160, 100, 125, 38, "Off", 40, 8));
    buttons.push_back(Button("wifiOkBtn", 30, 160, 255, 38, "Back", 100, 8));

    renderButtons(true);
}

void WifiScreen::renderButton(Button& btn)
{
    if (btn.getName() == "wifiOnBtn") {
        drawButton(btn, WiFi.ready() ? ILI9341_OLIVE : ILI9341_BLACK, 3);

    } else if (btn.getName() == "wifiOffBtn") {
        drawButton(btn, !WiFi.ready() ? ILI9341_OLIVE : ILI9341_BLACK, 3);

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

#include "temp_unit.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;

void TempUnitScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.drawBitmap(20, 4, thermometer_36, 36, 36, ILI9341_WHITE);

    tft.setCursor(60, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Temperature Unit");

    buttons.push_back(Button("tempFahrenheitBtn", 80, 70, 150, 38, "Fahrenheit", 20, 11));
    buttons.push_back(Button("tempCelsiusBtn", 80, 140, 150, 38, "Celsius", 40, 11));

    renderButtons(true);
}

void TempUnitScreen::renderButton(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "tempFahrenheitBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (systemStatus.getTempUnit() == 'F') {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);

    } else if (btn.getName() == "tempCelsiusBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        if (systemStatus.getTempUnit() == 'C') {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);
        } else {
            tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_BLACK);
        }
        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(buttonText);
    }
}

void TempUnitScreen::renderButtonPressed(Button& btn)
{
}

void TempUnitScreen::handleButton(Button& btn)
{
    if (btn.getName() == "tempFahrenheitBtn") {
        systemStatus.setTempUnit('F');
        renderButtons(true);
        screenManager.homeScreen();
        tent.check_temperature(systemStatus.getTempUnit());

    } else if (btn.getName() == "tempCelsiusBtn") {
        systemStatus.setTempUnit('C');
        renderButtons(true);
        screenManager.homeScreen();
        tent.check_temperature(systemStatus.getTempUnit());
    }
}

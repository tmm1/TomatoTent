#include "temp_unit.h"
#include "icons.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
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
    if (btn.getName() == "tempFahrenheitBtn") {
        drawButton(btn, tent.state.getTempUnit() == 'F' ? ILI9341_OLIVE : ILI9341_BLACK, 2);

    } else if (btn.getName() == "tempCelsiusBtn") {
        drawButton(btn, tent.state.getTempUnit() == 'C' ? ILI9341_OLIVE : ILI9341_BLACK, 2);
    }
}

void TempUnitScreen::renderButtonPressed(Button& btn)
{
}

void TempUnitScreen::handleButton(Button& btn)
{
    if (btn.getName() == "tempFahrenheitBtn") {
        tent.state.setTempUnit('F');
        renderButtons(true);
        screenManager.homeScreen();
        tent.checkTemperature();

    } else if (btn.getName() == "tempCelsiusBtn") {
        tent.state.setTempUnit('C');
        renderButtons(true);
        screenManager.homeScreen();
        tent.checkTemperature();
    }
}

#include "fan.h"
#include "icons.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
extern Tent tent;

void FanScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.drawBitmap(20, 4, fan_36, 36, 36, ILI9341_WHITE);

    tft.setCursor(60, 8);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Speed");

    buttons.push_back(Button("fanAutoBtn", 20, 65, 150, 38, "Automatic", 20, 11));
    buttons.push_back(Button("fanManualBtn", 20, 115, 150, 38, "Manual", 40, 11));
    buttons.push_back(Button("fanUpBtn", 240, 50, 40, 40, "", 0, 0));
    buttons.push_back(Button("fanDownBtn", 240, 110, 40, 40, "", 0, 0));
    buttons.push_back(Button("fanOkBtn", 20, 180, 250, 38, "Ok", 110, 8));

    renderButtons(true);
    drawFanStatus();
}

void FanScreen::update()
{
    if (screenManager.wasNeedsRedraw(FAN))
        drawFanStatus();

    Screen::update();
}

void FanScreen::renderButton(Button& btn)
{
    if (btn.getName() == "fanAutoBtn") {
        drawButton(btn, tent.state.getFanAutoMode() ? ILI9341_OLIVE : ILI9341_BLACK, 2);

    } else if (btn.getName() == "fanManualBtn") {
        drawButton(btn, !tent.state.getFanAutoMode() ? ILI9341_OLIVE : ILI9341_BLACK, 2);

    } else if (btn.getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_RED);
        tft.drawTriangle(240, 95, 260, 55, 280, 95, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_RED);
        tft.drawTriangle(240, 110, 260, 150, 280, 110, ILI9341_LIGHTGREY);

    } else if (btn.getName() == "fanOkBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);
    }
}

void FanScreen::renderButtonPressed(Button& btn)
{
    if (btn.getName() == "fanUpBtn") {
        tft.fillTriangle(240, 95, 260, 55, 280, 95, ILI9341_WHITE);

    } else if (btn.getName() == "fanDownBtn") {
        tft.fillTriangle(240, 110, 260, 150, 280, 110, ILI9341_WHITE);
    }
}

void FanScreen::handleButton(Button& btn)
{
    if (btn.getName() == "fanAutoBtn") {
        tent.state.setFanAutoMode(true);
        renderButton(buttons[0]);
        renderButton(buttons[1]);
        tent.adjustFan();

    } else if (btn.getName() == "fanManualBtn") {
        tent.state.setFanAutoMode(false);
        renderButton(buttons[0]);
        renderButton(buttons[1]);
        tent.adjustFan();

    } else if (btn.getName() == "fanUpBtn") {
        float fanSpeedPercent = tent.state.getFanSpeed();
        tent.state.setFanAutoMode(false);
        renderButton(buttons[0]);
        renderButton(buttons[1]);

        if (fanSpeedPercent < 100) {
            fanSpeedPercent += 5;
            tent.state.setFanSpeed(fanSpeedPercent);
            tent.adjustFan();
        }

    } else if (btn.getName() == "fanDownBtn") {
        float fanSpeedPercent = tent.state.getFanSpeed();
        tent.state.setFanAutoMode(false);
        renderButton(buttons[0]);
        renderButton(buttons[1]);

        if (fanSpeedPercent > 0) {
            fanSpeedPercent -= 5;
            tent.state.setFanSpeed(fanSpeedPercent);
            tent.adjustFan();
        }

    } else if (btn.getName() == "fanOkBtn") {
        screenManager.homeScreen();
    }
}

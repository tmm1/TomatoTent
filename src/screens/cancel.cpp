#include "cancel.h"
#include "icons.h"
#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;
extern Tent tent;

void CancelScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    drawDay();

    buttons.push_back(Button("dayUpBtn", 50, 35, 40, 40, "", 0, 0));
    buttons.push_back(Button("dayDownBtn", 50, 135, 40, 40, "", 0, 0));

    buttons.push_back(Button("cancelScreenOkBtn", 160, 70, 145, 38, "Ok", 60, 8));
    buttons.push_back(Button("terminateBtn", 160, 130, 145, 28, "End Grow", 30, 7));

    renderButtons(true);
}

void CancelScreen::drawDay()
{
    tft.fillRect(20, 90, 135, 30, ILI9341_BLACK);
    tft.setCursor(20, 90);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.print("Day " + String(tent.state.getDayCount()));
}

void CancelScreen::renderButton(Button& btn)
{
    if (btn.getName() == "terminateBtn") {
        drawButton(btn, ILI9341_RED, 2);

    } else if (btn.getName() == "cancelScreenOkBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);

    } else if (btn.getName() == "dayUpBtn") {
        drawButtonTriangleUp(btn, ILI9341_YELLOW);

    } else if (btn.getName() == "dayDownBtn") {
        drawButtonTriangleDown(btn, ILI9341_YELLOW);
    }
}

void CancelScreen::renderButtonPressed(Button& btn)
{
    if (btn.getName() == "dayUpBtn") {
        drawButtonTriangleUp(btn, ILI9341_WHITE);

    } else if (btn.getName() == "dayDownBtn") {
        drawButtonTriangleDown(btn, ILI9341_WHITE);
    }
}

void CancelScreen::handleButton(Button& btn)
{
    if (btn.getName() == "terminateBtn") {
        screenManager.cancelConfirmationScreen();

    } else if (btn.getName() == "cancelScreenOkBtn") {
        screenManager.homeScreen();

    } else if (btn.getName() == "dayUpBtn") {
        tent.state.setDayCount(tent.state.getDayCount() + 1);
        drawDay();

    } else if (btn.getName() == "dayDownBtn") {
        tent.state.setDayCount(tent.state.getDayCount() - 1);
        drawDay();
    }
}

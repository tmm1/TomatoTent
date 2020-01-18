#include "cancel_confirm.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;
extern Timer minutelyTicker, draw_temp_home;

void CancelConfirmScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(3);
    tft.print("Really Terminate?");

    buttons.push_back(Button("terminateYesBtn", 70, 120, 180, 28, "Yes", 78, 7));
    buttons.push_back(Button("terminateNoBtn", 70, 180, 180, 38, "No", 75, 8));

    renderButtons(true);
}

void CancelConfirmScreen::renderButton(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "terminateYesBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_RED);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);

        tft.print(buttonText);

    } else if (btn.getName() == "terminateNoBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);

        tft.print(buttonText);
    }
}

void CancelConfirmScreen::renderButtonPressed(Button& btn)
{
}

void CancelConfirmScreen::handleButton(Button& btn)
{
    if (btn.getName() == "terminateYesBtn") {
        tent.growLight("OFF");
        draw_temp_home.stop();
        tent.fan("OFF");
        minutelyTicker.stop();

        systemStatus.init();
        screenManager.homeScreen();

    } else if (btn.getName() == "terminateNoBtn") {
        screenManager.homeScreen();
    }
}

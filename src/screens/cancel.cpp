#include "cancel.h"
#include "systemStatus.h"
#include "icons.h"
#include "tent.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;
extern SystemStatus systemStatus;
extern Tent tent;

void CancelScreen::render()
{
    tft.fillScreen(ILI9341_BLACK);

    tft.setCursor(20, 70);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Your grow is " + String(systemStatus.getDayCount() - 1) + " days old.");

    tft.setCursor(20, 100);
    tft.print("Most Cannabis is ready");

    tft.setCursor(20, 120);
    tft.print("after about 110 days.");

    /* Include if you want to know your free memory */
    //tft.setTextSize(1);
    //tft.setCursor(50,140);
    //uint32_t freemem = System.freeMemory();
    //tft.print(freemem);

    buttons.push_back(Button("cancelScreenOkBtn", 20, 180, 250, 38, "Ok", 110, 8));
    buttons.push_back(Button("terminateBtn", 120, 10, 185, 28, "Terminate Grow", 10, 7));
    renderButtons(true);

    if (tent.getGrowLightStatus() == "LOW") {
        tent.drawDimmedIndicator();
    }
}

void CancelScreen::renderButton(Button& btn)
{
    uint16_t x0 = btn.x0, y0 = btn.y0, w = btn.w, h = btn.h, textOffsetLeft = btn.textOffsetLeft, textOffsetTop = btn.textOffsetTop;
    String buttonText = btn.buttonText;

    if (btn.getName() == "terminateBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_RED);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);

        tft.print(buttonText);

    } else if (btn.getName() == "cancelScreenOkBtn") {
        tft.drawRect(x0, y0, w, h, ILI9341_WHITE);

        tft.fillRect(x0 + 1, y0 + 1, w - 2, h - 2, ILI9341_OLIVE);

        tft.setCursor(x0 + textOffsetLeft, y0 + textOffsetTop);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(3);

        tft.print(buttonText);
    }
}

void CancelScreen::renderButtonPressed(Button& btn)
{
}

void CancelScreen::handleButton(Button& btn)
{
    if (btn.getName() == "terminateBtn") {
        screenManager.cancelConfirmationScreen();

    } else if (btn.getName() == "cancelScreenOkBtn") {
        screenManager.homeScreen();
    }
}

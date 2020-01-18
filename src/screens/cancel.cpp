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
    if (btn.getName() == "terminateBtn") {
        drawButton(btn, ILI9341_RED, 2);

    } else if (btn.getName() == "cancelScreenOkBtn") {
        drawButton(btn, ILI9341_OLIVE, 3);
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

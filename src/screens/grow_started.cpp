#include "grow_started.h"
#include "icons.h"
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;

void GrowStartedScreen::render()
{
    tft.fillScreen(ILI9341_OLIVE);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);

    tft.setCursor(50, 30);
    tft.print("Your Grow");
    tft.setCursor(60, 90);
    tft.print("has started!");

    tft.drawBitmap(124, 160, plant_filled_72x72, 72, 72, ILI9341_WHITE);
}

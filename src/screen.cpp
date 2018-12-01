#include "screen.h"


  Screen::Screen() {}
  
void Screen::homeScreen(Button *buttons) {
    
    tft.fillScreen(ILI9341_BLACK);
    Button startGrowBtn("startGrowBtn", 20,180,250,38, "Start a Grow",18,8);
    buttons[0] = startGrowBtn;
    startGrowBtn.render();
 
  }

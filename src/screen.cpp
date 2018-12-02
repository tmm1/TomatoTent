#include "screen.h"


Screen::Screen() {}
  
void Screen::homeScreen(Button *buttons) {
  
    this->clear();
      
    if(systemStatus.getDayCount() == -1) {
      
      Button startGrowBtn("startGrowBtn", 20,180,250,38, "Start a Grow",18,8);
      buttons[0] = startGrowBtn;
      startGrowBtn.render();
      
    } else { // a grow is in progress
      
      tft.drawRect(20,180,250,38,ILI9341_BLACK);

      tft.setCursor(20,180);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(3);

      tft.print("Day "+String(systemStatus.getDayCount()));
      
      Button cancelBtn("cancelBtn", 20,180,250,38, "",18,8);
      buttons[1] = cancelBtn;
      cancelBtn.render();

  }
}

void Screen::cancelScreen(Button *buttons) {
  this->clear();
  
}

void Screen::clear() {
  tft.fillScreen(ILI9341_BLACK);
}

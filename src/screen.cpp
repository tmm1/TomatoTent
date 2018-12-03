#include "screen.h"


Screen::Screen() {}
  
void Screen::homeScreen(Button *buttons) {
  
    this->clear(buttons);
      
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
      
      Button dayCounterBtn("dayCounterBtn", 20,180,250,38, "",18,8);
      buttons[1] = dayCounterBtn;
      dayCounterBtn.render();

  }
}

void Screen::cancelScreen(Button *buttons) {

  this->clear(buttons);
  
  tft.setCursor(20,70);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Your grow is "+String(systemStatus.getDayCount()-1)+" days old.");
  
  tft.setCursor(20,100);
  tft.print("Most Cannabis is ready");
  
  tft.setCursor(20,120);
  tft.print("after about 110 days.");

  Button cancelScreenOkBtn("cancelScreenOkBtn", 20,180,250,38, "Ok",28,8);
  buttons[0] = cancelScreenOkBtn;
  cancelScreenOkBtn.render();
}

void Screen::clear(Button *buttons) {
  
  Button blankBtn("blankBtn",-1,-1,-1,-1, "",-1,-1);
   
  tft.fillScreen(ILI9341_BLACK);
    
  buttons[0] = blankBtn;
  buttons[1] = blankBtn;
  buttons[2] = blankBtn;
  buttons[3] = blankBtn;
  buttons[4] = blankBtn;

}

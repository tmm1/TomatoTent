#include "screen.h"


Screen::Screen() {}
  
void Screen::homeScreen(Button *buttons, String &currentScreen) {
  
    this->clear(buttons);
    currentScreen = "homeScreen";
      
    if(systemStatus.getDayCount() == -1) {
      
      Button startGrowBtn("startGrowBtn", 20,180,250,38, "Start a Grow",18,8);
      buttons[0] = startGrowBtn;
      startGrowBtn.render();
      
    } else { // a grow is in progress
      
      tft.drawRect(20,180,250,38,ILI9341_BLACK);

      systemStatus.drawDayCounter();
      
      Button dayCounterBtn("dayCounterBtn", 20,180,250,38, "",18,8);
      buttons[1] = dayCounterBtn;
      dayCounterBtn.render();
      
      Button timerBtn("timerBtn", 10,10,115,25, "",18,8);
      buttons[2] = timerBtn;
      timerBtn.render();
      
      systemStatus.drawTimerStatus();
      
      if(tent.getGrowLightStatus() == "LOW") {
        tent.drawDimmedIndicator();  
      }
      
  }
    tent.drawStats();
  
}

void Screen::cancelScreen(Button *buttons, String &currentScreen) {

  this->clear(buttons);
  currentScreen = "cancelScreen";
  
  tft.setCursor(20,70);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Your grow is "+String(systemStatus.getDayCount()-1)+" days old.");
  
  tft.setCursor(20,100);
  tft.print("Most Cannabis is ready");
  
  tft.setCursor(20,120);
  tft.print("after about 110 days.");

  Button cancelScreenOkBtn("cancelScreenOkBtn", 20,180,250,38, "Ok",110,8);
  buttons[0] = cancelScreenOkBtn;
  cancelScreenOkBtn.render();
  
  Button terminateBtn("terminateBtn", 120,10,185,28, "Terminate Grow",10,7);
  buttons[1] = terminateBtn;
  terminateBtn.render();  
  
  if(tent.getGrowLightStatus() == "LOW") {
    tent.drawDimmedIndicator();  
  }
}


void Screen::cancelConfirmationScreen(Button *buttons, String &currentScreen) {
  
  this->clear(buttons);
  currentScreen = "cancelConfirmationScreen";
  
  tft.setCursor(10,50);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.print("Really Terminate?");
  
  Button terminateYesBtn("terminateYesBtn", 70,120,180,28, "Yes",78,7);
  buttons[0] = terminateYesBtn;
  terminateYesBtn.render();
  
  Button terminateNoBtn("terminateNoBtn", 70,180,180,38, "No",75,8);
  buttons[1] = terminateNoBtn;
  terminateNoBtn.render();  
}

void Screen::timerScreen(Button *buttons, String &currentScreen) {
  
  this->clear(buttons);
  currentScreen = "timerScreen";
  
  tft.setCursor(20,8);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.print("Light Timer");
  
  int dayDuration = systemStatus.getDayDuration();
  
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10,70);
  tft.print(String(dayDuration/60)+" Hours ON");
  
  int nightDuration = (24*60)-dayDuration;
  tft.setCursor(10,140);
  tft.print(String(nightDuration/60)+" Hours OFF");  
  
  
  Button timerUpBtn("timerUpBtn", 240,50,40,40, "",0,0);
  buttons[0] = timerUpBtn;
  timerUpBtn.render();
  
  Button timerDownBtn("timerDownBtn", 240,130,40,40, "",0,0);
  buttons[1] = timerDownBtn;
  timerDownBtn.render();
  
  Button timerOkBtn("timerOkBtn", 20,180,250,38, "Ok",110,8);
  buttons[2] = timerOkBtn;
  timerOkBtn.render();
  
  if(tent.getGrowLightStatus() == "LOW") {
    tent.drawDimmedIndicator();  
  }  
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

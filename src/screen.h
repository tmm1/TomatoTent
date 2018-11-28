#include "application.h"
#include "tent.h"

Button buttons[1];

class Screen {

  public:
  
  Screen() {

  }
  
void homeScreen() {
    
    tft.fillScreen(ILI9341_BLACK);
    Button startGrowBtn("startGrowBtn", 20,180,250,38, "Start a Grow",18,8);
    buttons[0] = startGrowBtn;
    startGrowBtn.render();
 
  }
  
  
void countMinute() {
 
  int minutesInPhotoperiod = systemStatus.getMinutesInPhotoperiod()+1;
  Serial.println(minutesInPhotoperiod);
  
  if(systemStatus.isDay()) {
    if(systemStatus.getMinutesInPhotoperiod() >= systemStatus.getDayDuration()) {   //day is over
      //tent.growLight("OFF");
      systemStatus.setIsDay(false);
      systemStatus.setMinutesInPhotoperiod(0);
    };     
  } else {
    if(systemStatus.getMinutesInPhotoperiod() > ((24*60) - systemStatus.getDayDuration())) {   //night is over
      //tent.growLight("HIGH");
      systemStatus.setIsDay(true);
      systemStatus.setMinutesInPhotoperiod(0);
    }; 
  }

    //EEPROM.put(0, systemStatus);
  
       
    int hoursLeft = floor((systemStatus.getDayDuration() - systemStatus.getMinutesInPhotoperiod()) / 60);
    int minutesLeft = (systemStatus.getDayDuration() - systemStatus.getMinutesInPhotoperiod()) % 60;
  
    tft.fillRect(10,10,140,18,ILI9341_BLACK);
    
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);

    tft.print("* "+String(hoursLeft)+":"+String(minutesLeft));  

}
   
};
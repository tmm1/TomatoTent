#include <systemStatus.h>

SystemStatus::SystemStatus() {
    
 Status status;
 EEPROM.get(0, status);
  
 Serial.println(status.dayCounter);   
}
  
int SystemStatus::getDayCounter() {
 // Serial.println(status.dayCounter);
  //return status.dayCounter;
  return 1;
}
bool SystemStatus::isDay() {
  return true;
}
int SystemStatus::getMinutesInPhotoperiod() {
  return 1;
}
int SystemStatus::getDayDuration() {
  return 1;
}
void SystemStatus::setIsDay(bool isDay) {
   
}
void SystemStatus::setMinutesInPhotoperiod(int minutesInPhotoperiod) {
  
}

  
void SystemStatus::countMinute() {
 
  int minutesInPhotoperiod = this->getMinutesInPhotoperiod()+1;
  
  if(this->isDay()) {
    if(this->getMinutesInPhotoperiod() >= this->getDayDuration()) {   //day is over
      //tent.growLight("OFF");
      this->setIsDay(false);
      this->setMinutesInPhotoperiod(0);
    };     
  } else {
    if(this->getMinutesInPhotoperiod() > ((24*60) - this->getDayDuration())) {   //night is over
      //tent.growLight("HIGH");
      this->setIsDay(true);
      this->setMinutesInPhotoperiod(0);
    }; 
  }

    //EEPROM.put(0, systemStatus);
  
       
    int hoursLeft = (this->getDayDuration() - this->getMinutesInPhotoperiod()) / 60;
    int minutesLeft = (this->getDayDuration() - this->getMinutesInPhotoperiod()) % 60;
  
    tft.fillRect(10,10,140,18,ILI9341_BLACK);
    
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);

    tft.print("* "+String(hoursLeft)+":"+String(minutesLeft));  

}


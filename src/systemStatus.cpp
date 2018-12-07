#include <systemStatus.h>

SystemStatus::SystemStatus() {
    
 EEPROM.get(0, status);
  
}
  
int SystemStatus::getDayCount() {
  return this->status.dayCounter;
}
bool SystemStatus::isDay() {
  return this->status.isDay;
}
int SystemStatus::getMinutesInPhotoperiod() {
  return this->status.minutesInPhotoperiod;
}
int SystemStatus::getDayDuration() {
  return this->status.dayDuration;
}

void SystemStatus::setDayDuration(int dayDuration) {
  this->status.dayDuration = dayDuration;
  this->save();
}

void SystemStatus::setDayCount(int dayCount) {
  this->status.dayCounter = dayCount;
  this->save();
}
void SystemStatus::setIsDay(bool isDay) {
  this->status.isDay = isDay;
  this->save();
}
void SystemStatus::setMinutesInPhotoperiod(int minutesInPhotoperiod) {
  this->status.minutesInPhotoperiod = minutesInPhotoperiod;
  this->save();
}  
void SystemStatus::countMinute() {
 
  this->setMinutesInPhotoperiod(this->getMinutesInPhotoperiod()+1);
  
  if(this->isDay()) {
    if(this->getMinutesInPhotoperiod() >= this->getDayDuration()) {   //day is over
      tent.growLight("OFF");
      this->setIsDay(false);
      this->setMinutesInPhotoperiod(0);
    }
      
    
  } else {
 
    if(this->getMinutesInPhotoperiod() > ((24*60) - this->getDayDuration())) {   //night is over
      
      this->setDayCount(this->getDayCount()+1);
      
      this->drawDayCounter();
    
      tent.growLight("HIGH");
      this->setIsDay(true);
      this->setMinutesInPhotoperiod(0);
    }   
   
  }
  
  this->save();
  
  if(currentScreen == "homeScreen") {     
    this->drawTimerStatus();
  }
}


void SystemStatus::drawTimerStatus() {
  
    int hoursLeft;
    int minutesLeft;
  
    if(this->isDay()) {
      tft.setTextColor(ILI9341_YELLOW);
      hoursLeft = floor((this->getDayDuration() - this->getMinutesInPhotoperiod()) / 60);
      minutesLeft = (this->getDayDuration() - this->getMinutesInPhotoperiod()) % 60; 
    } else {
      tft.setTextColor(ILI9341_BLUE);
      hoursLeft = floor((((24*60)-this->getDayDuration()) - this->getMinutesInPhotoperiod()) / 60);
      minutesLeft = (((24*60)-this->getDayDuration()) - this->getMinutesInPhotoperiod()) % 60;
    }
  
    if(hoursLeft < 0 || minutesLeft < 0) {
      
      this->countMinute();  
      
    } else {
  
      tft.fillRect(10,10,115,25,ILI9341_BLACK);

      tft.setCursor(10, 10);
      tft.setTextSize(2);

      tft.print("* "+String(hoursLeft));
      tft.setTextSize(1);
      tft.print("hrs ");
      tft.setTextSize(2);
      tft.print(""+String(minutesLeft));
      tft.setTextSize(1);
      tft.print("min");

      tft.setCursor(38, 27);
      tft.setTextSize(1);
      if(this->isDay()) {
        tft.print("until sunset");
      } else {
        tft.print("until sunrise");
      }
    }
    
}

void SystemStatus::drawDayCounter() {
      tft.setCursor(70,180);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(3);

      tft.print("Day "+String(this->getDayCount()));
}


void SystemStatus::save() {
  EEPROM.put(0, this->status);
}

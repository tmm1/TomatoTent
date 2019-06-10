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
  
      tft.fillRect(5,5,137,37,ILI9341_BLACK);

      tft.setCursor(50, 10);
      tft.setTextSize(2);

      tft.print(String(hoursLeft));
      tft.setTextSize(1);
      tft.print("hrs ");
      tft.setTextSize(2);
      tft.print(""+String(minutesLeft));
      tft.setTextSize(1);
      tft.print("min");

      tft.setCursor(53, 31);
      tft.setTextSize(1);
      if(this->isDay()) {
        tft.drawBitmap(7, 5, sun_36, 36, 36, ILI9341_YELLOW);
        tft.print("until sunset");
      } else {
        tft.drawBitmap(7, 5, moon_and_stars_36, 36, 36, ILI9341_BLUE);
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

int SystemStatus::getFanMode() {
  return this->status.fanMode;
}

void SystemStatus::setFanMode(int fanMode) {
  this->status.fanMode = fanMode;
  this->save();
}


void SystemStatus::check_fan() {
      
      if(this->status.fanMode != -1) {
        
        fanSpeed = map(this->status.fanMode, 0, 100, 0, 255);
        
        analogWrite(FAN_SPEED_PIN,fanSpeed, 25000);

      } else {

        fanSpeed = FAN_SPEED_MIN;
        difference_min_max = FAN_SPEED_MAX - FAN_SPEED_MIN;
        step = ceil(difference_min_max/6);

        if(temp > 70 || hum > 40) {
            fanSpeed += step;
        }
        if(temp > 72 || hum > 50) {
            fanSpeed += step;
        }
        if(temp > 74 || hum > 60) {
            fanSpeed += step;
        }
        if(temp > 76 || hum > 70) {
            fanSpeed += step;
        }
        if(temp > 78 || hum > 80) {
            fanSpeed += step;
        }
        if(temp > 80 || hum > 90) {
            fanSpeed += step;
        }
        //for sensor fail
        if(temp > 200 || hum > 200) {
            fanSpeed = FAN_SPEED_MIN;
        }
      
      analogWrite(FAN_SPEED_PIN,255-fanSpeed, 25000);
      //analogWrite(FAN_SPEED_PIN,fanSpeed,25000);
        
    }
  
     fanSpeedPercent = map(fanSpeed,0,255,0,100);
  
     if(currentScreen == "homeScreen") {      
         
       //write status to screen
       tft.fillRect(190, 10, 50, 35, ILI9341_BLACK);
       
       
       tft.setCursor(190, 20);
       tft.setTextSize(2);
       tft.setTextColor(ILI9341_WHITE); 
                     
       tft.print(String::format("%.0f",fanSpeedPercent));
       tft.print("%");
     }
  
    if(currentScreen == "fanScreen") {
      
       tft.fillRect(10, 10, 50, 25, ILI9341_BLACK);
      
       tft.setCursor(10,140);
       tft.print(String(fanSpeedPercent));  
      
    }
 
      
    }  

void SystemStatus::init() {
    
  this->status.dayCounter = -1; //counting days the grow was active. Starting from 1
  this->status.isDay = true; // true if the light is on
  this->status.minutesInPhotoperiod = 0; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
  this->status.dayDuration = 18*60; //how long is the light on?  Starts out at 18 hrs (18*60)
  this->status.fanMode = 50; // -1 for auto, num value 0-100 for percentage
  
  this->save();
  
}

void SystemStatus::save() {
  EEPROM.put(0, this->status);
}

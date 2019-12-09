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
  
      tft.fillRect(130, 180, 80, 25, ILI9341_BLACK);
  
      tft.setCursor(70,180);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(3);

      tft.print("Day "+String(this->getDayCount()));
}

void SystemStatus::drawFanSpeed() {
       //write status to screen
       tft.fillRect(200, 10, 56, 35, ILI9341_BLACK);
       
       tft.setCursor(210, 10);
       tft.setTextSize(2);
       tft.setTextColor(ILI9341_WHITE); 
       
       //tft.print(String(String::format("%.0f",this->status.fanSpeed)));
       tft.print(String(String::format("%.0f",this->status.fanSpeed+5)));

       tft.print("%");
       
       tft.setTextSize(1);
       if(this->status.fanAutoMode) {
         tft.setCursor(200, 30);
         tft.print("automatic");
       } else {
         tft.setCursor(210, 30);
         tft.print("manual");
       }
}

bool SystemStatus::getFanAutoMode() {
  return this->status.fanAutoMode;
}

void SystemStatus::setFanAutoMode(bool fanAutoMode) {
  this->status.fanAutoMode = fanAutoMode;
  this->save();
}

char SystemStatus::getTempUnit() {
  return this->status.tempUnit;
}

void SystemStatus::setTempUnit(char tempUnit) {
  this->status.tempUnit = tempUnit;
  this->save();
}

float SystemStatus::getFanSpeed() {
  return this->status.fanSpeed;
}

void SystemStatus::setFanSpeed(float fanSpeed) {
  this->status.fanSpeed = fanSpeed;
  this->save();
}

void SystemStatus::check_fan() {
      
      if(this->status.fanAutoMode == 0) { //manual
        
        int fanSpeed = map(this->status.fanSpeed, 0.0, 100.0, 0.0, 255.0);
        
        analogWrite(FAN_SPEED_PIN,255-fanSpeed, 25000);
        //analogWrite(FAN_SPEED_PIN,fanSpeed, 25000);

      } else {

        float fanSpeedPercent = FAN_SPEED_MIN;
        step = 5;

        if(temp > 70 || hum > 40) {
            fanSpeedPercent += step;
        }
    
        if(temp > 72 || hum > 50) {
            fanSpeedPercent += step;
        }
  
        if(temp > 74 || hum > 60) {
            fanSpeedPercent += step;
        }    
        if(temp > 76 || hum > 70) {
            fanSpeedPercent += step;
        }
        if(temp > 78 || hum > 80) {
            fanSpeedPercent += step;
        }
        if(temp > 80 || hum > 90) {
            fanSpeedPercent += step;
        }
        //for sensor fail
        if(temp > 200 || hum > 200) {
            fanSpeedPercent = FAN_SPEED_MIN+15;
        }
        
        this->setFanSpeed(fanSpeedPercent);
        
        int fanSpeed = map(fanSpeedPercent, 0.0, 100.0, 0.0, 255.0);

        analogWrite(FAN_SPEED_PIN,255-fanSpeed, 25000);
      //analogWrite(FAN_SPEED_PIN,fanSpeed,25000);
        
    }
  
    if(currentScreen == "homeScreen" || currentScreen == "fanScreen") {
      this->drawFanSpeed();
    }
      
    }  

void SystemStatus::init() {
    
  this->status.dayCounter = -1; //counting days the grow was active. Starting from 1
  this->status.isDay = true; // true if the light is on
  this->status.minutesInPhotoperiod = 0; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
  this->status.dayDuration = 18*60; //how long is the light on?  Starts out at 18 hrs (18*60)
  this->status.fanAutoMode = 1; // 1 for auto, 0 for manual
  this->status.fanSpeed = 30; // 0-100 %
  this->status.tempUnit = 'F'; // C or F. Single characters use single quotes.
  
  this->save();
  
}

void SystemStatus::save() {
  EEPROM.put(0, this->status);
}

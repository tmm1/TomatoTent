#include "systemStatus.h" 
#include <Particle.h>

SystemStatus::SystemStatus() {
    
    //int dayCounter =-1, bool isDay = false, int minutesInPhotoperiod = 0, int dayDuration = (18*60)
  
 // SystemStatusStruct _systemStatus = { dayCounter, isDay, minutesInPhotoperiod, dayDuration };


    
    
   //EEPROM.get(0, systemStatus); // get system status after re-start


    
}
  
int SystemStatus::getDayCounter() {
  //return this->dayCounter;
  return 1;
}
bool SystemStatus::isDay() {
  return true;
}
int SystemStatus::getMinutesInPhotoperiod() {
  return 1;
}
int SystemStatus::getDayDuration() {
  return 100;
}

void SystemStatus::setIsDay(bool isDay) {
   
}
void SystemStatus::setMinutesInPhotoperiod(int minutesInPhotoperiod) {
  
}
void SystemStatus::save() {
  
}

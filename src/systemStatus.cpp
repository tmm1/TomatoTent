#include <systemStatus.h>

SystemStatus::SystemStatus() {
    
    //int dayCounter =-1, bool isDay = false, int minutesInPhotoperiod = 0, int dayDuration = (18*60)
  
   //status.dayCounter = -1;// = { -1, false, 0, (18*60) };
    
    
   //EEPROM.get(0, systemStatus); // get system status after re-start


    
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
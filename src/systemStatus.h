#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "application.h"

class SystemStatus {
/*  
  struct SystemStatusStruct {
     int dayCounter; //counting days the grow was active. Starting from 1
     bool isDay; // true if the light is on
     int minutesInPhotoperiod; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
     int dayDuration; //how long is the light on?  Starts out at 18 hrs (18*60)
  };
*/
  
  public:
  SystemStatus();
  int getDayCounter();
  bool isDay();
  int getMinutesInPhotoperiod();
  int getDayDuration();
  
  void setIsDay(bool);
  void setMinutesInPhotoperiod(int);
  void save();

};
#endif
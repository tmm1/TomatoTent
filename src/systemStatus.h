#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Particle.h"
#include "math.h"
#include "Adafruit_ILI9341.h"
#include "tent.h"

extern Adafruit_ILI9341 tft;
extern Tent tent;
extern String currentScreen;



class SystemStatus {
 
  struct Status {
     int dayCounter; //counting days the grow was active. Starting from 1
     bool isDay; // true if the light is on
     int minutesInPhotoperiod; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
     int dayDuration; //how long is the light on?  Starts out at 18 hrs (18*60)
  };
  
  Status status;
  
  public:
  SystemStatus();
  int getDayCount(void);
  bool isDay(void);
  int getMinutesInPhotoperiod(void);
  int getDayDuration(void);
  void setDayCount(int);
  void setIsDay(bool);
  void setMinutesInPhotoperiod(int);
  void countMinute();
  void save();
};
#endif

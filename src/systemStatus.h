#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include "Particle.h"
#include <Arduino.h>
#include "math.h"
#include "Adafruit_ILI9341.h"
#include "tent.h"
#include "screen_manager.h"

extern Adafruit_ILI9341 tft;
extern Tent tent;
extern ScreenManager screenManager;

class SystemStatus {

    double step;

    struct Status {
        int dayCounter; //counting days the grow was active. Starting from 1
        bool isDay; // true if the light is on
        int minutesInPhotoperiod; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
        int dayDuration; //how long is the light on?  Starts out at 18 hrs (18*60)
        bool fanAutoMode; // 1 for auto, 0 for manual
        float fanSpeed; // 0-100%
        char tempUnit;
    };

    Status status;

public:
    SystemStatus();
    int getDayCount(void);
    bool isDay(void);
    int getMinutesInPhotoperiod(void);
    int getDayDuration(void);
    bool getFanAutoMode(void);
    char getTempUnit(void);
    float getFanSpeed();
    void check_fan();
    void setDayCount(int);
    void setIsDay(bool);
    void setMinutesInPhotoperiod(int);
    void setDayDuration(int);
    void setFanAutoMode(bool);
    void setFanSpeed(float);
    void setTempUnit(char);
    void countMinute();
    void drawTimerStatus();
    void drawFanSpeed();
    void drawDayCounter();
    void init();
    void save();
};
#endif
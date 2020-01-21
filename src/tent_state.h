#pragma once

#include "Particle.h"
#include <Arduino.h>
#include "math.h"

class TentState {

    struct {
        int dayCounter; //counting days the grow was active. Starting from 1
        bool isDay; // true if the light is on
        int minutesInPhotoperiod; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
        int dayDuration; //how long is the light on?  Starts out at 18 hrs (18*60)
        bool fanAutoMode; // 1 for auto, 0 for manual
        float fanSpeed; // 0-100%
        char tempUnit;
    } eeprom;

public:
    TentState();
    int getDayCount(void);
    bool isDay(void);
    int getMinutesInPhotoperiod(void);
    int getDayDuration(void);
    bool getFanAutoMode(void);
    char getTempUnit(void);
    float getFanSpeed();
    void setDayCount(int);
    void setIsDay(bool);
    void setMinutesInPhotoperiod(int);
    void setDayDuration(int);
    void setFanAutoMode(bool);
    void setFanSpeed(float);
    void setTempUnit(char);
    void init();
    void save();
};

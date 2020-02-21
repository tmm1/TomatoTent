#pragma once

#include "Particle.h"
#include <Arduino.h>
#include "math.h"

class TentState {

    struct {
        int dayCounter; // counting days the grow was active. Starting from 1
        bool isDay; // true if the light is on
        int minutesInPhotoperiod; // how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
        int dayDuration; // how long is the light on?  Starts out at 18 hrs (18*60)
        bool fanAutoMode; // 1 for auto, 0 for manual
        float fanSpeed; // 0-100%
        char tempUnit;
    } eeprom;

public:
    TentState();

    void init()
    {
        eeprom.dayCounter = -1;
        eeprom.isDay = true;
        eeprom.minutesInPhotoperiod = 0;
        eeprom.dayDuration = 18 * 60;
        eeprom.fanAutoMode = 1;
        eeprom.fanSpeed = 30;
        eeprom.tempUnit = 'F';
        save();
    }
    void migrate()
    {
        //for updates from earlier version that don't have temp units
        if (getTempUnit() != 'F' && getTempUnit() != 'C') {
            setTempUnit('F');
        }
    }
    void begin();
    void save();

    bool isDay(void);
    void setIsDay(bool);

    int getDayCount(void);
    void setDayCount(int);

    int getMinutesInPhotoperiod(void);
    void setMinutesInPhotoperiod(int);

    int getDayDuration(void);
    void setDayDuration(int);

    bool getFanAutoMode(void);
    void setFanAutoMode(bool);

    float getFanSpeed();
    void setFanSpeed(float);

    char getTempUnit(void);
    void setTempUnit(char);
};

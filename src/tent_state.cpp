#include "tent_state.h"

TentState::TentState()
{
    EEPROM.get(0, eeprom);
}

int TentState::getDayCount()
{
    return eeprom.dayCounter;
}

bool TentState::isDay()
{
    return eeprom.isDay;
}

int TentState::getMinutesInPhotoperiod()
{
    return eeprom.minutesInPhotoperiod;
}

int TentState::getDayDuration()
{
    return eeprom.dayDuration;
}

void TentState::setDayDuration(int dayDuration)
{
    eeprom.dayDuration = dayDuration;
    save();
}

void TentState::setDayCount(int dayCount)
{
    eeprom.dayCounter = dayCount;
    save();
}

void TentState::setIsDay(bool isDay)
{
    eeprom.isDay = isDay;
    save();
}

void TentState::setMinutesInPhotoperiod(int minutesInPhotoperiod)
{
    eeprom.minutesInPhotoperiod = minutesInPhotoperiod;
    save();
}

bool TentState::getFanAutoMode()
{
    return eeprom.fanAutoMode;
}

void TentState::setFanAutoMode(bool fanAutoMode)
{
    eeprom.fanAutoMode = fanAutoMode;
    save();
}

char TentState::getTempUnit()
{
    return eeprom.tempUnit;
}

void TentState::setTempUnit(char tempUnit)
{
    eeprom.tempUnit = tempUnit;
    save();
}

float TentState::getFanSpeed()
{
    return eeprom.fanSpeed;
}

void TentState::setFanSpeed(float fanSpeed)
{
    eeprom.fanSpeed = fanSpeed;
    save();
}

void TentState::init()
{
    eeprom.dayCounter = -1; //counting days the grow was active. Starting from 1
    eeprom.isDay = true; // true if the light is on
    eeprom.minutesInPhotoperiod = 0; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
    eeprom.dayDuration = 18 * 60; //how long is the light on?  Starts out at 18 hrs (18*60)
    eeprom.fanAutoMode = 1; // 1 for auto, 0 for manual
    eeprom.fanSpeed = 30; // 0-100 %
    eeprom.tempUnit = 'F'; // C or F. Single characters use single quotes.

    save();
}

void TentState::save()
{
    EEPROM.put(0, eeprom);
}

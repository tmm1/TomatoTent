#include "systemStatus.h"

SystemStatus::SystemStatus()
{
    EEPROM.get(0, status);
}

int SystemStatus::getDayCount()
{
    return this->status.dayCounter;
}

bool SystemStatus::isDay()
{
    return this->status.isDay;
}

int SystemStatus::getMinutesInPhotoperiod()
{
    return this->status.minutesInPhotoperiod;
}

int SystemStatus::getDayDuration()
{
    return this->status.dayDuration;
}

void SystemStatus::setDayDuration(int dayDuration)
{
    this->status.dayDuration = dayDuration;
    this->save();
}

void SystemStatus::setDayCount(int dayCount)
{
    this->status.dayCounter = dayCount;
    this->save();
}

void SystemStatus::setIsDay(bool isDay)
{
    this->status.isDay = isDay;
    this->save();
}

void SystemStatus::setMinutesInPhotoperiod(int minutesInPhotoperiod)
{
    this->status.minutesInPhotoperiod = minutesInPhotoperiod;
    this->save();
}

bool SystemStatus::getFanAutoMode()
{
    return this->status.fanAutoMode;
}

void SystemStatus::setFanAutoMode(bool fanAutoMode)
{
    this->status.fanAutoMode = fanAutoMode;
    this->save();
}

char SystemStatus::getTempUnit()
{
    return this->status.tempUnit;
}

void SystemStatus::setTempUnit(char tempUnit)
{
    this->status.tempUnit = tempUnit;
    this->save();
}

float SystemStatus::getFanSpeed()
{
    return this->status.fanSpeed;
}

void SystemStatus::setFanSpeed(float fanSpeed)
{
    this->status.fanSpeed = fanSpeed;
    this->save();
}

void SystemStatus::init()
{
    this->status.dayCounter = -1; //counting days the grow was active. Starting from 1
    this->status.isDay = true; // true if the light is on
    this->status.minutesInPhotoperiod = 0; //how long has the system been in current photoperiod?  E.g. 31 minutes in NIGHT
    this->status.dayDuration = 18 * 60; //how long is the light on?  Starts out at 18 hrs (18*60)
    this->status.fanAutoMode = 1; // 1 for auto, 0 for manual
    this->status.fanSpeed = 30; // 0-100 %
    this->status.tempUnit = 'F'; // C or F. Single characters use single quotes.

    this->save();
}

void SystemStatus::save()
{
    EEPROM.put(0, this->status);
}

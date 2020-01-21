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

void TentState::save()
{
    EEPROM.put(0, eeprom);
}

#include "systemStatus.h"
#include "tent.h"
#include "screen_manager.h"

extern Tent tent;
extern ScreenManager screenManager;

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

void SystemStatus::countMinute()
{
    this->setMinutesInPhotoperiod(this->getMinutesInPhotoperiod() + 1);

    if (this->isDay()) {
        if (this->getMinutesInPhotoperiod() >= this->getDayDuration()) { //day is over
            tent.growLight("OFF");
            this->setIsDay(false);
            this->setMinutesInPhotoperiod(0);
        }

    } else {
        if (this->getMinutesInPhotoperiod() > ((24 * 60) - this->getDayDuration())) { //night is over
            this->setDayCount(this->getDayCount() + 1);
            screenManager.markNeedsRedraw(DAY);
            tent.growLight("HIGH");
            this->setIsDay(true);
            this->setMinutesInPhotoperiod(0);
        }
    }

    screenManager.markNeedsRedraw(TIMER);
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

void SystemStatus::check_fan()
{
    if (this->status.fanAutoMode == 0) { //manual

        int fanSpeed = map(this->status.fanSpeed, 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);

    } else {

        float fanSpeedPercent = FAN_SPEED_MIN;
        step = 5;
        float tempFahrenheit = temp;
        if (this->status.tempUnit == 'C') {
            tempFahrenheit = (temp * 1.8) + 32;
        }

        if (tempFahrenheit > 70 || hum > 40) {
            fanSpeedPercent += step;
        }

        if (tempFahrenheit > 72 || hum > 50) {
            fanSpeedPercent += step;
        }

        if (tempFahrenheit > 74 || hum > 60) {
            fanSpeedPercent += step;
        }
        if (tempFahrenheit > 76 || hum > 70) {
            fanSpeedPercent += step;
        }
        if (tempFahrenheit > 78 || hum > 80) {
            fanSpeedPercent += step;
        }
        if (tempFahrenheit > 80 || hum > 90) {
            fanSpeedPercent += step;
        }
        //for sensor fail
        if (tempFahrenheit > 200 || hum > 200) {
            fanSpeedPercent = FAN_SPEED_MIN + 15;
        }

        this->setFanSpeed(fanSpeedPercent);
        int fanSpeed = map(fanSpeedPercent, 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);
    }

    screenManager.markNeedsRedraw(FAN);
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

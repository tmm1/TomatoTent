#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;

Tent::Tent()
{
    this->growLightStatus = "OFF";
}

void Tent::begin()
{
    tp = new Timer(50000, &Tent::displayLightLow, *this, 1);
    tp1 = new Timer(60000, &Tent::displayLightOff, *this, 1);

    this->displayLightHigh();
}

void Tent::checkTemperature()
{
    double currentTemp = sht20.readTemperature();
    char tempUnit = state.getTempUnit();

    if (tempUnit == 'F') {
        currentTemp = (currentTemp * 1.8) + 32;
    }

    if ((temp == 0) || (temp != currentTemp)) {
        temp = currentTemp;
        screenManager.markNeedsRedraw(TEMPERATURE);
    }
}

void Tent::checkHumidity()
{
    double currentHumidity = sht20.readHumidity();

    if ((hum == 0) || (hum != currentHumidity)) {
        hum = currentHumidity;
        screenManager.markNeedsRedraw(HUMIDITY);
    }
}

void Tent::checkWaterLevel()
{
    double currentWaterLevel = sht20.readHumidity();

    if ((waterLevel == 0) || (waterLevel != currentWaterLevel)) {
        waterLevel = currentWaterLevel;
        screenManager.markNeedsRedraw(WATER_LEVEL);
    }
}

void Tent::fan(String fanStatus)
{
    if (fanStatus == "OFF") {
        analogWrite(FAN_SPEED_PIN, 255, 25000);
    }
}

void Tent::doCheckStats()
{ //checks & draws stats
    this->checkStats = true;
}

bool Tent::getCheckStats()
{
    return this->checkStats;
}

void Tent::resetCheckStats()
{
    this->checkStats = false;
}

int Tent::growLight(String brightness)
{
    if (brightness == "HIGH") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 255, 25000);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        this->growLightStatus = "HIGH";

    } else if (brightness == "LOW") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 20, 25000);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        this->growLightStatus = "LOW";
        this->dimTimeout = 15;

    } else if (brightness == "OFF") {
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, LOW);
        this->growLightStatus = "OFF";
    }

    return 1;
}

String Tent::getGrowLightStatus()
{
    return this->growLightStatus;
}

void Tent::minutelyTick()
{
    if (this->growLightStatus == "LOW") {
        this->dimTimeout -= 1;
        if (this->dimTimeout == 0) {
            this->growLight("HIGH");
        }
        screenManager.markNeedsRedraw(DIMMED);
    }

    countMinute();
}

void Tent::dimGrowLight()
{
    this->displayLightHigh();

    if (this->growLightStatus == "HIGH") {
        this->growLight("LOW");
    } else if (this->growLightStatus == "LOW") {
        this->growLight("HIGH");
    }

    screenManager.markNeedsRedraw(DIMMED);
}

void Tent::displayLightLow(void)
{
    while (displayBrightness > 30) {
        displayBrightness -= 5;
        analogWrite(TFT_BRIGHTNESS_PIN, displayBrightness);
        delay(10);
    }
}

void Tent::displayLightOff(void)
{
    analogWrite(TFT_BRIGHTNESS_PIN, 0);
    RGB.control(true);
    RGB.brightness(0);
}

bool Tent::displayLightHigh()
{
    unsigned long now = millis();

    if ((now - lastTime) >= 15000 || lastTime == 0) {
        lastTime = now;

        while (displayBrightness < 255) {
            displayBrightness += 5;
            analogWrite(TFT_BRIGHTNESS_PIN, displayBrightness);
            delay(5);
        }
        RGB.brightness(255);
        RGB.control(false);

        tp->start();
        tp1->start();
        return true;

    } else {
        return false;
    }
}

void Tent::countMinute()
{
    state.setMinutesInPhotoperiod(state.getMinutesInPhotoperiod() + 1);

    if (state.isDay()) {
        if (state.getMinutesInPhotoperiod() >= state.getDayDuration()) { //day is over
            growLight("OFF");
            state.setIsDay(false);
            state.setMinutesInPhotoperiod(0);
        }

    } else {
        if (state.getMinutesInPhotoperiod() > ((24 * 60) - state.getDayDuration())) { //night is over
            state.setDayCount(state.getDayCount() + 1);
            screenManager.markNeedsRedraw(DAY);
            growLight("HIGH");
            state.setIsDay(true);
            state.setMinutesInPhotoperiod(0);
        }
    }

    screenManager.markNeedsRedraw(TIMER);
}

void Tent::adjustFan()
{
    if (!state.getFanAutoMode()) { //manual

        int fanSpeed = map(state.getFanSpeed(), 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);

    } else {

        float fanSpeedPercent = FAN_SPEED_MIN;
        float step = 5;
        float tempFahrenheit = temp;
        if (state.getTempUnit() == 'C') {
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

        state.setFanSpeed(fanSpeedPercent);
        int fanSpeed = map(fanSpeedPercent, 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);
    }

    screenManager.markNeedsRedraw(FAN);
}

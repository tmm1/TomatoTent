#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;

Tent::Tent()
    : sensorTimer { Timer(7013, &Tent::doCheckStats, *this) }
    , minuteTimer { Timer(60000, &Tent::minutelyTick, *this) }
    , displayDimTimer { Timer(50000, &Tent::displayLightLow, *this, 1) }
    , displayOffTimer { Timer(60000, &Tent::displayLightOff, *this, 1) }
{
    this->growLightStatus = "OFF";
}

void Tent::setup()
{
    Particle.variable("tentTemperatureC", sensors.tentTemperatureC);
    Particle.variable("tentTemperatureF", sensors.tentTemperatureF);
    Particle.variable("tentHumidity", sensors.tentHumidity);

    // Init SHT20 Sensor
    sht20.initSHT20();
    delay(255);
    sht20.checkSHT20();

    this->displayLightHigh();

    // was there a grow in process before (re)booting?
    if (state.getDayCount() > -1) {
        state.migrate();

        if (state.isDay()) { // was the light on when we restarted?
            growLight("HIGH");
        }

        doCheckStats(); // First time right away
        countMinute(); // after restart
        start();

    } else {
        state.init();
    }
}

void Tent::start()
{
    minuteTimer.start();
    sensorTimer.start();
}

void Tent::stop()
{
    minuteTimer.stop();
    sensorTimer.stop();
}

void Tent::checkTemperature()
{
    double currentTemp = sht20.readTemperature();

    if ((sensors.tentTemperatureC == 0) || (sensors.tentTemperatureC != currentTemp)) {
        sensors.tentTemperatureC = currentTemp;
        sensors.tentTemperatureF = (currentTemp * 1.8) + 32;
        screenManager.markNeedsRedraw(TEMPERATURE);
    }
}

void Tent::checkHumidity()
{
    double currentHumidity = sht20.readHumidity();

    if ((sensors.tentHumidity == 0) || (sensors.tentHumidity != currentHumidity)) {
        sensors.tentHumidity = currentHumidity;
        screenManager.markNeedsRedraw(HUMIDITY);
    }
}

void Tent::checkWaterLevel()
{
    double currentWaterLevel = sht20.readHumidity();

    if ((sensors.waterLevel == 0) || (sensors.waterLevel != currentWaterLevel)) {
        sensors.waterLevel = currentWaterLevel;
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

        displayDimTimer.start();
        displayOffTimer.start();
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

        if (sensors.tentTemperatureF > 70 || sensors.tentHumidity > 40) {
            fanSpeedPercent += step;
        }

        if (sensors.tentTemperatureF > 72 || sensors.tentHumidity > 50) {
            fanSpeedPercent += step;
        }

        if (sensors.tentTemperatureF > 74 || sensors.tentHumidity > 60) {
            fanSpeedPercent += step;
        }
        if (sensors.tentTemperatureF > 76 || sensors.tentHumidity > 70) {
            fanSpeedPercent += step;
        }
        if (sensors.tentTemperatureF > 78 || sensors.tentHumidity > 80) {
            fanSpeedPercent += step;
        }
        if (sensors.tentTemperatureF > 80 || sensors.tentHumidity > 90) {
            fanSpeedPercent += step;
        }
        //for sensor fail
        if (sensors.tentTemperatureF > 200 || sensors.tentHumidity > 200) {
            fanSpeedPercent = FAN_SPEED_MIN + 15;
        }

        state.setFanSpeed(fanSpeedPercent);
        int fanSpeed = map(fanSpeedPercent, 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);
    }

    screenManager.markNeedsRedraw(FAN);
}

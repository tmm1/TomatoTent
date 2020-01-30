#include "tent.h"
#include "screen_manager.h"

extern ScreenManager screenManager;

Tent::Tent()
    : sensorTimer { Timer(7013, &Tent::markNeedsSensorUpdate, *this) }
    , minuteTimer { Timer(60000, &Tent::minutelyTick, *this) }
    , displayDimTimer { Timer(120000, &Tent::displayLightLow, *this, true) }
    , displayOffTimer { Timer(300000, &Tent::displayLightOff, *this, true) }
{
    this->growLightStatus = "OFF";
}

void Tent::setup()
{
    Particle.variable("tentTemperatureC", sensors.tentTemperatureC);
    Particle.variable("tentTemperatureF", sensors.tentTemperatureF);
    Particle.variable("tentHumidity", sensors.tentHumidity);
    Particle.variable("soilTemperatureC", sensors.soilTemperatureC);
    Particle.variable("soilTemperatureF", sensors.soilTemperatureF);
    Particle.variable("soilMoisture", sensors.soilMoisture);

    // init sensors
    sht20.initSHT20();
    delay(255);
    sht20.checkSHT20();
    sht30.setAddress(0);
    sht30.update();

    displayLightHigh();

    // was there a grow in process before (re)booting?
    if (state.getDayCount() > -1) {
        state.migrate();

        if (state.isDay()) { // was the light on when we restarted?
            growLight("HIGH");
        }

        start();

    } else {
        state.init();
    }
}

void Tent::start()
{
    minuteTimer.start();
    sensorTimer.start();

    fan("ON");
    markNeedsSensorUpdate();
    countMinute();
}

void Tent::stop()
{
    minuteTimer.stop();
    sensorTimer.stop();
}

void Tent::checkTent()
{
    double rawTemp = sht20.readTemperature();
    if (rawTemp == 998.0) {
        sensors.tentTemperatureC = sensors.tentTemperatureF = -1;
        sensors.tentHumidity = -1;
        return;
    }

    double currentTemp = (int)(rawTemp * 10) / 10.0;
    double currentHumidity = (int)(sht20.readHumidity() * 10) / 10.0;
    Serial.printlnf("action=sensor name=tent humidity=%.1f temperature=%.1f", currentHumidity, currentTemp);

    if ((sensors.tentTemperatureC == 0) || (sensors.tentTemperatureC != currentTemp)) {
        sensors.tentTemperatureC = currentTemp;
        sensors.tentTemperatureF = (currentTemp == 0 || currentTemp > 900) ? currentTemp : (currentTemp * 1.8 + 32);
        screenManager.markNeedsRedraw(TEMPERATURE);
    }

    if ((sensors.tentHumidity == 0) || (sensors.tentHumidity != currentHumidity)) {
        sensors.tentHumidity = currentHumidity;
        screenManager.markNeedsRedraw(HUMIDITY);
    }
}

void Tent::checkSoil()
{
    bool updated = sht30.update();
    if (!updated || sht30.temperature > 900) {
        sensors.waterLevel = 0;
        sensors.soilTemperatureC = sensors.soilTemperatureF = -1;
        return;
    }

    double moisture = (int)(sht30.humidity * 10) / 10.0;
    double temperature = (int)(sht30.temperature * 10) / 10.0;
    int waterLevel = (int)moisture;
    Serial.printlnf("action=sensor name=soil moisture=%.1f temperature=%.1f", moisture, temperature);

    sensors.soilMoisture = moisture;
    if ((sensors.waterLevel == 0) || (sensors.waterLevel != waterLevel)) {
        sensors.waterLevel = waterLevel;
        screenManager.markNeedsRedraw(SOIL_MOISTURE);
    }

    if ((sensors.soilTemperatureC == 0) || (sensors.soilTemperatureC != temperature)) {
        sensors.soilTemperatureC = temperature;
        sensors.soilTemperatureF = (temperature == 0 || temperature > 900) ? temperature : (temperature * 1.8 + 32);
        screenManager.markNeedsRedraw(SOIL_TEMPERATURE);
    }
}

void Tent::fan(String fanStatus)
{
    if (fanStatus == "OFF") {
        analogWrite(FAN_SPEED_PIN, 255, 25000);
    } else {
        int fanSpeed = map(state.getFanSpeed(), 0.0, 100.0, 0.0, 255.0);
        analogWrite(FAN_SPEED_PIN, 255 - fanSpeed, 25000);
    }
}

void Tent::markNeedsSensorUpdate()
{
    needsSensorUpdate = true;
}

void Tent::checkSensors()
{
    if (!needsSensorUpdate) {
        return;
    }
    needsSensorUpdate = false;

    checkTent();
    checkSoil();
    adjustFan();
}

void Tent::checkInputs()
{
    if (digitalRead(DIM_PIN) == LOW) {
        displayLightHigh();
        dimmerBtnPressed = true;
        return;
    } else if (!dimmerBtnPressed) {
        return;
    }

    unsigned long now = millis();
    unsigned long diff = now - lastDimmerBtnTime;

    lastDimmerBtnTime = now;
    dimmerBtnPressed = false;

    if (diff <= 300 && growLightStatus == "LOW") {
        muteGrowLight();
    } else {
        dimGrowLight();
    }
}

int Tent::growLight(String brightness)
{
    if (brightness == "HIGH") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 255, 25000);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        growLightStatus = brightness;

    } else if (brightness == "LOW") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 20, 25000);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        growLightStatus = brightness;
        dimTimeout = 15;

    } else if (brightness == "MUTE") {
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, LOW);
        growLightStatus = brightness;

    } else if (brightness == "OFF") {
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, LOW);
        growLightStatus = brightness;
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

void Tent::fadeGrowLight(String mode, int percent)
{
    int brightness = 128;
    if (mode == "SUNRISE") {
        brightness = 20 + (235 * percent / 100);
    } else if (mode == "SUNSET") {
        brightness = 255 - (235 * percent / 100);
    }
    analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, brightness, 25000);
    digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
}

void Tent::dimGrowLight()
{
    if (growLightStatus == "HIGH") {
        growLight("LOW");
    } else if (growLightStatus == "LOW" || growLightStatus == "MUTE") {
        growLight("HIGH");
    }

    screenManager.markNeedsRedraw(DIMMED);
}

void Tent::muteGrowLight()
{
    if (growLightStatus != "MUTE") {
        growLight("MUTE");
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

    if ((now - lastDisplayLightTime) >= 15000 || lastDisplayLightTime == 0) {
        lastDisplayLightTime = now;

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

    int minutesInPeriod = state.getMinutesInPhotoperiod();
    int dayDuration = state.getDayDuration();
    int nightDuration = (24 * 60) - dayDuration;

    if (state.isDay()) {
        if (minutesInPeriod > dayDuration) { //day is over
            growLight("OFF");
            state.setIsDay(false);
            state.setMinutesInPhotoperiod(0);
            screenManager.markNeedsRedraw(DAY);

        } else if (minutesInPeriod >= dayDuration - 10) { //day is ending
            if (growLightStatus == "HIGH") {
                fadeGrowLight("SUNSET", (10 - (dayDuration - minutesInPeriod)) * 10);
            }
        }

    } else {
        if (minutesInPeriod > nightDuration) { //night is over
            state.setDayCount(state.getDayCount() + 1);
            growLight("HIGH");
            state.setIsDay(true);
            state.setMinutesInPhotoperiod(0);
            screenManager.markNeedsRedraw(DAY);

        } else if (minutesInPeriod >= nightDuration - 10) { //night is ending
            if (growLightStatus == "OFF") {
                fadeGrowLight("SUNRISE", (10 - (nightDuration - minutesInPeriod)) * 10);
            }
        }
    }

    screenManager.markNeedsRedraw(TIMER);
}

void Tent::adjustFan()
{
    if (!state.getFanAutoMode()) { //manual

        fan("ON");

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

        if (fanSpeedPercent != state.getFanSpeed()) {
            state.setFanSpeed(fanSpeedPercent);
            fan("ON");
            screenManager.markNeedsRedraw(FAN);
        }
    }
}

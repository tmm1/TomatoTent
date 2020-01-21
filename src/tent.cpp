#include "tent.h"

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

void Tent::check_temperature(char tempUnit)
{
    double currentTemp = sht20.readTemperature();

    if (tempUnit == 'F') {
        currentTemp = (currentTemp * 1.8) + 32;
    }

    if ((temp == 0) || (temp != currentTemp)) {
        temp = currentTemp;
        screenManager.markDirty(TEMPERATURE);
    }
}

void Tent::check_humidity()
{
    double currentHumidity = sht20.readHumidity();

    if ((hum == 0) || (hum != currentHumidity)) {
        hum = currentHumidity;
        screenManager.markDirty(HUMIDITY);
    }
}

void Tent::check_waterlevel()
{
    double currentWaterLevel = sht20.readHumidity();

    if ((waterLevel == 0) || (waterLevel != currentWaterLevel)) {
        waterLevel = currentWaterLevel;
        screenManager.markDirty(WATER_LEVEL);
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
        screenManager.markDirty(DIMMED);
    }
}

void Tent::dimGrowLight()
{
    this->displayLightHigh();

    if (this->growLightStatus == "HIGH") {
        this->growLight("LOW");
    } else if (this->growLightStatus == "LOW") {
        this->growLight("HIGH");
    }

    screenManager.markDirty(DIMMED);
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

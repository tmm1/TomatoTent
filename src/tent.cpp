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
        this->draw_temperature_home(tempUnit);
    }
}

void Tent::draw_temperature_home(char tempUnit)
{
    if (currentScreen == "homeScreen") {

        //reset screen
        tft.fillRect(50, 60, 141, 25, ILI9341_BLACK);

        //Temperature
        tft.setCursor(50, 60);
        tft.setTextColor(ILI9341_GREEN);
        tft.setTextSize(3);

        tft.print(String::format("%.1f", temp));
        tft.setTextSize(2);

        if (tempUnit == 'F') {
            tft.print(" F");
        } else {
            tft.print(" C");
        }
    }
}

void Tent::check_humidity()
{
    double currentHumidity = sht20.readHumidity();

    if ((hum == 0) || (hum != currentHumidity)) {
        hum = currentHumidity;
        this->draw_humidity_home();
    }
}

void Tent::draw_humidity_home()
{
    if (currentScreen == "homeScreen") {
        /////// Humidity
        tft.fillRect(50, 110, 141, 25, ILI9341_BLACK);

        tft.setCursor(50, 110);
        tft.setTextColor(ILI9341_PINK);
        tft.setTextSize(3);

        tft.print(String::format("%.1f", hum));

        tft.setTextSize(2);
        tft.print(" %");
    }
}

void Tent::check_waterlevel()
{
    double currentWaterLevel = sht20.readHumidity();

    if ((waterLevel == 0) || (waterLevel != currentWaterLevel)) {
        waterLevel = currentWaterLevel;
        this->draw_waterlevel_home();
    }
}

void Tent::draw_waterlevel_home()
{
    if (currentScreen == "homeScreen") {
        const float waterLevelBoxHeight = 150;
        const int waterLevelBoxTop = 60;

        int waterLevelHeight = floor((waterLevelBoxHeight / 100) * waterLevel);

        int waterLevelTop = (waterLevelBoxHeight - waterLevelHeight) + waterLevelBoxTop - 1;

        //icon
        tft.drawBitmap(280, 30, iconWateringCan_24x24, 24, 24, ILI9341_GREEN);

        //outside box
        tft.drawRect(280, waterLevelBoxTop, 25, waterLevelBoxHeight, ILI9341_DARKGREY);

        //reset the box
        tft.fillRect(281, waterLevelBoxTop + 1, 23, waterLevelBoxHeight - 2, ILI9341_BLACK);

        //draw current water level
        tft.fillRect(281, waterLevelTop, 23, waterLevelHeight, ILI9341_BLUE);
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

void Tent::drawStats(char tempUnit)
{ //only draws stats
    this->draw_temperature_home(tempUnit);
    this->draw_humidity_home();
    this->draw_waterlevel_home(); //remove for stand alone controller
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
            tft.fillRoundRect(0, 220, 320, 25, 5, ILI9341_BLACK);
        } else {
            this->drawDimmedIndicator();
        }
    }
}

void Tent::dimGrowLight()
{
    this->displayLightHigh();

    if (this->growLightStatus == "HIGH") {

        this->growLight("LOW");
        this->drawDimmedIndicator();

    } else if (this->growLightStatus == "LOW") {

        this->growLight("HIGH");
        tft.fillRoundRect(0, 220, 320, 25, 5, ILI9341_BLACK);
    }
}

void Tent::drawDimmedIndicator()
{
    tft.fillRoundRect(0, 220, 320, 25, 5, ILI9341_RED);

    tft.setCursor(120, 222);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Dimmed (" + String(dimTimeout) + "m)");

    tft.drawBitmap(97, 222, iconBulb_16x16, 16, 16, ILI9341_WHITE);
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

/* 
void Tent::drawTime() {
    String currentTime = Time.format(Time.now(), "%l:%M %P %S");

    tft.fillRect(190,7,140,18,ILI9341_BLACK);

    tft.setCursor(190, 7);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    tft.print(currentTime);
}
*/

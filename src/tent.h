#ifndef TENT_H
#define TENT_H

#include "Particle.h"
#include <Arduino.h>
#include "DFRobot_SHT20.h"
#include "libs/SHT30.h"
#include "tent_state.h"

#define GROW_LIGHT_BRIGHTNESS_PIN TX
#define GROW_LIGHT_ON_OFF_PIN D7
#define FAN_SPEED_PIN RX
#define FAN_SPEED_MIN 10 //percent
#define TFT_BRIGHTNESS_PIN WKP
#define DIM_PIN DAC

class Tent {
private:
    DFRobot_SHT20 sht20;
    SHT30 sht30;
    Timer sensorTimer, minuteTimer;
    Timer displayDimTimer, displayOffTimer;

    int displayBrightness = 0;
    String growLightStatus;
    unsigned long lastDisplayLightTime = 0;
    bool dimmerBtnPressed = false;
    unsigned long lastDimmerBtnTime = 0;

    bool needsSensorUpdate;
    void markNeedsSensorUpdate();

    void dimGrowLight();
    void muteGrowLight();
    void fadeGrowLight(String mode, int percent);

    void displayLightLow();
    void displayLightOff();

public:
    Tent();
    TentState state;
    int dimTimeout = 0;

    struct {
        double tentTemperatureC;
        double tentTemperatureF;
        double tentHumidity;
        double soilTemperatureC;
        double soilTemperatureF;
        double soilMoisture;
        int waterLevel;
    } sensors;

    void setup();
    void start();
    void stop();

    void adjustFan();
    void countMinute();
    void minutelyTick();

    void checkInputs();
    void checkSensors();
    void checkTemperature();
    void checkHumidity();
    void checkSoil();

    void fan(String fanStatus);
    int growLight(String brightness);
    String getGrowLightStatus();
    bool displayLightHigh();
};
#endif

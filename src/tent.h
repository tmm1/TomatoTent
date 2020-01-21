#ifndef TENT_H
#define TENT_H

#include "Particle.h"
#include <Arduino.h>
#include "DFRobot_SHT20.h"
#include "screen_manager.h"
#include "Adafruit_ILI9341.h"
#include "icons.h"

#define GROW_LIGHT_BRIGHTNESS_PIN TX
#define GROW_LIGHT_ON_OFF_PIN D7
#define FAN_SPEED_PIN RX
#define FAN_SPEED_MIN 10 //percent
#define TFT_BRIGHTNESS_PIN WKP
#define DIM_PIN DAC

extern DFRobot_SHT20 sht20;
extern Adafruit_ILI9341 tft;
extern double temp;
extern double hum;
extern double waterLevel;
extern ScreenManager screenManager;

class Tent {

    int displayBrightness = 0;
    int dimTimeout = 0;
    String growLightStatus;

public:
    Tent();
    unsigned long lastTime = 0;
    Timer* tp;
    Timer* tp1;
    bool checkStats;

    void begin();
    void check_temperature(char tempUnit);
    void check_humidity();
    void check_waterlevel();
    void fan(String fanStatus);
    void doCheckStats();
    bool getCheckStats();
    void resetCheckStats();
    void minutelyTick();
    int growLight(String brightness);
    String getGrowLightStatus();
    void dimGrowLight();
    void drawDimmedIndicator();
    void displayLightLow(void);
    void displayLightOff(void);
    bool displayLightHigh(void);
};
#endif

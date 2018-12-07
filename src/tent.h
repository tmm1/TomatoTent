#ifndef TENT_H
#define TENT_H

#include "Particle.h"

#include "DFRobot_SHT20.h"
#include <Adafruit_ILI9341.h>

#define GROW_LIGHT_BRIGHTNESS_PIN TX
#define GROW_LIGHT_ON_OFF_PIN D7
#define FAN_SPEED_PIN RX
#define FAN_SPEED_MAX 175
#define FAN_SPEED_MIN 230
#define TFT_BRIGHTNESS_PIN WKP
#define DIM_PIN DAC

extern DFRobot_SHT20 sht20;
extern Adafruit_ILI9341 tft;
extern double temp;
extern double hum;
extern double waterLevel;
extern double fanSpeed;
extern String currentScreen;
extern bool dimmerButtonPressed;

class Tent {
  
  double step;
  double difference_min_max;
  int displayBrightness = 0;
  String growLightStatus;
  
  public:
    Tent();
    unsigned long lastTime = 0;
    Timer *tp;
    Timer *tp1;

    void check_temperature();
    void draw_temperature_home();
    void check_humidity();
    void draw_humidity_home();
    void check_waterlevel();
    void draw_waterlevel_home();
    void check_fan();
    void checkStats();
    void drawStats();
    int growLight(String brightness);
    String getGrowLightStatus();
    void setDimButtonPressed();
    void dimGrowLight();
    void drawDimmedIndicator();
    void displayLightLow(void);
    void displayLightOff(void);
    bool displayLightHigh(void);
  
   // void drawTime();
   
};
#endif

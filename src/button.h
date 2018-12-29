#ifndef BUTTON_H
#define BUTTON_H

#include <Particle.h>
#include <Adafruit_ILI9341.h>
#include "icons.h"

extern Adafruit_ILI9341 tft;

class Button {
  
  String name;
  uint16_t x0;
  uint16_t y0;
  uint16_t w;
  uint16_t h;
  String buttonText;
  uint16_t textOffsetLeft;
  uint16_t textOffsetTop;
  String currentStatus;
  
  public:
  bool pressed;
  Button();
  Button(String name, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, String buttonText, uint16_t textOffsetLeft, uint16_t textOffsetTop);
  
  void render();
  
  void renderPressed();
  String getName();
  uint16_t getx0();
   uint16_t gety0();
   uint16_t getW();
   uint16_t getH();
   String getButtonText();
   uint16_t getTextOffsetLeft();
   uint16_t getTextOffsetTop();
  
    bool isPressed(int x,int y);
    void setStatus(String status);
    String getStatus();
    
};
#endif
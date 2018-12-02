#ifndef SCREEN_H
#define SCREEN_H

#include <Particle.h>

#include <Adafruit_ILI9341.h>
extern Adafruit_ILI9341 tft;

#include <button.h>
extern Button button;

#include "systemStatus.h"
extern SystemStatus systemStatus;

class Screen {

  public:
  Screen();
  void homeScreen(Button *buttons);
};
#endif

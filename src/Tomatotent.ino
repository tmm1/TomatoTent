#define CS_PIN D5
#define TIRQ_PIN D1
#define TFT_DC A1
#define TFT_CS A2

double temp;
double hum;
double waterLevel;
double fanSpeed = 0;
String currentScreen = "homeScreen";

#include <XPT2046_Touch.h>
#include <Adafruit_mfGFX.h>
#include "DFRobot_SHT20.h"
#include "systemStatus.h"
#include "button.h"
#include "tent.h"
#include "screen.h"
#include "Adafruit_ILI9341.h"

/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, D6);
DFRobot_SHT20 sht20;
Button buttons[5];
int Button::buttonCount = 0;
Tent tent;
Screen screen;
SystemStatus systemStatus;

Timer draw_temp_home(5000,&Tent::checkStats,tent);

//sets the timer for the GrowLight Photoperiod
Timer minuteCounter(6000,&SystemStatus::countMinute, systemStatus);   //once per minute

XPT2046_Touchscreen ts(SPI1, 320, 240, CS_PIN, TIRQ_PIN);

void setup() {
    
    /*SET WIFI 
        Can remember up to 5 Networks,
        Set through Particle phone app or here in-code
        Needs to be set only once then will remember
    */
  // WiFi.setCredentials("WiFi-9L3X", "97953215");
   // WiFi.setCredentials("CIRCUITWEST", "Timelord14");
   // WiFi.setCredentials("Pasta", "Agam2804");
    //END SET WIFI
    
    Time.zone(+8);

    tft.begin();
    tft.setRotation(3);
    //DISPLAY BRIGHNESS
    pinMode(TFT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_ON_OFF_PIN, OUTPUT);
    pinMode(FAN_SPEED_PIN, OUTPUT);

    
    //TOUCH
    ts.begin();
    ts.setRotation(3);
    
    //fan
    
    analogWrite(FAN_SPEED_PIN,255);
    //END PINSS

    draw_temp_home.start();
    
    // Init SHT20 Sensor
    sht20.initSHT20();
    delay(255); 
    sht20.checkSHT20();
    
    //REMOTE FUNCTIONS
    //Particle.variable("temperatureC", temp);
    //Particle.variable("humidity", hum);
    //Particle.variable("fanSpeed", fanSpeed);
    
    //END REMOTE FUNCTIONS  
    
    screen.homeScreen(buttons,currentScreen);
  
    tent.displayLightHigh();
    
  
    //was there a grow in process before (re)booting? 
    if(systemStatus.getDayCount() > -1) {
    
      if(systemStatus.isDay()) {  //was the light on when we restarted?
        tent.growLight("HIGH");  
      }
      systemStatus.countMinute();  //after restart
      minuteCounter.start();
       
    }

    Serial.begin();
}



void loop(void) {
    
    if(ts.touched()) {
      
      tent.displayLightHigh(); // Switch on Displaylight on touch
      
      TS_Point p = ts.getPosition();
      
      Serial.println(currentScreen);
      

      //WAS A BUTTON TOUCHED - And which one?
     uint8_t c {0};

      for(c = 0; c < (sizeof(buttons) / sizeof(buttons[0])); ++c) {
        
        if(buttons[c].isPressed(p.x,p.y)) {
                    
          if( (buttons[c].getName() == "startGrowBtn") && (buttons[c].getStatus() == "none") ) {
            buttons[c].setStatus("pressed");
            
            tent.growLight("HIGH");
            systemStatus.setDayCount(1);
            systemStatus.setMinutesInPhotoperiod(0);
            screen.homeScreen(buttons, currentScreen);
                    
            systemStatus.countMinute(); // First time on new grow
            minuteCounter.start();
            
            break;
            
          }
          
          if( (buttons[c].getName() == "dayCounterBtn") && (buttons[c].getStatus() == "none") ) {
            buttons[c].setStatus("pressed");
            screen.cancelScreen(buttons, currentScreen);
            break;
          }
          
          if( (buttons[c].getName() == "cancelScreenOkBtn") && (buttons[c].getStatus() == "none") ) {
             buttons[c].setStatus("pressed");
             screen.homeScreen(buttons, currentScreen);
             break;
          }
          
          if( (buttons[c].getName() == "terminateBtn") && (buttons[c].getStatus() == "none") ) {
             buttons[c].setStatus("pressed");
             screen.cancelConfirmationScreen(buttons, currentScreen);
             break;
          }
          
          if( (buttons[c].getName() == "terminateYesBtn") && (buttons[c].getStatus() == "none") ) {
            buttons[c].setStatus("pressed");
            
            tent.growLight("OFF");
            systemStatus.setDayCount(-1);            
            minuteCounter.stop();
            screen.homeScreen(buttons, currentScreen);
            break;
          }  
          
          if( (buttons[c].getName() == "terminateNoBtn") && (buttons[c].getStatus() == "none") ) {
             buttons[c].setStatus("pressed");
             screen.homeScreen(buttons, currentScreen);
             break;
          }
          
          if( (buttons[c].getName() == "timerBtn") && (buttons[c].getStatus() == "none") ) {
             buttons[c].setStatus("pressed");
             screen.timerScreen(buttons, currentScreen);
             break;
          }           
        };  
      }
  
      delay(10); 
    
    }

};

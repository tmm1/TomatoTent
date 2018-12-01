#define CS_PIN D5
#define TIRQ_PIN D1
#define TFT_DC A1
#define TFT_CS A2

double temp;
double hum;
double waterLevel;
double fanSpeed = 0;


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
SystemStatus systemStatus;
Button buttons[1];
Tent tent;
Screen screen;


Timer draw_temp_home(5000,&Tent::checkStats,tent);

//starts the timer for the GrowLight Photoperiod
Timer minuteCounter(60000,&Screen::countMinute, screen);   //once per minute

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
    
    screen.homeScreen(buttons);
  
    tent.displayLightHigh();
    
  
    //check the EEPROM if a grow was in process after startup
    
  
  if(systemStatus.get()) {
    
  }
  
  
    if(systemStatus.getDayCounter() > -1) {   //was a grow in progress before we restarted?
    
      if(systemStatus.isDay()) {  //was the light on when we restarted?
        tent.growLight("HIGH");  
      }
      screen.countMinute();  //after restart
      minuteCounter.start();
       
    }
   
    Serial.begin();
}



void loop(void) {
    
    if(ts.touched()) {
      
      tent.displayLightHigh(); // Switch on Displaylight on touch
      
      TS_Point p = ts.getPosition();
      

      //WAS A BUTTON TOUCHED - And which one?
     uint8_t c {0};
      
      for(c = 0; c < (sizeof(buttons) / sizeof(buttons[0])); ++c) {
        
        if(buttons[c].isPressed(p.x,p.y)) {
                    
          //all the buttons
          if( (buttons[c].getName() == "startGrowBtn") && (buttons[c].getStatus() == "none")) {
            
            tent.growLight("LOW");
            buttons[c].setStatus("pressed");
            Serial.println(buttons[c].getName());
 
            //systemStatus.dayCounter() = 0;
            
            //EEPROM.clear();
           // systemStatus = { -1, false, 0, (18*60) };
            //EEPROM.put(0, systemStatus);
            
            //screen.countMinute(); // First time on new grow
            //minuteCounter.start();
            
          }
          
          
        };  
      }
  
      delay(10); 
    
    }

};

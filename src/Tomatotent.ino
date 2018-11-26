#define CS_PIN D5
#define TIRQ_PIN D1
#define GROW_LIGHT_BRIGHTNESS_PIN TX
#define GROW_LIGHT_ON_OFF_PIN D7
#define FAN_SPEED_PIN RX
#define FAN_SPEED_MAX 175
#define FAN_SPEED_MIN 230
#define TFT_DC A1
#define TFT_CS A2
#define TFT_BRIGHTNESS_PIN WKP

double temp;
double hum;
double waterLevel;
double fanSpeed = 0;


#include <XPT2046_Touch.h>
#include <Adafruit_mfGFX.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20 sht20;

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

#include "Adafruit_ILI9341.h"


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, D6);

#include "button.h"
#include "tent.h"
#include "screen.h"

DisplayLight displayLight;
Tent tent;
Screen screen;
SystemStatus systemStatus;


Timer draw_temp_home(5000,&Tent::checkStats,tent);


//START DISPLAY RELATED FUNCTIONS



// DRAWS CURRENT TIME

void drawTime() {
    
    String currentTime = Time.format(Time.now(), "%l:%M %P %S");
        
    tft.fillRect(190,7,140,18,ILI9341_BLACK);
    
    tft.setCursor(190, 7);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    tft.print(currentTime);

}





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
    Particle.variable("temperatureC", temp);
    Particle.variable("humidity", hum);
    Particle.variable("fanSpeed", fanSpeed);
    
    //END REMOTE FUNCTIONS  
    
    screen.homeScreen();
  
    displayLight.high();

    Serial.begin();
  
  
    EEPROM.get(0, systemStatus);
  
    if(systemStatus.dayCounter > -1) {   //is a grow in progress?
     
      if(systemStatus.isDay) {  //was the light on when we left?
        tent.growLight("HIGH");  
      }
    }
  
  

}



void loop(void) {
    
    if(ts.touched()) {
      
      displayLight.high();
      
      TS_Point p = ts.getPosition();

      //WAS A BUTTON TOUCHED?
      int c {0};
      for(c = 0; c < (sizeof(buttons) / sizeof(buttons[0])); ++c) {
        if(buttons[c].isPressed(p.x,p.y)) {
          String buttonName = buttons[c].getName();
          
          //all the buttons
          if(buttonName == "startGrowBtn" && buttons[c].pressed == false) {
            buttons[c].pressed = true;

            tent.growLight("LOW");
 
            systemStatus.dayCounter = 0;
            systemStatus.isDay = true;
            systemStatus.minutesLeftPhotoperiod = (18*60);
            
            EEPROM.put(0, systemStatus);

            
          }
          
          
        };  
      }
      
      delay(10); 
    
    }

}



// This #include statement was automatically added by the Particle IDE.
#include <XPT2046_Touch.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_mfGFX.h>


// This #include statement was automatically added by the Particle IDE.
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
#define TFT_DC A1
#define TFT_CS A2
#define TFT_BRIGHTNESS_PIN WKP

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, D6);
#include "button.h"

#define GROW_LIGHT_BRIGHTNESS_PIN TX
#define GROW_LIGHT_ON_OFF_PIN D7
#define FAN_SPEED_PIN RX
#define FAN_SPEED_MAX 175
#define FAN_SPEED_MIN 230

double temp;
double hum;
double waterLevel;
double fanSpeed = 0;

double step;
double difference_min_max;

void update_clock()
{
    drawTime();
};


//START DISPLAY RELATED FUNCTIONS

void check_temperature(){
    double currentTemp = sht20.readTemperature();
    
    if ((temp == 0) || (temp != currentTemp)){
        temp = currentTemp;
        draw_temperature_home();
    }
}

void draw_temperature_home() {
    
    //reset screen
    tft.fillRect(50,60,141,25,ILI9341_BLACK);
    
    //Temperature
    tft.setCursor(50, 60);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);

    tft.print(temp);    
    tft.print(" C");
}

void check_humidity(){
    
    double currentHumidity = sht20.readHumidity();
    
    if ((hum == 0) || (hum != currentHumidity)){
        hum = currentHumidity;
        draw_humidity_home();
    }
}

void draw_humidity_home() {
    

    /////// Humidity
    tft.fillRect(50,110,141,25,ILI9341_BLACK);
    
    tft.setCursor(50, 110);
    tft.setTextColor(ILI9341_BLUE);
    tft.setTextSize(3);

    tft.print(hum);    
    tft.print(" %");
}

void check_waterlevel(){
    
    double currentWaterLevel = sht20.readHumidity();

    if ((waterLevel == 0) || (waterLevel != currentWaterLevel)){
        waterLevel = currentWaterLevel;
        draw_waterlevel_home();
    }
}

void draw_waterlevel_home() {

    const float waterLevelBoxHeight = 150;
    const int waterLevelBoxTop = 60;

    int waterLevelHeight = floor((waterLevelBoxHeight / 100)*waterLevel);
    
    int waterLevelTop = (waterLevelBoxHeight - waterLevelHeight)+waterLevelBoxTop-1;
    
    //outside box
    tft.drawRect(280, waterLevelBoxTop, 25, waterLevelBoxHeight, ILI9341_CYAN);
    
    //reset the box
    tft.fillRect(281, waterLevelBoxTop+1, 23, waterLevelBoxHeight-2, ILI9341_BLACK);
     
    //draw current water level 
    tft.fillRect(281, waterLevelTop, 23, waterLevelHeight, ILI9341_BLUE);
    
}


void check_fan() {
    
    fanSpeed = FAN_SPEED_MIN;
    difference_min_max = FAN_SPEED_MIN - FAN_SPEED_MAX;
    step = ceil(difference_min_max/6);
    
    if(temp > 20 || hum > 40) {
        fanSpeed -= step;
    }
    if(temp > 22 || hum > 50) {
        fanSpeed -= step;
    }
    if(temp > 24 || hum > 60) {
        fanSpeed -= step;
    }
    if(temp > 26 || hum > 70) {
        fanSpeed -= step;
    }
    if(temp > 28 || hum > 80) {
        fanSpeed -= step;
    }
    if(temp > 30 || hum > 90) {
        fanSpeed -= step;
    }
    //for sensor fail
    if(temp > 200 || hum > 200) {
        fanSpeed = FAN_SPEED_MIN;
    }
    
    analogWrite(FAN_SPEED_PIN,fanSpeed);
}


// DRAWS CURRENT TIME

void drawTime() {
    
    String currentTime = Time.format(Time.now(), "%l:%M %P %S");
    
        
        tft.fillRect(190,7,140,18,ILI9341_BLACK);
    
        tft.setCursor(190, 7);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);

        tft.print(currentTime);

}

//END DRAW CURRENT TIME

//CLOUD FUNCTIONS

bool success = Particle.function("growLight", growLight);

int growLight(String brightness) {
    
    //SET TO HIGH BRIGHTNESS
    if(brightness == "HIGH") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 255);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        
        //analogWrite(FAN_SPEED_PIN,FAN_SPEED_MAX);
    } 
    //SET to low Brightness
    if(brightness == "LOW") {
        analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 20);
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        //analogWrite(FAN_SPEED_PIN,FAN_SPEED_MIN);
    }
    //SWITCH OFF
    if(brightness == "OFF") {
        digitalWrite(GROW_LIGHT_ON_OFF_PIN, LOW);
       // analogWrite(FAN_SPEED_PIN,255);
    }
  
    return 1;    
}


//END CLOUD FUNCTIONS

void checkStats(){
    check_temperature();
    check_humidity();
    check_waterlevel();
    check_fan();
}


// START CREATING TIMERS

Timer draw_temp_home(5000,checkStats);

// END CREATING TIMERS



//TOUCH

#define CS_PIN D5
#define TIRQ_PIN D1

XPT2046_Touchscreen ts(SPI1,CS_PIN, TIRQ_PIN);

void showTouchCoordinates() {
    
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
      
    TS_Point p = ts.getPoint();

    tft.fillRect(120,7,120,100,ILI9341_BLACK);
    
    tft.setCursor(120, 7);
    tft.print("z: ");
    tft.print(p.z);
    
    
    tft.setCursor(120, 30);
    tft.print("x: ");
    tft.print(p.x);
    
    tft.setCursor(120, 50);
    tft.print("y: ");
    tft.print(p.y); 

}

    Button startGrowBtn(20,180,250,38, "Start a Grow",18,8);
    Button lightTimeUpBtn(20,80,250,38, "UP",18,8);

    Button buttons[2] {startGrowBtn,lightTimeUpBtn};

// END TOUCH


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
    //TFT Display
    tft.begin();
    tft.setRotation(3);
    //DISPLAY BRIGHNESS
    pinMode(TFT_BRIGHTNESS_PIN, OUTPUT);
    analogWrite(TFT_BRIGHTNESS_PIN, 255);  //0 (dark) to 255 (max brightness)
    //END TFT DISPLAY
    
    //TOUCH
     ts.begin();
    ts.setRotation(3);
    
    //PINS
    pinMode(GROW_LIGHT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_ON_OFF_PIN, OUTPUT);
    growLight("OFF");
    
    //fan
    pinMode(FAN_SPEED_PIN, OUTPUT);
    analogWrite(FAN_SPEED_PIN,255);
    //END PINSS
    
    
    
    draw_temp_home.start();
    
    // Init SHT20 Sensor
    sht20.initSHT20();
    delay(255); 
    sht20.checkSHT20();
    
    tft.fillScreen(ILI9341_BLACK);
    
    //REMOTE FUNCTIONS
    Particle.variable("temperatureC", temp);
    Particle.variable("humidity", hum);
    Particle.variable("fanSpeed", fanSpeed);
    
    Particle.variable("step", step);
    Particle.variable("differenc", difference_min_max);
    
    //END REMOTE FUNCTIONS  
    
    //INTERRUPTS
    //attachInterrupt(TIRQ_PIN, showTouchCoordinates, FALLING);
    // END INTERRUPTS

    startGrowBtn.render();

      Serial.begin();

    
}



void loop(void) {
    
    if(ts.touched()) {

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
      
    TS_Point p = ts.getPoint();

    tft.fillRect(120,7,120,100,ILI9341_BLACK);
    
    tft.setCursor(120, 7);
    tft.print("z: ");
    tft.print(p.z);
    
    
    tft.setCursor(120, 30);
    tft.print("x: ");
    tft.print(p.x);
    
    tft.setCursor(120, 50);
    tft.print("y: ");
    tft.print(p.y); 
    
    delay(30);
      
      /////////////////// 
      int c {0};
      for(c = 0; c < (sizeof(buttons) / sizeof(buttons[0])); ++c) {
        if(buttons[c].isPushed(p.x,p.y)) {
          Serial.println("pushed");
        };  
      }
      
    
    
    }

}



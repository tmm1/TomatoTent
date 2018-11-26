#include "application.h"
using namespace std;

  struct SystemStatus {
     int dayCounter{-1};
     bool isDay;
     int minutesLeftPhotoperiod;
  };


class Tent {
  
  private:
  double step;
  double difference_min_max;

  public:
  
  Tent() {
    Particle.function("growLight", &Tent::growLight, this);
  }
  

    void check_temperature(){
        double currentTemp = sht20.readTemperature();

        if ((temp == 0) || (temp != currentTemp)){
            temp = currentTemp;
            this->draw_temperature_home();
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
            this->draw_humidity_home();
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
            this->draw_waterlevel_home();
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

  
    void checkStats(){
      this->check_temperature();
      this->check_humidity();
      this->check_waterlevel();
      this->check_fan();
    }
  
  
     int growLight(String brightness) {

        //SET TO HIGH BRIGHTNESS
        if(brightness == "HIGH") {
            analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 255);
            digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        } 
        //SET to low Brightness
        if(brightness == "LOW") {
            analogWrite(GROW_LIGHT_BRIGHTNESS_PIN, 20);
            digitalWrite(GROW_LIGHT_ON_OFF_PIN, HIGH);
        }
        //SWITCH OFF
        if(brightness == "OFF") {
            digitalWrite(GROW_LIGHT_ON_OFF_PIN, LOW);
        }

        return 1;    
    }


};




class DisplayLight
{
      private:
        
        unsigned long lastTime = 0;
        int brightness;
        Timer *tp;
        Timer *tp1;
        
        void low(void) {
          
          while(brightness > 30) {
            brightness -= 5;
            analogWrite(TFT_BRIGHTNESS_PIN, brightness);
            delay(10);
          }
                    
        };
        void off(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 0);
        }; 
        
        public:

        bool high ()
        {
        	unsigned long now = millis();
          if ((now - lastTime) >= 15000 || lastTime == 0) {
            lastTime = now;
            
            while(brightness < 255) {
              brightness += 5;
              analogWrite(TFT_BRIGHTNESS_PIN, brightness);
              delay(5);
            }            

            analogWrite(TFT_BRIGHTNESS_PIN, brightness);
            tp->reset ();
            tp1->reset ();
            return true;

          } else {
            return false;
          }

        };
        
        DisplayLight ()
        { 
            tp = new Timer (50000, &DisplayLight::low, *this, 1);
            tp1 = new Timer (60000, &DisplayLight::off, *this, 1);
        }        
        
};
#include "tent.h"

  Tent::Tent() {
    Particle.function("growLight", &Tent::growLight, this);
    tp = new Timer (50000, &Tent::displayLightLow, *this, 1);
    tp1 = new Timer (60000, &Tent::displayLightOff, *this, 1);
  }

  void Tent::check_temperature(){
        double currentTemp = sht20.readTemperature();
    
        currentTemp = (currentTemp * 1.8) + 32;

        if ((temp == 0) || (temp != currentTemp)){
            temp = currentTemp;
            this->draw_temperature_home();
        }
    }

    void Tent::draw_temperature_home() {
      
        if(currentScreen == "homeScreen") {

          //reset screen
          tft.fillRect(50,60,141,25,ILI9341_BLACK);

          //Temperature
          tft.setCursor(50, 60);
          tft.setTextColor(ILI9341_GREEN);
          tft.setTextSize(3);

          tft.print(temp);
          tft.setTextSize(2);
          tft.print(" F");
        }
    }

    void Tent::check_humidity(){
      
        double currentHumidity = sht20.readHumidity();

        if ((hum == 0) || (hum != currentHumidity)){
            hum = currentHumidity;
            this->draw_humidity_home();
        }
    }

    void Tent::draw_humidity_home() {

        if(currentScreen == "homeScreen") {

          /////// Humidity
          tft.fillRect(50,110,141,25,ILI9341_BLACK);

          tft.setCursor(50, 110);
          tft.setTextColor(ILI9341_BLUE);
          tft.setTextSize(3);

          tft.print(hum);
          tft.setTextSize(2);
          tft.print(" %");
        }
    }

    void Tent::check_waterlevel(){

        double currentWaterLevel = sht20.readHumidity();

        if ((waterLevel == 0) || (waterLevel != currentWaterLevel)){
            waterLevel = currentWaterLevel;
            this->draw_waterlevel_home();
        }
    }

    void Tent::draw_waterlevel_home() {
      
        if(currentScreen == "homeScreen") {

          const float waterLevelBoxHeight = 150;
          const int waterLevelBoxTop = 60;

          int waterLevelHeight = floor((waterLevelBoxHeight / 100)*waterLevel);

          int waterLevelTop = (waterLevelBoxHeight - waterLevelHeight)+waterLevelBoxTop-1;

          //outside box
          tft.drawRect(280, waterLevelBoxTop, 25, waterLevelBoxHeight, ILI9341_DARKGREY);

          //reset the box
          tft.fillRect(281, waterLevelBoxTop+1, 23, waterLevelBoxHeight-2, ILI9341_BLACK);

          //draw current water level 
          tft.fillRect(281, waterLevelTop, 23, waterLevelHeight, ILI9341_BLUE);
        }

    }


    void Tent::check_fan() {

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

  
    void Tent::checkStats(){   //checks & draws stats
      this->check_temperature();
      this->check_humidity();
      this->check_waterlevel();
      this->check_fan();
    }

    void Tent::drawStats(){  //only draws stats
      this->draw_temperature_home();
      this->draw_humidity_home();
      this->draw_waterlevel_home();
    }  
  
     int Tent::growLight(String brightness) {

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



        void Tent::displayLightLow(void) {
          
          while(displayBrightness > 30) {
            displayBrightness -= 5;
            analogWrite(TFT_BRIGHTNESS_PIN, displayBrightness);
            delay(10);
          }
                    
        }
        void Tent::displayLightOff(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 0);
        } 


        bool Tent::displayLightHigh()
        {
        	unsigned long now = millis();
          if ((now - lastTime) >= 15000 || lastTime == 0) {
            lastTime = now;
            
            while(displayBrightness < 255) {
              displayBrightness += 5;
              analogWrite(TFT_BRIGHTNESS_PIN, displayBrightness);
              delay(5);
            }            

            analogWrite(TFT_BRIGHTNESS_PIN, displayBrightness);
            tp->reset ();
            tp1->reset ();
            return true;

          } else {
            return false;
          }

        }
        

       /* 
        void Tent::drawTime() {
    
            String currentTime = Time.format(Time.now(), "%l:%M %P %S");

            tft.fillRect(190,7,140,18,ILI9341_BLACK);

            tft.setCursor(190, 7);
            tft.setTextColor(ILI9341_WHITE);
            tft.setTextSize(2);

            tft.print(currentTime);

        } */
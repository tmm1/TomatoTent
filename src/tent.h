#include "application.h"
using namespace std;

class Tent {

  public:
  
  Tent() {}


};


class DisplayLight
{
      private:
      
        unsigned long lastTime = 0;
        Timer *tp;
        Timer *tp1;
        
        void low(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 30);          
        };
        void off(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 0);
        }; 
        
        public:

        bool high ()
        {
        	unsigned long now = millis();
          if ((now - lastTime) >= 15000) {
            lastTime = now;
          } else {
            return false;
          }
          analogWrite(TFT_BRIGHTNESS_PIN, 255);
          tp->reset ();
          tp1->reset ();
          return true;
        }
        DisplayLight ()
        { 
            tp = new Timer (50000, &DisplayLight::low, *this, 1);
            tp1 = new Timer (60000, &DisplayLight::off, *this, 1);
        }        
        
};
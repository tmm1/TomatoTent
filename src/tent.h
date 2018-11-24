#include "application.h"
using namespace std;

class Tent {

  public:
  
  Tent() {}


};


class DisplayLight
{
      private:

        Timer *tp;
        void low(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 30);
          tp = new Timer (5000, &DisplayLight::off, *this, 1);
          tp->start ();
        };
        void off(void) {
          analogWrite(TFT_BRIGHTNESS_PIN, 0);
        }; 
        
        public:

        void high ()
        {
          analogWrite(TFT_BRIGHTNESS_PIN, 255);
          tp->start ();
        }
        DisplayLight ()
        { 
            tp = new Timer (5000, &DisplayLight::low, *this, 1);
        }        
        
};
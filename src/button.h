class Button {
  
  private:
  
  String name;
  uint16_t x0;
  uint16_t y0;
  uint16_t w;
  uint16_t h;
  String buttonText;
  uint16_t textOffsetLeft;
  uint16_t textOffsetTop;
  
  public:
  bool pressed;
  Button() { }
  Button(String name, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, String buttonText, uint16_t textOffsetLeft, uint16_t textOffsetTop)
  :name{name},x0{x0},y0{y0},w{w},h{h},buttonText{buttonText},textOffsetLeft{textOffsetLeft},textOffsetTop{textOffsetTop} {
   this->pressed = false;
  };
  
  void render() {
    
    tft.drawRect(x0,y0,w,h,ILI9341_WHITE);
    
    tft.fillRect(x0+1,y0+1,w-2,h-2,ILI9341_OLIVE);
    
    tft.setCursor(x0 + textOffsetLeft,y0 + textOffsetTop);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);

    tft.print(buttonText);     
  }
  
  void renderPressed() {
    tft.drawRect(x0,y0,w,h,ILI9341_RED);
  }
  String getName() {
    return name;
  };
  uint16_t getx0() {
    return x0;
  };
   uint16_t gety0() {
    return y0;
  };
   uint16_t getW() {
    return w;
  };
   uint16_t getH() {
    return h;
  };
   String getButtonText() {
    return buttonText;
  };
   uint16_t getTextOffsetLeft() {
    return textOffsetLeft;
  };
   uint16_t getTextOffsetTop() {
    return textOffsetTop;
  };
  
  bool isPressed(int x,int y) {
    
   if( (x > this->getx0() && x < (this->getx0()+this->getW()) ) && (y > this->gety0() && (y < (this->gety0()+this->getH()) ) )  ) {
     
     this->renderPressed();
     return true;
     
   } else {
     if(this->pressed) {
       this->pressed = false;
     }
    return false;
   }
     
  };


  
};
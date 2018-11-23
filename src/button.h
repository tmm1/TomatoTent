class Button {
  
  uint16_t x0;
  uint16_t y0;
  uint16_t w;
  uint16_t h;
  String buttonText;
  uint16_t textOffsetLeft;
  uint16_t textOffsetTop;
  
  public:
  
  Button(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, String buttonText, uint16_t textOffsetLeft, uint16_t textOffsetTop)
  :x0{x0},y0{y0},w{w},h{h},buttonText{buttonText},textOffsetLeft{textOffsetLeft},textOffsetTop{textOffsetTop} {};
  
  void render() {
    
    tft.drawRect(x0,y0,w,h,ILI9341_WHITE);
    
    tft.fillRect(x0+1,y0+1,w-2,h-2,ILI9341_OLIVE);
    
    tft.setCursor(x0 + textOffsetLeft,y0 + textOffsetTop);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);

    tft.print(buttonText);     
  }

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
  
  bool isPushed(int x,int y) {
    
    if() {
      
    }
    
    Serial.println(x);
    return true;
    
  }

  
};
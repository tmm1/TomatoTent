Button buttons[1];

class Screen {

  public:
  
  Screen() {

  }
  
  void homeScreen() {
    
    tft.fillScreen(ILI9341_BLACK);
    Button startGrowBtn("startGrowBtn", 20,180,250,38, "Start a Grow",18,8);
    buttons[0] = startGrowBtn;
    startGrowBtn.render();
 
  }
  
  
  
  
  
};
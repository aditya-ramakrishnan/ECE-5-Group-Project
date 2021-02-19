#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <URTouch.h>
#include <URTouchCD.h>

#define TFT_CS 12 
#define TFT_RST 11  
#define TFT_DC 10         
#define TFT_MOSI 9 
#define TFT_SCK 8             
#define TFT_MISO 7                    
#define t_SCK 6              
#define t_CS 5                
#define t_DIN 4              
#define t_DO 3             
#define t_IRQ 2         
    
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
URTouch ts(t_SCK, t_CS, t_DIN, t_DO, t_IRQ);
//x axis: 0 to 320
//y-axis: 0 to 240

int listenMode = 0;
int x, y;

void setup() {
  tft.begin();                     
  tft.setRotation(3);           
 
  ts.InitTouch();                   
  ts.setPrecision(PREC_EXTREME);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(6);               
  tft.setCursor(40,70);              
  tft.print("Welcome"); 

  tft.fillRect(58, 142, 210, 50, tft.color565(0, 87, 52));
  tft.fillRect(63, 147, 200, 40, tft.color565(0, 255, 100));
  tft.setTextColor(tft.color565(89, 94, 92));  
  tft.setTextSize(2);
  tft.setCursor(73,160);           
  tft.print("Start Listening");
}

void loop() {
  enterGUI();
  if(listenMode == 1) {
    listenMode = 0;
    trackTitle();
    homeButton();
    playButton();
    shuffleButtons();
  }
}

void enterGUI() {
  if (listenMode == 0) {
    while(ts.dataAvailable()){
      ts.read();                      
      x = ts.getX();                 
      y = ts.getY();
      if((x>=58) && (x<=268) && (y>=142) && (y<=192)){                      
        tft.fillScreen(ILI9341_BLACK);
      }
      else {
        break;
      }
      listenMode = 1;
      return;
    }
  }
  else {
    return;
  }
}

void homeScreen() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(6);               
  tft.setCursor(40,70);              
  tft.print("Welcome"); 

  tft.fillRect(58, 142, 210, 50, tft.color565(0, 87, 52));
  tft.fillRect(63, 147, 200, 40, tft.color565(0, 255, 100));
  tft.setTextColor(tft.color565(89, 94, 92));  
  tft.setTextSize(2);
  tft.setCursor(73,160);           
  tft.print("Start Listening");
}

void trackTitle() {
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(4);               
  tft.setCursor(40,50);              
  tft.print("Track Name"); 
}

void homeButton() {
  tft.fillRect(130, 190, 60, 50, tft.color565(0, 87, 52));
  tft.fillRect(135, 195, 50, 40, tft.color565(0, 255, 100));
  tft.fillRect(145, 213, 30, 20, tft.color565(255, 255, 255));
  tft.fillTriangle(159, 197, 140, 213, 178, 213, tft.color565(255, 255, 255));
}

void playButton(){
  tft.fillRect(130, 100, 60, 50, tft.color565(99, 99, 96));
  tft.fillRect(135, 105, 50, 40, tft.color565(168, 168, 165));
  tft.fillTriangle(140, 110, 140, 140, 180, 125, tft.color565(255, 255, 255));
}

void pauseButton() {
  tft.fillRect(130, 100, 60, 50, tft.color565(99, 99, 96));
  tft.fillRect(135, 105, 50, 40, tft.color565(168, 168, 165));
  tft.fillRect(145, 110, 10, 30, tft.color565(255, 255, 255));
  tft.fillRect(165, 110, 10, 30, tft.color565(255, 255, 255));
}

void shuffleButtons() {
  //forward
  tft.fillRect(230, 100, 60, 50, tft.color565(99, 99, 96));
  tft.fillRect(235, 105, 50, 40, tft.color565(168, 168, 165));
  tft.fillTriangle(240, 110, 240, 140, 280, 125, tft.color565(255, 255, 255));
  tft.fillRect(276, 110, 5, 30, tft.color565(255, 255, 255));

  //backward
  tft.fillRect(30, 100, 60, 50, tft.color565(99, 99, 96));
  tft.fillRect(35, 105, 50, 40, tft.color565(168, 168, 165));
  tft.fillTriangle(40, 125, 77, 140, 77, 110, tft.color565(255, 255, 255));
  tft.fillRect(40, 110, 5, 30, tft.color565(255, 255, 255));
}

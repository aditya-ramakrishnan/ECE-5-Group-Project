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
  // put your main code here, to run repeatedly:

}

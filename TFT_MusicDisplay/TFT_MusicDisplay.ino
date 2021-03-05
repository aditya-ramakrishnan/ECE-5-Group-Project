#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <vs1053_SdFat.h>  // Mp3 Shield Library

#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <URTouch.h>
#include <URTouchCD.h>

#define TFT_CS 22 
#define TFT_RST 24   
#define TFT_DC 26       
#define TFT_MOSI 28  
#define TFT_SCK 30             
#define TFT_MISO 32                    
#define t_SCK 34              
#define t_CS 36                
#define t_DIN 38              
#define t_DO 40             
#define t_IRQ 42         
		
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
URTouch ts(t_SCK, t_CS, t_DIN, t_DO, t_IRQ);
//x axis: 0 to 320
//y-axis: 0 to 240

int listenMode = 0;
int playPauseTracker = 1;
int x, y;
int songNum = 1;

SdFat sd; // Create object to handle SD functions
vs1053 MP3player; // Create Mp3 library object
int volumeLevel = 50;

void setup() {
	Serial.begin(115200);
	
	//initialize sd card
	if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
	if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
	
	//initialize shield
	MP3player.begin();

	MP3player.setVolume(50, 50);

	tft.begin();                     
	tft.setRotation(3);           
 
	ts.InitTouch();                   
	ts.setPrecision(PREC_EXTREME);
	homeScreen();
}

void loop() {
	buttonPress();
	if(listenMode == 1) {
		listenMode = 2;
		trackTitle();
		homeButton();
		playOrPause();
		shuffleButtons();
		volumeBar();
	}
}

void buttonPress() {
	if (listenMode == 0) {
		while(ts.dataAvailable()){
			ts.read();                      
			x = ts.getX();                 
			y = ts.getY();
			if((x>=58) && (x<=268) && (y>=142) && (y<=192)){                      
				tft.fillScreen(ILI9341_BLACK);
				listenMode = 1;
				return;
			}
		}
	}
	else if (listenMode == 2) {
			while(ts.dataAvailable()){
				ts.read();                      
				x = ts.getX();                 
				y = ts.getY();
				if((x>=130) && (x<=190) && (y>=190) && (y<=240)){ //home button check                      
					MP3player.stopTrack();
					homeScreen();
					listenMode = 0;
					songNum = 1;
					playPauseTracker = 1;
					return;
				}
				else if((x>=130) && (x<=190) && (y>=80) && (y<=130)) {  //play/pause check
					playPauseTracker++;
					if (playPauseTracker == 2) {
						playSong();
					}
					else if ((playPauseTracker%2) == 0) {
						MP3player.resumeMusic();
					}
					else {
						MP3player.pauseMusic();
					}
					playOrPause();
					return;
				}
				else if((x>=60) && (x<=95) && (y>=145) && (y<=175)) { //volume minus check
					volumeLevel += 2;
					MP3player.setVolume(volumeLevel, volumeLevel);
					return;
				}
				else if((x>=225) && (x<=260) && (y>=145) && (y<=175)) { //volume plus check
					volumeLevel -= 2;
					MP3player.setVolume(volumeLevel, volumeLevel);
					return;
				}
				else if((x>=230) && (x<=290) && (y>=80) && (y<=130)) { //shuffle forward check
					switch(songNum) {
						case 3: 
							songNum = 1;
							break;
						default:
							songNum++;
							break;
					}
					trackTitle();
					if (playPauseTracker != 2) {
						playPauseTracker = 2;
						playOrPause();
					}
					playSong();
					return;
				}
				else if((x>=30) && (x<=90) && (y>=80) && (y<=130)) { //shuffle backward check
					switch(songNum) {
						case 1: 
							songNum = 3;
							break;
						default:
							songNum--;
							break;
					}
					trackTitle();
					if (playPauseTracker != 2) {
						playPauseTracker = 2;
						playOrPause();
					}
					playSong();
					return;
				}
			}
	}
	else {
		return;
	}
}

void playSong() {
	MP3player.stopTrack();
	MP3player.playTrack(songNum);
}

void playOrPause() {
	if ((playPauseTracker%2) == 0) {
		pauseButton();
	}
	else {
		playButton();
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
	tft.fillRect(220, 30, 30, 30, tft.color565(0, 0, 0));
	tft.setTextColor(ILI9341_WHITE);  
	tft.setTextSize(4);               
	tft.setCursor(80,30);
	switch(songNum) {
		case 1: 
			tft.print("Track 1");
			break;
		case 2: 
			tft.print("Track 2");
			break;
		case 3: 
			tft.print("Track 3");
			break;
	}               
}

void homeButton() {
	tft.fillRect(130, 190, 60, 50, tft.color565(0, 87, 52));
	tft.fillRect(135, 195, 50, 40, tft.color565(0, 255, 100));
	tft.fillRect(145, 213, 30, 20, tft.color565(255, 255, 255));
	tft.fillTriangle(159, 197, 140, 213, 178, 213, tft.color565(255, 255, 255));
}

void playButton(){
	tft.fillRect(130, 80, 60, 50, tft.color565(99, 99, 96));
	tft.fillRect(135, 85, 50, 40, tft.color565(168, 168, 165));
	tft.fillTriangle(140, 90, 140, 120, 180, 105, tft.color565(255, 255, 255));
}

void pauseButton() {
	tft.fillRect(130, 80, 60, 50, tft.color565(99, 99, 96));
	tft.fillRect(135, 85, 50, 40, tft.color565(168, 168, 165));
	tft.fillRect(145, 90, 10, 30, tft.color565(255, 255, 255));
	tft.fillRect(165, 90, 10, 30, tft.color565(255, 255, 255));
}

void shuffleButtons() {
	//forward
	tft.fillRect(230, 80, 60, 50, tft.color565(99, 99, 96));
	tft.fillRect(235, 85, 50, 40, tft.color565(168, 168, 165));
	tft.fillTriangle(240, 90, 240, 120, 280, 105, tft.color565(255, 255, 255));
	tft.fillRect(276, 90, 5, 30, tft.color565(255, 255, 255));

	//backward
	tft.fillRect(30, 80, 60, 50, tft.color565(99, 99, 96));
	tft.fillRect(35, 85, 50, 40, tft.color565(168, 168, 165));
	tft.fillTriangle(40, 105, 77, 120, 77, 90, tft.color565(255, 255, 255));
	tft.fillRect(40, 90, 5, 30, tft.color565(255, 255, 255));
}

void volumeBar() {
	tft.fillRect(55, 140, 210, 40, tft.color565(99, 99, 96));
	tft.fillRect(60, 145, 200, 30, tft.color565(168, 168, 165));
	tft.fillRect(95, 140, 7, 40, tft.color565(99, 99, 96));
	tft.fillRect(215, 140, 7, 40, tft.color565(99, 99, 96));
	tft.fillRect(65, 157, 26, 7, tft.color565(255, 255, 255));  //minus button
	tft.fillRect(227, 157, 28, 7, tft.color565(255, 255, 255)); //plus button
	tft.fillRect(238, 147, 7, 26, tft.color565(255, 255, 255)); //plus button
	tft.setTextColor(ILI9341_WHITE);  
	tft.setTextSize(3);               
	tft.setCursor(107,150);              
	tft.print("Volume"); 
}

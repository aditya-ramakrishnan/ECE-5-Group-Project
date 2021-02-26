#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <vs1053_SdFat.h>  // Mp3 Shield Library

SdFat sd; // Create object to handle SD functions
vs1053 MP3player; // Create Mp3 library object


uint8_t result;

void setup()
{  
  Serial.begin(115200);
  
  //initialize sd card
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
  
  //initialize shield
  result = MP3player.begin();

  MP3player.setVolume(100, 100);
  
}
void loop()
{
  result = MP3player.playTrack(0);
  result = MP3player.playTrack(1);
  result = MP3player.playTrack(2);
}

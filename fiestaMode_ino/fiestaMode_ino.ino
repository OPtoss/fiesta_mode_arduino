#include <Adafruit_NeoPixel.h>

#include <Bridge.h>
#include <Console.h>
#include <YunServer.h>
#include <YunClient.h>

#include <Event.h>
#include <Timer.h>

#define PIN 6

#define NUM_PLAYERS 8
#define NUM_LEDS 232

#include "Color.h"
#include "Animations.h"

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint8_t mode = -1;
uint8_t prevMode = -1;

YunServer server;
YunClient client;

Timer timer;
uint8_t fiestaTimerID = -1;

char colorChars[10];
PlayerColorAnimation* playerColor = new PlayerColorAnimation();
//AmbianceAnimation* ambiance = new AmbianceAnimation();
RandomAnimation* fiestaAnim = new RandomAnimation();
CountdownAnimation* countdown = new CountdownAnimation();
RainbowAnimation* rainbow = new RainbowAnimation();
//PokerAnimation* poker = new PokerAnimation();

void setup() {
  // put your setup code here, to run once:
  
  //colorChars = (char*)malloc(100*sizeof(char));
  
  //strip init
  strip.begin();
  strip.setPixelColor( 0, 0xFF0000 );
  showStrip();
  
  Serial.begin(9600);
  Bridge.begin();
  Console.begin();
  
  strip.setPixelColor( 1, 0x0000FF );
  showStrip();
  
  //server init
  server.listenOnLocalhost();
  server.begin();
  
  for (int i=0; i<NUM_LEDS / NUM_PLAYERS; ++i)
  {
    strip.setPixelColor( i, 0x00FF00 );
    showStrip();
    delay(5);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // client input
  client = server.accept();
  if (client) {
    
    String modeStr = client.readStringUntil('/');
    modeStr.trim();        //kill whitespace
    int newMode = modeStr.toInt();
    
    //timer
    if (newMode == 4)
    {
      updateCountdown();
    }
    //modes
    else
      changeMode( newMode );
    
    client.println( modeStr );
    Console.println( modeStr );
    
    client.flush();
    
    //stop client
    client.stop();
  }

  //Update each mode every frame
  switch (mode )
  {
    case 0:
     break;
    case 1: //player per color
     playerColor->update( &strip );
     break;
    /*case 2: //ambiance animation
     ambiance->update( &strip );
     break;*/
    case 3:
     fiestaAnim->update( &strip );
     break; 
    case 5:
     rainbow->update( &strip );
     break;
    /*case 6:
     poker->update( &strip );
     break;*/
  }

  timer.update();
  
  countdown->update( &strip );
  
  showStrip();
  
  //delay(5);
}

void changeMode( int newMode )
{
  //changes the mode, can only be called inside client connection!
  
  //only save mode type changes if we changed the type
  if (newMode != mode)
  {
    prevMode = mode;
    mode = newMode;
  }
  switch( mode )
  {
    case 0:
      break;
    case 1:
      updatePlayerColors();
      break;
    /*case 2:
      updateAmbiance();
      break;*/
    case 3:
      updateFiesta();
      break;
    case 5:
      updateRainbow();
      break;
    /*case 6:
      updatePoker();*/
    default:
      break;
  }
}

void changeModeToPrevious()
{
  //stop timer
  if (fiestaTimerID >= 0)
  {
    timer.stop( fiestaTimerID );
    fiestaTimerID = -1;
  }
  
  //switches mode to previous without requiring client connection
  int prev = mode;
  mode = prevMode;
  prevMode = prev;
}
/*
void updatePoker()
{
  poker->percent = 0;
  
  int player = 0;
  for (int i=0; i<NUM_PLAYERS; ++i)
  {
    poker->colors[i] = hexToColor( stringToHex( client.readStringUntil('/') ) );
  }
  
  String inStr = client.readStringUntil('/');
  inStr.trim();
  char floatbuf[2]; // make this at least big enough for the whole string
  inStr.toCharArray(floatbuf, sizeof(floatbuf));
  poker->dealer = atof(floatbuf);
  
  inStr = client.readStringUntil('/');
  inStr.trim();
  inStr.toCharArray(floatbuf, sizeof(floatbuf));
  poker->small = atof(floatbuf);
  
  inStr = client.readStringUntil('/');
  inStr.trim();
  inStr.toCharArray(floatbuf, sizeof(floatbuf));
  poker->big = atof(floatbuf);
  
}*/

void updateRainbow()
{
  //get speed
  String speedStr = client.readStringUntil('/');
  speedStr.trim();
  char floatbuf[10]; // make this at least big enough for the whole string
  speedStr.toCharArray(floatbuf, sizeof(floatbuf));
  rainbow->percentSpeed = atof(floatbuf);
}

void updateCountdown()
{
  String timeStr = client.readStringUntil('/');
  timeStr.trim();        //kill whitespace
  
  countdown->startCountdown( timeStr.toInt() );
}

void updatePlayerColors()
{
  //Color* col;
  int player = 0;
  do
  {
    if (player >= NUM_PLAYERS)
      break;
   
    int playerLength = NUM_LEDS / NUM_PLAYERS;
    
    playerColor->colors[player] = hexToColor( stringToHex( client.readStringUntil('/') ) );
  
    /*for (int i=0; i<playerLength; ++i)
    {
      strip.setPixelColor( player*playerLength + i, col->r, col->g, col->b );
    }*/
    
    player += 1;
  }
  while( client.available());
  
  showStrip();
}
/*
void updateAmbiance()
{
  ambiance->percent = 0;
  
  //get speed
  String speedStr = client.readStringUntil('/');
  speedStr.trim();
  char floatbuf[10]; // make this at least big enough for the whole string
  speedStr.toCharArray(floatbuf, sizeof(floatbuf));
  ambiance->percentSpeed = atof(floatbuf);
  
  //get scale
  String scaleStr = client.readStringUntil('/');
  scaleStr.trim();
  scaleStr.toCharArray(floatbuf, sizeof(floatbuf));
  ambiance->scale = atof(floatbuf);
  
  ambiance->colors[0] = hexToColor( stringToHex( client.readStringUntil('/') ) );
  ambiance->colors[1] = hexToColor( stringToHex( client.readStringUntil('/') ) );
  ambiance->colors[2] = hexToColor( stringToHex( client.readStringUntil('/') ) );
  ambiance->colors[3] = hexToColor( stringToHex( client.readStringUntil('/') ) );
}
*/
void updateFiesta()
{
  if (fiestaTimerID >= 0)
    timer.stop( fiestaTimerID );
  fiestaTimerID = timer.after( 5000, changeModeToPrevious );
  
  fiestaAnim->percent = 0;
}

uint32_t stringToHex( String color )
{
  color.trim();        //kill whitespace
  // convert to packed unsigned int32
  color = "0x" + color;
  color.toCharArray(colorChars, 10);
  return strtol( colorChars, NULL, 0);
}

void showStrip()
{
  //strip.setBrightness( 200 );
  strip.show();
}






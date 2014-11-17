
#ifndef ANIMATIONS
#define ANIMATIONS

#include "Color.h"
//#include "Noise.h"
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <Console.h>

#define NUM_PLAYERS 8
#define NUM_LEDS 232

class Animation
{
public:
  float percentSpeed;
  float percent;

  void update( Adafruit_NeoPixel* strip );
};


class CountdownAnimation : public Animation
{
protected:
  Color* color;
  unsigned long timeLength;
  unsigned long startTime;
  unsigned int endBlinkTimeLength;
public:
  CountdownAnimation();
  
  void startCountdown( unsigned long len );

  void update( Adafruit_NeoPixel* strip );
};



class PlayerColorAnimation : public Animation
{
public:
  Color* colors[NUM_PLAYERS];
  Color* color;
  
  PlayerColorAnimation();

  void update( Adafruit_NeoPixel* strip );
};
/*
class PokerAnimation : public Animation
{
public:
  Color* colors[NUM_PLAYERS];
  Color* color;
  uint8_t dealer, small, big;
  
  PokerAnimation();

  void update( Adafruit_NeoPixel* strip );
};

class AmbianceAnimation : public Animation
{
public:
  float scale;
  Color* colors[4];
  Color* color;
  
  AmbianceAnimation();

  void update( Adafruit_NeoPixel* strip );
  
};
*/
class RainbowAnimation : public Animation
{
public:
  uint16_t percentPixel;
  Color* color;
  
  RainbowAnimation();

  void update( Adafruit_NeoPixel* strip );
};

class FiestaAnimation : public Animation
{
public:
  Color* colors[4];
  Color* color;
  
  FiestaAnimation();
  
  void update( Adafruit_NeoPixel* strip );
  
};

class RandomAnimation : public Animation
{
  public:
  Color* color;
  
  RandomAnimation();
  
  void update( Adafruit_NeoPixel* strip );
};

#endif

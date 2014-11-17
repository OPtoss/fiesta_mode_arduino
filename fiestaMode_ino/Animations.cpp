#include "Animations.h"

void Animation::update( Adafruit_NeoPixel* strip )
{
  
}


CountdownAnimation::CountdownAnimation()
{
  percent = 5;
  endBlinkTimeLength = 1500;
  color = new Color( 255, 255, 255);
}

void CountdownAnimation::update( Adafruit_NeoPixel* strip )
{
  if (percent < 1)
  {
    percent = (millis() - startTime) / (float)timeLength;
    if (percent >1)
      percent = 1;
  
    strip->setPixelColor( percent * strip->numPixels(), color->r, color->g, color->b );
    
    Console.println(percent);
  }
  else if (percent < 2)
  {
    //set them all red
    for (int i=0; i<strip->numPixels(); ++i)
    {
      strip->setPixelColor( i, 255,0,0 );
    }
    
    percent = (millis() - startTime - timeLength) / (float)endBlinkTimeLength;
    percent += 1; //keep it from 1-2
    
    Console.println(percent);
  }
}

void CountdownAnimation::startCountdown( unsigned long len )
{
  percent = 0;
  timeLength = len;
  startTime = millis();
  
  Console.println("start");
}




/*
PokerAnimation::PokerAnimation()
{
  percentSpeed = 0.01;
  color = new Color( 0, 0, 0);

  for (int i=0; i<NUM_PLAYERS; ++i)
  {
    colors[i] = new Color( 0, 0, 0 );
  }
}

void PokerAnimation::update( Adafruit_NeoPixel* strip )
{
  percent += percentSpeed;
  while (percent > 1)
    percent -= 1;
  
  int player = 0;
  int playerLength = NUM_LEDS / NUM_PLAYERS;
  
  float pulseStrength = sin( percent * 2 * PI ) * 0.5 + 0.5; //0-1
  
  for (int player=0; player<NUM_PLAYERS; ++player)
  {
    for (int i=0; i<playerLength; ++i)
    {
      float gradStrength = sin( (i / (float)playerLength) * PI ); //0-1
      float strength = pulseStrength * gradStrength * gradStrength * 0.6 + 0.4; //0.4-1
      
      if (player == dealer)
      {
        if (i < 0.3 * playerLength || i > 0.7 * playerLength)
          copyColor( &LIGHT_GRAY, color );
        else
          copyColor( colors[player], color );
      }
      else if (player == small)
      {
        if (i < 0.3 * playerLength || i > 0.7 * playerLength)
          copyColor( &GRAY, color );
        else
          copyColor( colors[player], color );
      }
      else if (player == big)
      {
        if (i < 0.3 * playerLength || i > 0.7 * playerLength)
          copyColor( &DARK_GRAY, color );
        else
          copyColor( colors[player], color );
      }
      else
      {
        copyColor( colors[player], color );
      }
      
      color->r = int( color->r * strength );
      color->g = int( color->g * strength );
      color->b = int( color->b * strength );
      
      strip->setPixelColor( player*playerLength + i, color->r, color->g, color->b );
    }
  }
}
*/



PlayerColorAnimation::PlayerColorAnimation()
{
  percentSpeed = 0.01;
  color = new Color( 0, 0, 0);
  
  for (int i=0; i<NUM_PLAYERS; ++i)
  {
    colors[i] = new Color( 0, 0, 0 );
  }
}

void PlayerColorAnimation::update( Adafruit_NeoPixel* strip )
{
  /*percent += percentSpeed;
  while (percent > 1)
    percent -= 1;*/
  
  int player = 0;
  int playerLength = NUM_LEDS / NUM_PLAYERS;
  
  for (int player=0; player<NUM_PLAYERS; ++player)
  {
    for (int i=0; i<playerLength; ++i)
    {
      /*
      float pulseStrength = sin( percent * 2 * PI ) * 0.5 + 0.5; //0-1
      float gradStrength = sin( (i / (float)playerLength) * PI ); //0-1
      float strength = pulseStrength * gradStrength * gradStrength * 0.6 + 0.4; //0.4-1
      
      color->r = int( colors[player]->r * strength );
      color->g = int( colors[player]->g * strength );
      color->b = int( colors[player]->b * strength );
      */
      strip->setPixelColor( player*playerLength + i, colors[player]->r, colors[player]->g, colors[player]->b );
    }
  }
}


/*
AmbianceAnimation::AmbianceAnimation()
{
  scale = 1;  
  
  for (int i=0; i<4; ++i)
  {
    colors[i] = new Color( 0, 0, 0 );
  }
  color = new Color(0,0,0);
}

void AmbianceAnimation::update( Adafruit_NeoPixel* strip )
{
  percent += percentSpeed * 1000L; //increase by 1000 to bring into range of noise function
  //TODO loop percent by period of noise function
  
  float pixelStep = 1.0 / strip->numPixels();
  
  for (int i=0; i<strip->numPixels(); ++i)
  {
    //Offset percent by pixel X period
    // TODO use actual noise period, when it is known
    // TODO add an int multiplier by the period as an input variable (maybe redundant with scale input)
    int c = 0;//renderNoise( percent, 0, i*pixelStep*(1L<<20)*scale );
    
    //int c = sin( percent + i*pixelStep )*255;
    
    //amplkitude multiplier increases contrast and causes chasing effect, and may result in banding too
    float p = (c)/255.0 * 10.0;
    
    //mod it by 4 so we don't exceed our bounds
    p = fmod( p, 4.0 );

    if (p<1)
    {
      lerpColors( colors[0], colors[1], p, color );
    }
    else if (p<2)
    {
      lerpColors( colors[1], colors[2], p-1, color );
    }
    else if (p<3)
    {
      lerpColors( colors[2], colors[3], p-2, color );
    }
    else
    {
      lerpColors( colors[3], colors[0], p-3, color );
    }  
    
    strip->setPixelColor( i, color->r, color->g, color->b );
  }
}
*/

RainbowAnimation::RainbowAnimation()
{
  percentPixel = 0;
  color = new Color(0,0,0);
  percentSpeed = 0.01;
}


void RainbowAnimation::update( Adafruit_NeoPixel* strip )
{
  //increment based on floats
  percent += percentSpeed;
  
  //animate based on integer math
  while (percent >= 1)
  {
    if (percentPixel == 255)
    {
      percentPixel = 0;
    }
    else
      percentPixel += 1;
    percent -= 1;
  }
  
  uint16_t i;
  for(i=0; i<strip->numPixels(); i++) {
    Wheel((i+percentPixel) & 255, color);
    strip->setPixelColor(i, color->r, color->g, color->b );
  }
/*
  for (int i=0; i<strip->numPixels(); ++i) //temp magic number
  {
    int h = (int) ((percent + i/(float)strip->numPixels()) * 360);
    hsi2rgb( h, 1, 1, color );
    strip->setPixelColor( i, color->r, color->g, color->b );
  }*/
}

RandomAnimation::RandomAnimation()
{
  percent = 0;
}


void RandomAnimation::update( Adafruit_NeoPixel* strip )
{
  percent += percentSpeed;
  while (percent > 1)
  {
    percent -= 1;
  }
  
  for (int i=0; i<strip->numPixels(); ++i) //temp magic number
  {
    uint8_t h = random( 0, 255 );
    //hsi2rgb( h, 1, 0.5, color );
    Wheel(h, color);
    strip->setPixelColor( i, color->r, color->g, color->b );
  }
}


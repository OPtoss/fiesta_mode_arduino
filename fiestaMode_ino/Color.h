#ifndef COLOR
#define COLOR

#include <Adafruit_NeoPixel.h>

struct Color
{
  uint8_t r, g, b;
  
  Color(){}
  Color( int red, int green, int blue )
  {
    r = red;
    g = green;
    b = blue;
  }
};

static struct Color WHITE(255,255,255);
static struct Color LIGHT_GRAY(200,200,200);
static struct Color GRAY(128,128,128);
static struct Color DARK_GRAY(50,50,50);

Color* hexToColor( uint32_t hex );

uint32_t colorToHex( Color* color );

void hsi2rgb( float h, float s, float i, Color* rgb );

void lerpColors( Color* colorA, Color* colorB, float time, Color* outColor );

void copyColor( Color* a, Color* b );

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void Wheel(uint8_t WheelPos, Color* color);

#endif

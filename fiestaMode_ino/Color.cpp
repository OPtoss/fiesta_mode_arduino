#include "Color.h"
#include <Adafruit_NeoPixel.h>

Color* hexToColor( uint32_t hex )
{
  Color* color = new Color();
  hex = (hex & 0x00FFFFFF);
  color->b = (hex & 0x0000FF); //channel Blue
  hex = (hex >> 8);
  color->g = (hex & 0x0000FF); //channel Green
  hex = (hex >> 8);
  color->r =(hex & 0x0000FF); //channel Red
  return color;
}

uint32_t colorToHex( Color* color )
{
  return Adafruit_NeoPixel::Color( color->r, color->g, color->b );
}

void hsi2rgb( float H, float S, float I, Color* rgb ) {
  uint8_t r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159f*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667f-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667f-H)));
    b = 255*I/3*(1-S);
  } else if(H < 4.188787f) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667f-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667f-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787f;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667f-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667f-H)));
    g = 255*I/3*(1-S);
  }
  rgb->r = r;
  rgb->g = g;
  rgb->b = b;
}

void lerpColors( Color* colorA, Color* colorB, float time, Color* outColor )
{
  outColor->r = colorA->r * (1-time) + colorB->r * time;
  outColor->g = colorA->g * (1-time) + colorB->g * time;
  outColor->b = colorA->b * (1-time) + colorB->b * time;
}

void copyColor( Color* a, Color* b )
{
  b->r = a->r;
  b->g = a->g;
  b->b = a->b;
}

void Wheel(uint8_t WheelPos, Color* color)
 {
  if(WheelPos < 85) {
    color->r = WheelPos * 3;
    color->g = 255 - WheelPos * 3;
    color->b = 0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   color->r = 255 - WheelPos * 3;
   color->g = 0;
   color->b = WheelPos * 3;
  } else {
   WheelPos -= 170;
   color->r = 0;
   color->g = WheelPos * 3;
   color->b = 255 - WheelPos * 3;
  }
}

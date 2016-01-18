#include <MemoryFree.h>
#include <Adafruit_NeoPixel.h>

#include <Bridge.h>
#include <Console.h>
#include <Process.h>

#include <Event.h>
#include <Timer.h>

#define PIN 6
#define NUM_PLAYERS 8
#define NUM_LEDS 232

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


unsigned long FADE_TIME = 1000;

String modeStr;
int newMode;

char cr[8], cg[8], cb[8], rgb[100];
uint8_t r, g, b, ir, ig, ib, fr, fg, fb, tr, tg, tb;
unsigned long fadeStartTime = 0;
float fadePercent = 0;
String consoleCmd;

void setup() {
  // put your setup code here, to run once:

  //strip init
  strip.begin();
  strip.setPixelColor( 0, 0xFF0000 );
  showStrip();

  // bridge init
  //Serial.begin(115200);
  Bridge.begin();
  Console.begin();

  //while(!Console); // wait for console connection
  //Console.println("Bridge began. \nConsole connection established.\n");

  //Process p;
  //p.runShellCommand("python /mnt/sda1/server/launch.py");
  //Console.println("Server has started.\n");

  r = 0;
  g = 0;
  b = 0;

  for (int i=0; i<NUM_LEDS; ++i)
  {
    strip.setPixelColor( i, 0x00FF00 );
    showStrip();
    delay(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get current color from Bridge
  Bridge.get("r", cr, 3);
  Bridge.get("g", cg, 3);
  Bridge.get("b", cb, 3);

  // Convert to uint8_t and store in input color
  sscanf(cr, "%hhu", &ir);
  sscanf(cg, "%hhu", &ig);
  sscanf(cb, "%hhu", &ib);

  // Update the strip with the current input color
  updateSolidColor();
  
  // Read console commands and act on them
  if (Console)
  {
    updateConsoleCommands();
  }

  //delay(10);
}

void updateConsoleCommands()
{
  if (Console.available() > 0)
  {
    char c = Console.read();
    if (c == '\n')
    {
      // Color request, respond with current color
      if (consoleCmd == "color")
      {
        sprintf(rgb, "(%d,%d,%d)", r, g, b);
        Console.print("color = ");
        Console.println(rgb);
      }
      // Memory request, respond with current free RAM
      else if (consoleCmd == "memory")
      {
        Console.print("freeMemory = ");
        Console.println(freeMemory());
      }
      // Input color
      else if (consoleCmd == "input")
      {
        sprintf(rgb, "(%d,%d,%d)", ir, ig, ib);
        Console.print("input = ");
        Console.println(rgb);
      }
      // From color
      else if (consoleCmd == "from")
      {
        sprintf(rgb, "(%d,%d,%d)", fr, fg, fb);
        Console.print("from = ");
        Console.println(rgb);
      }
      // To color
      else if (consoleCmd == "to")
      {
        sprintf(rgb, "(%d,%d,%d)", tr, tg, tb);
        Console.print("to = ");
        Console.println(rgb);
      }
      // Bridge chars
      else if (consoleCmd == "chars")
      {
        sprintf(rgb, "(%s,%s,%s)", cr, cg, cb);
        Console.print("chars = ");
        Console.println(rgb);
      }
      consoleCmd = "";
    }
    else 
    {
      consoleCmd += c;
    }
  }
}

void updateSolidColor()
{
  // If we detect a new input color, set from and to colors and restart fader
  if (ir != tr || ig != tg || ib != tb)
  {
    fr = r;
    fg = g;
    fb = b;
    
    tr = ir;
    tg = ig;
    tb = ib;
    
    fadeStartTime = millis();
  }

  // calculate our percentage through the fade
  fadePercent = (millis() - fadeStartTime) / (float)FADE_TIME;
  if (fadePercent > 1)
    fadePercent = 1;
  
  // Lerp the from color to the input color and store in current color
  lerpColor( fr, fg, fb, tr, tg, tb, &r, &g, &b, fadePercent );
  
  // Set the output color on all LEDs
  for (int i=0; i<NUM_LEDS; ++i)
  {
    strip.setPixelColor( i, r, g, b );
  }
  showStrip();
}

void lerpColor( uint8_t fr, uint8_t fg, uint8_t fb, uint8_t tr, uint8_t tg, uint8_t tb, uint8_t* r, uint8_t* g, uint8_t* b, float t )
{
  *r = fr * (1-t) + tr * t;
  *g = fg * (1-t) + tg * t;
  *b = fb * (1-t) + tb * t;
}

void showStrip()
{
  strip.setBrightness( 200 );
  strip.show();
}

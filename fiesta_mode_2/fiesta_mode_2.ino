#include <Bridge.h>
#include <Console.h>
#include <Process.h>

#include <Event.h>
#include <Timer.h>

#define PIN 6


String modeStr;
int newMode;

char r[8], g[8], b[8], rgb[100];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Bridge.begin();

  while(!Serial); // wait for a serial connection
  Serial.println("Bridge began. \nSerial connection established.\n");

  //Process p;
  //p.runShellCommand("python /mnt/sda1/server/launch.py");

  Serial.println("Server has started.\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  // client input
  Bridge.get("r", r, 3);
  Bridge.get("g", g, 3);
  Bridge.get("b", b, 3);
  
  sprintf(rgb, "(%s,%s,%s)", r, g, b);

  Serial.println(rgb);

  delay(100);
}



#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_24bargraph bar = Adafruit_24bargraph();
int r[12] = {0};

void setup() {
//  Serial.begin(9600);
//  Serial.println("HT16K33 Bi-Color Bargraph test");
  
  bar.begin(0x70);  // pass in the address
//
//  for (uint8_t b=0; b<24; b++ ){
//    if ((b % 3) == 0)  bar.setBar(b, LED_RED);
//    if ((b % 3) == 1)  bar.setBar(b, LED_YELLOW);
//    if ((b % 3) == 2)  bar.setBar(b, LED_GREEN);
//  }
//  bar.writeDisplay();
//  delay(500);
//  for (uint8_t b=0; b<24; b++) {
//   bar.setBar(b, LED_RED);
//   bar.writeDisplay();
//   delay(50);
//   bar.setBar(b, LED_OFF);
//   bar.writeDisplay();
// }
}


void loop() {
//  Serial.println("Begin read");
  for(int i=0 ; i<12; i++)
  {
    r[i] = digitalRead(i+2);
//    Serial.println(r[i]);
  }
 
  for (uint8_t b = 0; b < 12; b++) {
    if(r[b])
      bar.setBar(b, LED_YELLOW);
    else
      bar.setBar(b, LED_OFF);
  }
 bar.writeDisplay();
}

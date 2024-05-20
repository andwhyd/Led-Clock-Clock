// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int limit = 50;
int pos = 0;
int dir = 1;
int color[3] = { random(limit), random(limit), random(limit) };

void setup() {
  Serial.begin(9600);
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setPixelColor(0, 0, 0, 0);
  pixels.show();
}

void loop() {
  while (Serial.available() <= 0) {}
  switch (Serial.read()) {
    case 'w':
      limit += 10;
      if (limit >= 255) {
        limit = 255;
      }
      Serial.println(limit);
      break;
    case 's':
      limit -= 10;
      if (limit <= 0) {
        limit = 0;
      }
      Serial.println(limit);
      break;
    case 'r':
      pixels.clear();  // Set all pixel colors to 'off'
      pixels.setPixelColor(0, random(limit), random(limit), random(limit));
      pixels.setPixelColor(1, random(limit), random(limit), random(limit));
      pixels.setPixelColor(2, random(limit), random(limit), random(limit));
      pixels.setPixelColor(3, random(limit), random(limit), random(limit));
      pixels.setPixelColor(4, random(limit), random(limit), random(limit));
      pixels.show();  // Send the updated pixel colors to the hardware.
      Serial.println("r");
      break;
    case 'y':
      for (size_t j = 0; j < 3; j++) {
        color[j] = random(limit);
      }
      pixels.clear();
      pixels.setPixelColor(pos, color[0], color[1], color[2]);
      pixels.show();
      Serial.println("y");
      break;
    case 't':
      Serial.print("Loops: ");
      while (Serial.available() <= 0) {}
      int loops = Serial.parseInt();
      Serial.flush();
      Serial.print("Pause: ");
      while (Serial.available() <= 0) {}
      int pause = Serial.parseInt();
      for (size_t j = 0; j < loops; j++) {
        pixels.clear();
        pixels.setPixelColor(pos, color[0], color[1], color[2]);
        pixels.show();
        pos += dir;
        if (pos == NUMPIXELS - 1 || pos == 0) {
          dir *= -1;
        }
        delay(pause);
      }
      Serial.println("t");
  }
}

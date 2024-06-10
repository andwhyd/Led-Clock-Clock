#include <Adafruit_NeoPixel.h>

#define PIN 15
#define NUMPIXELS 24
#define BRIGHTNESS 200
#define PAUSE 50


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int color[3] = { random(BRIGHTNESS), random(BRIGHTNESS), random(BRIGHTNESS) };

void setup() {
  pixels.begin();
}

void loop() {
  color[0] = random(0, BRIGHTNESS);
  color[1] = random(0, BRIGHTNESS - color[0]);
  color[2] = BRIGHTNESS - (color[0] + color[1]);
  // for (size_t j = 0; j < 3; j++) {
  //   color[j] = random(BRIGHTNESS);
  // }
  for (size_t j = 0; j < NUMPIXELS; j++) {
    pixels.clear();
    pixels.setPixelColor(j, color[0], color[1], color[2]);
    pixels.show();
    delay(PAUSE);
  }
}

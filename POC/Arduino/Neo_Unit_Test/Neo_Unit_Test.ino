#include <Adafruit_NeoPixel.h>

#define PIN 28
#define NUMPIXELS 24
#define BRIGHTNESS 50
#define PAUSE 50


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int color[3] = { random(BRIGHTNESS), random(BRIGHTNESS), random(BRIGHTNESS) };

void setup() {
  pixels.begin();
}

void loop() {
  for (size_t j = 0; j < 3; j++) {
    color[j] = random(BRIGHTNESS);
  }
  for (size_t j = 0; j < NUMPIXELS; j++) {
    pixels.clear();
    pixels.setPixelColor(j, color[0], color[1], color[2]);
    pixels.show();
    delay(PAUSE);
  }
}

#ifndef CLOCK_DIGIT_H
#define CLOCK_DIGIT_H

#define NUM_CLOCKS 6

#include <Arduino.h>
#include "clock.h"  // Include the Clock class header file

extern const int handPos[][NUM_CLOCKS][3];

class ClockDigit {
private:
  Clock clocks[NUM_CLOCKS];  // Array to store the clock objects for each digit

public:
  Adafruit_NeoPixel strip;  // Neopixel
  bool complete;            // Flag to indicate whether the animation for all clocks is complete

  // Constructor
  ClockDigit(int pin);
  void setDigit(int digit);
  void update();
};

#endif
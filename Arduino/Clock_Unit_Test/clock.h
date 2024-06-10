#ifndef CLOCK_H
#define CLOCK_H

#define NUM_PIXELS 24
#define NUM_HANDS 3

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Clock {
private:
  int hands[NUM_HANDS];      // Array to store the positions of the clock hands
  int goalHands[NUM_HANDS];  // Array to store the goal positions of the clock hands

public:
  bool complete;  // Flag to indicate whether the clock animation is complete

  Clock();
  void setHands(int h1, int h2, int h3);
  void update();
  int* getHands();
};

#endif
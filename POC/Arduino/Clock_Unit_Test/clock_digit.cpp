#include "clock_digit.h"

const int handPos[][NUM_CLOCKS][3] = {
  // defintion hand positions for each digit
  {
    // 0
    { 0, 0, 6 },
    { 6, 6, 12 },
    { 6, 6, 18 },
    { 6, 6, 18 },
    { 0, 0, 18 },
    { 12, 12, 18 },
  },
  {
    // 1
    { 15, 15, 15 },
    { 6, 6, 6 },
    { 15, 15, 15 },
    { 6, 6, 18 },
    { 15, 15, 15 },
    { 18, 18, 18 },
  },
  {
    // 2
    { 0, 0, 6 },
    { 12, 12, 12 },
    { 0, 0, 18 },
    { 6, 6, 12 },
    { 0, 0, 0 },
    { 12, 12, 18 },
  },
  {
    // 3
    { 0, 0, 0 },
    { 6, 6, 12 },
    { 0, 0, 0 },
    { 6, 12, 18 },
    { 0, 0, 0 },
    { 12, 12, 18 },
  },
  {
    // 4
    { 15, 15, 15 },
    { 6, 6, 6 },
    { 0, 0, 6 },
    { 6, 12, 18 },
    { 18, 18, 18 },
    { 18, 18, 18 },
  },
  {
    // 5
    { 0, 0, 0 },
    { 6, 6, 12 },
    { 0, 0, 6 },
    { 12, 12, 18 },
    { 0, 0, 18 },
    { 12, 12, 12 },
  },
  {
    // 6
    { 0, 0, 6 },
    { 6, 6, 12 },
    { 0, 6, 18 },
    { 12, 12, 18 },
    { 0, 0, 18 },
    { 12, 12, 12 },
  },
  {
    // 7
    { 15, 15, 15 },
    { 6, 6, 6 },
    { 15, 15, 15 },
    { 6, 6, 18 },
    { 0, 0, 0 },
    { 12, 12, 18 },
  },
  {
    // 8
    { 0, 0, 6 },
    { 6, 6, 12 },
    { 0, 6, 18 },
    { 6, 12, 18 },
    { 0, 0, 18 },
    { 12, 18, 18 },
  },
  {
    // 9
    { 0, 0, 6 },
    { 6, 6, 12 },
    { 0, 0, 6 },
    { 6, 12, 18 },
    { 0, 0, 18 },
    { 12, 12, 18 },
  },
};

// Constructor
ClockDigit::ClockDigit(int pin)
  : strip(NUM_CLOCKS * NUM_PIXELS, pin, NEO_GRB + NEO_KHZ800) {
  complete = true;
  strip.begin();
  strip.show();
}

// Set the goal positions for all clocks in the digit
void ClockDigit::setDigit(int digit) {
  for (int i = 0; i < NUM_CLOCKS; i++) {
    clocks[i].setHands(handPos[digit][i][1], handPos[digit][i][1], handPos[digit][i][2]);
  }
  complete = false;
}

// Update all clocks in the digit
void ClockDigit::update() {
  complete = true;
  for (int i = 0; i < NUM_CLOCKS; i++) {
    clocks[i].update();
    if (!clocks[i].complete) {
      complete = false;
    }
  }
  if (!complete) {
    strip.clear();
    for (int i = 0; i < NUM_CLOCKS; i++) {
      for (int j = 0; j < NUM_HANDS; j++) {
        strip.setPixelColor(clocks[i].getHands()[j] + (24 * i), 50, 50, 50);
      }
    }
    strip.show();
  }
}
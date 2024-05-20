#include "clock.h"

// Constructor
Clock::Clock() {
  complete = true;
  hands[0] = 3;
  hands[1] = 3;
  hands[2] = 3;
  // Copy the initial positions to the goal positions
  memcpy(goalHands, hands, sizeof(hands));
}

// Set the goal positions of the clock hands
void Clock::setHands(int h1, int h2, int h3) {
  goalHands[0] = h1;
  goalHands[1] = h2;
  goalHands[2] = h3;
  complete = false;  // Setting new goal, animation is not complete
}

// Update the clock hands' positions towards the goal positions
void Clock::update() {
  complete = true;  // Assume animation is complete
  // Loop through each hand
  for (int i = 0; i < 3; i++) {
    if (hands[i] != goalHands[i]) {
      int cw = (goalHands[i] - hands[i] + 24) % 24;   // Clockwise distance
      int ccw = (hands[i] - goalHands[i] + 24) % 24;  // Counter-clockwise distance
      // Update the clock hand position based on the shortest distance
      if (cw <= ccw) {
        hands[i] = (hands[i] + 1) % 24;  // Move clockwise
      } else {
        hands[i] = (hands[i] - 1 + 24) % 24;  // Move counter-clockwise
      }
      complete = false;  // Animation is not complete if any hand is moving
    }
  }
}

int* Clock::getHands() {
  return hands;
}
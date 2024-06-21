#include <Arduino.h>
#include <FastLED.h>

#define BRIGHTNESS 100
#define NUM_LEDS_PER_STRIP 24
#define NUM_CLOCKS 6
#define DELAY 25  // Delay between LED updates

CRGB leds[NUM_CLOCKS][NUM_LEDS_PER_STRIP];
const uint8_t pins[NUM_CLOCKS] = {10, 11, 12, 13, 14, 15};
uint8_t current_indices[NUM_CLOCKS][3] = {{0}};
uint8_t new_indices[NUM_CLOCKS][3];
const uint8_t start_indices[NUM_CLOCKS] = {8, 14, 2, 14, 2, 20};
int8_t digit = -1;

// Digit LED locations (converted from Python)
const uint8_t digit_locations[10][6][3] = {
    {
        // 0
        {12, 12, 18},
        {6, 6, 12},
        {0, 0, 12},
        {0, 0, 12},
        {0, 0, 18},
        {0, 0, 6},
    },
    {
        // 1
        {9, 9, 9},
        {9, 9, 12},
        {9, 9, 9},
        {0, 0, 12},
        {9, 9, 9},
        {0, 0, 0},
    },
    {
        // 2
        {18, 18, 18},
        {6, 6, 12},
        {12, 12, 18},
        {0, 0, 6},
        {0, 18, 18},
        {6, 6, 6},
    },
    {
        // 3
        {18, 18, 18},
        {6, 6, 12},
        {18, 18, 18},
        {0, 6, 12},
        {18, 18, 18},
        {0, 0, 6},
    },
    {
        // 4
        {12, 12, 12},
        {12, 12, 12},
        {0, 0, 18},
        {0, 6, 12},
        {9, 9, 9},
        {0, 0, 0},
    },
    {
        // 5
        {12, 12, 18},
        {6, 6, 6},
        {0, 0, 18},
        {6, 6, 12},
        {18, 18, 18},
        {0, 0, 6},
    },
    {
        // 6
        {12, 12, 18},
        {6, 6, 6},
        {0, 12, 18},
        {6, 6, 12},
        {0, 0, 18},
        {0, 0, 6},
    },
    {
        // 7
        {18, 18, 18},
        {6, 6, 12},
        {9, 9, 9},
        {0, 0, 12},
        {9, 9, 9},
        {0, 0, 0},
    },
    {
        // 8
        {12, 12, 18},
        {6, 6, 12},
        {0, 12, 18},
        {0, 6, 12},
        {0, 0, 18},
        {0, 0, 6},
    },
    {
        // 9
        {12, 12, 18},
        {6, 6, 12},
        {0, 0, 18},
        {0, 6, 12},
        {9, 9, 9},
        {0, 0, 0},
    },
};

CRGB getRandomColor() { return CRGB(random(256), random(256), random(256)); }

void setup() {
  for (int i = 0; i < NUM_CLOCKS; i++) {
    switch (pins[i]) {
      case 10:
        FastLED.addLeds<NEOPIXEL, 10>(leds[i], NUM_LEDS_PER_STRIP);
        break;
      case 11:
        FastLED.addLeds<NEOPIXEL, 11>(leds[i], NUM_LEDS_PER_STRIP);
        break;
      case 12:
        FastLED.addLeds<NEOPIXEL, 12>(leds[i], NUM_LEDS_PER_STRIP);
        break;
      case 13:
        FastLED.addLeds<NEOPIXEL, 13>(leds[i], NUM_LEDS_PER_STRIP);
        break;
      case 14:
        FastLED.addLeds<NEOPIXEL, 14>(leds[i], NUM_LEDS_PER_STRIP);
        break;
      case 15:
        FastLED.addLeds<NEOPIXEL, 15>(leds[i], NUM_LEDS_PER_STRIP);
        break;
    }
  }
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  CRGB color = getRandomColor();  // Get a random color

  // Select a random digit (0-9)
  // digit = random(10);
  if (digit > 8) {
    digit = -1;
  }
  digit += 1;

  // Set LED locations based on the selected digit
  for (int i = 0; i < NUM_CLOCKS; i++) {
    for (int j = 0; j < 3; j++) {
      new_indices[i][j] = (digit_locations[digit][i][j] + start_indices[i]) %
                          NUM_LEDS_PER_STRIP;
    }
  }

  // Animate transition from current indices to new indices
  bool all_done = false;
  while (!all_done) {
    all_done = true;

    // Clear previous LEDs
    for (int i = 0; i < NUM_CLOCKS; i++) {
      fill_solid(leds[i], NUM_LEDS_PER_STRIP, CRGB::Black);
    }

    // Update LEDs for this step
    for (int i = 0; i < NUM_CLOCKS; i++) {
      for (int j = 0; j < 3; j++) {
        int current_pos = current_indices[i][j];
        int new_pos = new_indices[i][j];

        if (current_pos != new_pos) {
          // Calculate shortest distance in either direction
          int clockwise_distance =
              (new_pos - current_pos + NUM_LEDS_PER_STRIP) % NUM_LEDS_PER_STRIP;
          int counter_clockwise_distance =
              (current_pos - new_pos + NUM_LEDS_PER_STRIP) % NUM_LEDS_PER_STRIP;

          int distance = (clockwise_distance <= counter_clockwise_distance)
                             ? clockwise_distance
                             : -counter_clockwise_distance;
          int actual_pos = current_pos + (distance > 0 ? 1 : -1);

          if (actual_pos < 0) {
            actual_pos += NUM_LEDS_PER_STRIP;
          } else if (actual_pos >= NUM_LEDS_PER_STRIP) {
            actual_pos -= NUM_LEDS_PER_STRIP;
          }

          leds[i][actual_pos] = color;
          current_indices[i][j] = actual_pos;

          all_done = false;  // At least one LED is still moving
        } else {
          // LED is already at its target position, keep it illuminated
          leds[i][new_indices[i][j]] = color;
        }
      }
    }

    FastLED.show();
    delay(DELAY);
  }

  // Update current indices to new indices
  for (int i = 0; i < NUM_CLOCKS; i++) {
    for (int j = 0; j < 3; j++) {
      current_indices[i][j] = new_indices[i][j];
    }
  }

  delay(DELAY * 20);  // Delay before next cycle
}
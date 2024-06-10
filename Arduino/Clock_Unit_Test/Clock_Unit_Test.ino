#include "clock_digit.h"

#define UPDATE_INTERVAL 50
#define DEBOUNCE_DELAY 50
#define BUTTON_PIN 16
#define NEO_START_PIN 15
#define DIGIT_NUM 4

int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long lastUpdateTime = 0;
ClockDigit* digits[DIGIT_NUM];  // Use a pointer array

void setDigit(int digit, int idx = 0) {
  if (idx < DIGIT_NUM) {
    digits[idx]->setDigit(digit);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize each ClockDigit with its respective pin
  for (int i = 0; i < DIGIT_NUM; i++) {
    digits[i] = new ClockDigit(NEO_START_PIN + i);
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (Serial.available() > 0) {
    int command = Serial.parseInt();
    Serial.println(command);
    setDigit(command);
  }
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = currentTime;
  }
  if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        int newDigit = random(9);
        Serial.println(newDigit);
        setDigit(newDigit);
      }
    }
  }
  lastButtonState = reading;

  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    for (int i = 0; i < DIGIT_NUM; i++) {
      digits[i]->update();
    }
    lastUpdateTime = currentTime;
  }
}
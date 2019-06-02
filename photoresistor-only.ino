#include "PatternController.h"

#define PRESISTOR_PIN A0
#define RANDOM_PIN A2
#define BUTTON_PIN 10

#define MIN_LIGHT 30

volatile int f_timer=0;
PatternController *ledController;

void setup() {
  // Initialize sensor pins
  pinMode(PRESISTOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  randomSeed(analogRead(RANDOM_PIN));
  ledController = new PatternController();
}

unsigned long startMillis = 0;

bool buttonState = false;
bool checkButton() {
  bool currentButton = (digitalRead(BUTTON_PIN) == HIGH);

  // If the button state is different than last time and it is current pressed
  bool result = currentButton != buttonState && currentButton;
  // Update button state for next time
  buttonState = currentButton;

  return result;
}

void loop() {  
  // Check button
  if(checkButton()) {
    startRandomPattern();
  }

  ledController->tick(millis() - startMillis);
}

void startRandomPattern() {
  startMillis = millis();
  ledController->setType(random(0, PATTERN_COUNT));
}

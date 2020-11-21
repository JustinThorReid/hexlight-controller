#include "PatternController.h"

#define PRESISTOR_PIN 26 //Photo resistor pin
#define RANDOM_PIN 36
#define BUTTON_PIN 14

#define BRIGHTNESS_HIGH 200
#define BRIGHTNESS_MID 100
#define BRIGHTNESS_LOW 25
#define BRIGHT 3800 //4095
#define DARK 3175 //3100
#define BOUNDS 100

volatile int f_timer=0;
PatternController *ledController;

void setup() {
  // Initialize sensor pins
  pinMode(PRESISTOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  randomSeed(analogRead(RANDOM_PIN));
  ledController = new PatternController();

  startRandomPattern();
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

  ledController->setBrightness(getLightLevel());
  ledController->tick(millis() - startMillis);
}

void startRandomPattern() {
  startMillis = millis();
  ledController->setType(random(1, PATTERN_COUNT));
}

uint8_t lightLevel = 1;
uint8_t getLightLevel() {
  int light = analogRead(PRESISTOR_PIN);
  if(light < BRIGHT - BOUNDS && light > DARK + BOUNDS) lightLevel = BRIGHTNESS_MID;
  if(light > BRIGHT) lightLevel = BRIGHTNESS_HIGH;
  if(light < DARK) lightLevel = BRIGHTNESS_LOW;

  return lightLevel;
}

#include "PatternController.h"

#define PRESISTOR_PIN 13 //Photo resistor pin
#define RANDOM_PIN 36
#define BUTTON_PIN 14

#define BRIGHTNESS_HIGH 200
#define BRIGHTNESS_MID 100
#define BRIGHTNESS_LOW 25
#define BRIGHT 4000 //4095
#define DARK 3175   //3100
#define BOUNDS 200

volatile int f_timer = 0;
PatternController *ledController;

void setup()
{
  Serial.begin(115200);
  Serial.println("Startup begin");

  // Initialize sensor pins
  pinMode(PRESISTOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  randomSeed(analogRead(RANDOM_PIN));
  ledController = new PatternController();

  Serial.println("Startup start random");
  startRandomPattern();

  Serial.println("Startup complete");
}

unsigned long startMillis = 0;

bool buttonState = false;
bool checkButton()
{
  bool currentButton = (digitalRead(BUTTON_PIN) == HIGH);

  // If the button state is different than last time and it is current pressed
  bool result = (currentButton != buttonState) && currentButton;
  // Update button state for next time
  buttonState = currentButton;

  if (result)
    Serial.println("Button PRESS");

  return result;
}

void loop()
{
  Serial.println(millis());

  // Check button
  if (checkButton())
  {
    startRandomPattern();
  }

  ledController->setBrightness(getLightLevel());
  ledController->tick(millis() - startMillis);
}

void startRandomPattern()
{
  startMillis = millis();
  ledController->setType(random(1, PATTERN_COUNT));
  //ledController->setType(99);
}

uint8_t lightLevel = 1;
uint8_t getLightLevel()
{
  int light = analogRead(PRESISTOR_PIN);
  if (light < BRIGHT - BOUNDS && light > DARK + BOUNDS)
    lightLevel = BRIGHTNESS_MID;
  if (light > BRIGHT)
    lightLevel = BRIGHTNESS_HIGH;
  if (light < DARK)
    lightLevel = BRIGHTNESS_LOW;

  return lightLevel;
}

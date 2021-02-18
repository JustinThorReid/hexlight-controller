#include "src/patterns/PatternController.h"
#include "src/light-sensor.h"
#include "src/button.h"

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

void loop()
{
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

#include <ssl_client.h>

#include "secrets.h"
#include "src/patterns/PatternController.h"
#include "src/light-sensor.h"
#include "src/button.h"
#include "src/mqtt/MQTTHelper.h"
#include "src/updater/Updater.h"

PatternController *ledController;
MQTTHelper *mqtt;
Updater *updater;

void setup()
{
  Serial.begin(115200);
  Serial.println("Startup begin");

  mqtt = MQTTHelper::getInstance();
  mqtt->startConnection(WIFI_SSID, WIFI_PASS, MQTT_HOST, MQTT_USER, MQTT_PASS, MQTT_CLIENT);
  mqtt->subscribe(MQTT_TOPIC_ENABLED, MQTTQOS::AT_LEAST_ONCE);

  updater = Updater::getInstance();

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
bool isEnabled = true;

void loop()
{
  updater->loop();

  const MQTTNotification *response = mqtt->loop();
  if (response && response->topic && strcmp(response->topic, MQTT_TOPIC_ENABLED) == 0)
  {
    isEnabled = (strcmp(response->payload, "1") == 0);
  }

  // Check button
  if (checkButton())
  {
    isEnabled = true;
    startRandomPattern();
  }

  if (isEnabled)
  {
    ledController->setBrightness(getLightLevel());
    ledController->tick(millis() - startMillis);
  }
}

void startRandomPattern()
{
  startMillis = millis();
  ledController->setType(random(1, PATTERN_COUNT));
  //ledController->setType(99);
}

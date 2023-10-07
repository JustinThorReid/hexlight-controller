#include <Arduino.h>
#include <ssl_client.h>
#include <ArduinoOTA.h>

#include "secrets.h"
#include "src/patterns/PatternController.h"
#include "src/light-sensor.h"
#include "src/button.h"
#include "src/mqtt/MQTTHelper.h"

PatternController *ledController;
MQTTHelper *mqtt;

unsigned long startMillis = 0;
bool isEnabled = true;

void startRandomPattern()
{
  startMillis = millis();
  ledController->setType(random(1, PATTERN_COUNT));
  //ledController->setType(6);
}

void disable()
{
  isEnabled = false;
  ledController->unset();
}

void enable()
{
  isEnabled = true;
  startRandomPattern();
}

unsigned long lastWifiMillis = 0;
void checkWifi() {
if (WiFi.status() == WL_CONNECTED)
  Serial.println("WIFI Connected");

  if (WiFi.status() == WL_CONNECTED || WiFi.status() == WL_IDLE_STATUS) return;
  if (millis() - lastWifiMillis < 10000) return; //Don't attempt to connect too often
  
  lastWifiMillis = millis();

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname("Hex Light");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setAutoReconnect(true);
}

void setupOTA() {
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
}

bool hasStarted = false;
void checkOTA() {
  if(WiFi.status() != WL_CONNECTED) return;

  if(!hasStarted) {
    hasStarted = true;
    ArduinoOTA.begin();
    return;
  }

  ArduinoOTA.handle();
}

void setup()
{
  randomSeed(analogRead(RANDOM_PIN));
  
  Serial.begin(115200);
  Serial.println("Startup begin");
  checkWifi();

  mqtt = MQTTHelper::getInstance();
  mqtt->startConnection(MQTT_HOST, MQTT_USER, MQTT_PASS, MQTT_CLIENT);
  mqtt->subscribe(MQTT_TOPIC_ENABLED, MQTTQOS::AT_LEAST_ONCE);

  setupOTA();

  // Initialize sensor pins
  //pinMode(PRESISTOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  ledController = new PatternController();

  Serial.println("Startup start random");
  startRandomPattern();
  Serial.println("Startup complete");
}

void loop()
{
  checkWifi();
  checkOTA();

  const MQTTNotification *response = mqtt->loop();
  if (response && response->topic && strcmp(response->topic, MQTT_TOPIC_ENABLED) == 0)
  {
    if (strcmp(response->payload, "1") == 0)
    {
      enable();
    }
    else
    {
      disable();
    }
  }

  // Check button
  if (checkButton())
  {
    if (isEnabled)
    {
      disable();
    }
    else
    {
      enable();
    }
  }

  if (isEnabled)
  {
    unsigned long duration = millis() - startMillis;
    //ledController->setBrightness(getLightLevel());
    if(duration > PATTERN_TIME_MS) startRandomPattern();
    ledController->tick(duration);
  }
}



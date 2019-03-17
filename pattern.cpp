#include <FastLED.h>
#include "constants.c"
#include "pattern.h"

CHSV *hexColors;
float *hexOffsets;
float *pixelOffsets;

PatternController::PatternController(uint8_t pixelsPerHex, uint8_t hexCount) {
  this->pixelsPerHex = pixelsPerHex;
  this->hexCount = hexCount;
  this->totalPixels = pixelsPerHex * hexCount;
  this->leds = new CRGB[this->totalPixels];
  this->currentPattern = NONE;
  
  hexColors = new CHSV[this->hexCount];
  hexOffsets = new float[this->hexCount];
  pixelOffsets = new float[this->totalPixels];
  FastLED.addLeds<WS2812, LED_PIN, GRB>(this->leds, this->totalPixels);
}

void PatternController::setType(PatternType type) {
  this->currentPattern = type;

  switch(type) {
    case SOLID:
      this->PatternSolidInit();
      break;
    case SOLID_MIX:
      this->PatternSolidMixInit();
      break;
  }
  
  FastLED.show();  
}

void PatternController::tick(unsigned long milli) {
  switch(this->currentPattern) {
    case SOLID_MIX:
      this->PatternSolidMixTick(milli);
      break;
  }
  
  FastLED.show();  
}

void PatternController::unset() {
  this->currentPattern = NONE;
  FastLED.clear(true);
}



void PatternController::PatternSolidInit() {
  for(int i = 0; i < this->totalPixels; i += this->pixelsPerHex) {
    uint8_t hue = random(255);
    uint8_t sat = random(150, 255);
    uint8_t val = random(200, 255);
    
    for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
      this->leds[i+j] = CHSV(hue, sat, val);
    }
  }
}

void PatternController::PatternSolidMixInit() {  
  for(uint8_t i = 0; i < this->hexCount; i++) {
    hexColors[i] = CHSV(random(255), random(150, 255), random(200, 255));
    hexOffsets[i] = random(1000);
    
    for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
      pixelOffsets[i*this->pixelsPerHex + j] = random(250);
      this->leds[i*this->pixelsPerHex + j] = hexColors[i];
    }
  }
}
void PatternController::PatternSolidMixTick(unsigned long milli) {
  for(uint8_t i = 0; i < this->hexCount; i++) {
    float rotation1 = (milli + hexOffsets[i]) / 1000.0;
    float rotation2 = (milli + hexOffsets[i]) / 1397.0;
  
    float sVariance = (sin(rotation1) + cos(rotation2) + 2.0) / 4.0;
    float vVariance = (sin(rotation2) + cos(rotation1) + 2.0) / 4.0;
    
    for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
      float angle = pixelOffsets[i*this->pixelsPerHex + j];
      float sPcnt = (sVariance + (sin(angle) + 1) / 2.0) /2.0;
      float vPcnt = (vVariance + (sin(angle) + 1) / 2.0) /2.0;

      uint8_t h = ((int)hexColors[i].h) - (20 * sPcnt);
      uint8_t s = max(0, ((int)hexColors[i].s) - (150 * sPcnt));
      uint8_t v = max(0, ((int)hexColors[i].v) - (100 * vPcnt));

//      if(j == 0) Serial.println(s);
      this->leds[i*this->pixelsPerHex + j] = CHSV(h, s, v);
    }
  }
}

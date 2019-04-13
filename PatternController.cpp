#include <FastLED.h>
#include "constants.c"
#include "PatternController.h"
#include "PWalk.h"

#define LED_PER_HEX 12 // leds per hex
#define LINE_0_PIXEL_COUNT 5
#define LINE_1_PIXEL_COUNT LED_PER_HEX * 5
#define LINE_6_PIXEL_COUNT LED_PER_HEX * 4
#define LINE_10_PIXEL_COUNT LED_PER_HEX * 4
#define LINE_14_PIXEL_COUNT LED_PER_HEX * 5
#define LINE_19_PIXEL_COUNT LED_PER_HEX * 5

#define LINE_0_PIN 2
#define LINE_1_PIN 3
#define LINE_6_PIN 4
#define LINE_10_PIN 5
#define LINE_14_PIN 6
#define LINE_19_PIN 7

//CHSV *hexColors;
//float *hexOffsets;
//float *pixelOffsets;  
//  hexColors = new CHSV[this->hexCount];
//  hexOffsets = new float[this->hexCount];
//  pixelOffsets = new float[this->totalPixels];


PatternController::PatternController() {
  this->currentPattern = NULL;
  this->allPatterns = {
    new P_Walk(this)
  };

  // Configure FastLED
  this->leds[0] = new CRGB[LINE_0_PIXEL_COUNT];
  this->leds[1] = new CRGB[LINE_1_PIXEL_COUNT];
  this->leds[2] = new CRGB[LINE_6_PIXEL_COUNT];
  this->leds[3] = new CRGB[LINE_10_PIXEL_COUNT];
  this->leds[4] = new CRGB[LINE_14_PIXEL_COUNT];
  this->leds[5] = new CRGB[LINE_19_PIXEL_COUNT];   
  
  FastLED.addLeds<WS2812, LINE_0_PIN, GRB>(this->leds[0], LINE_0_PIXEL_COUNT);
  FastLED.addLeds<WS2812, LINE_1_PIN, GRB>(this->leds[1], LINE_1_PIXEL_COUNT);
  FastLED.addLeds<WS2812, LINE_6_PIN, GRB>(this->leds[2], LINE_6_PIXEL_COUNT);
  FastLED.addLeds<WS2812, LINE_10_PIN, GRB>(this->leds[3], LINE_10_PIXEL_COUNT);
  FastLED.addLeds<WS2812, LINE_14_PIN, GRB>(this->leds[4], LINE_14_PIXEL_COUNT);
  FastLED.addLeds<WS2812, LINE_19_PIN, GRB>(this->leds[5], LINE_19_PIXEL_COUNT);
}

void PatternController::setType(uint8_t patternId) {
  FastLED.clear(true);
  
  this->currentPattern = &allPatterns[patternId];
  this->currentPattern->init();
  
  FastLED.show();  
}

void PatternController::tick(unsigned long milli) {
  if(this->currentPattern) {
    this->currentPattern->tick(milli);
    FastLED.show();  
  }
}

void PatternController::unset() {
  this->currentPattern = NULL;
  FastLED.clear(true);
}

void PatternController::clear() {
  FastLED.clear(true);
}

void PatternController::setHex(uint8_t id, CRGB color) {
  CRGB *leds;
  uint16_t pixelStart;
  uint8_t pixelLen;

  // Lookup which pixel array to modify and where to start
  switch (id) {
    case 0:
      leds = this->leds[0];
      pixelLen = 5;
      pixelStart = 0;
      break;
    case 1 ... 5:
      leds = this->leds[1];
      pixelLen = LED_PER_HEX;
      pixelStart = (id - 1) * LED_PER_HEX;
      break;
    case 6 ... 9:
      leds = this->leds[2];
      pixelLen = LED_PER_HEX;
      pixelStart = (id - 6) * LED_PER_HEX;
      break;
    case 10 ... 13:
      leds = this->leds[3];
      pixelLen = LED_PER_HEX;
      pixelStart = (id - 10) * LED_PER_HEX;
      break;
    case 14 ... 18:
      leds = this->leds[4];
      pixelLen = LED_PER_HEX;
      pixelStart = (id - 14) * LED_PER_HEX;
      break;
    case 19 ... 23:
      leds = this->leds[5];
      pixelLen = LED_PER_HEX;
      pixelStart = (id - 19) * LED_PER_HEX;
      break;
    default:
      return;
  }

  for(uint8_t i = 0; i < pixelLen; i++) {
    leds[pixelStart + i] = color;
  }
}














// void PatternController::PatternSolidInit() {
//   for(int i = 0; i < this->totalPixels; i += this->pixelsPerHex) {
//     uint8_t hue = random(255);
//     uint8_t sat = random(150, 255);
//     uint8_t val = random(200, 255);
    
//     for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
//       this->leds[i+j] = CHSV(hue, sat, val);
//     }
//   }
// }

// void PatternController::PatternSolidMixInit() {  
//   for(uint8_t i = 0; i < this->hexCount; i++) {
//     hexColors[i] = CHSV(random(255), random(150, 255), random(200, 255));
//     hexOffsets[i] = random(1000);
    
//     for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
//       pixelOffsets[i*this->pixelsPerHex + j] = random(250);
//       this->leds[i*this->pixelsPerHex + j] = hexColors[i];
//     }
//   }
// }
// void PatternController::PatternSolidMixTick(unsigned long milli) {
//   for(uint8_t i = 0; i < this->hexCount; i++) {
//     float rotation1 = (milli + hexOffsets[i]) / 1000.0;
//     float rotation2 = (milli + hexOffsets[i]) / 1397.0;
  
//     float sVariance = (sin(rotation1) + cos(rotation2) + 2.0) / 4.0;
//     float vVariance = (sin(rotation2) + cos(rotation1) + 2.0) / 4.0;
    
//     for(uint8_t j = 0; j < this->pixelsPerHex; j++) {
//       float angle = pixelOffsets[i*this->pixelsPerHex + j];
//       float sPcnt = (sVariance + (sin(angle) + 1) / 2.0) /2.0;
//       float vPcnt = (vVariance + (sin(angle) + 1) / 2.0) /2.0;

//       uint8_t h = ((int)hexColors[i].h) - (20 * sPcnt);
//       uint8_t s = max(0, ((int)hexColors[i].s) - (150 * sPcnt));
//       uint8_t v = max(0, ((int)hexColors[i].v) - (100 * vPcnt));

// //      if(j == 0) Serial.println(s);
//       this->leds[i*this->pixelsPerHex + j] = CHSV(h, s, v);
//     }
//   }
// }

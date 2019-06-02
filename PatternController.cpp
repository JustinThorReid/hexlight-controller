#include "constants.c"
#include "PatternController.h"
#include "PWalk.h"
#include "PSolidGlow.h"
#include "PSpecks.h"
#include "PSnake.h"
#include "PFlood.h"
#include "PFloodRainbow.h"

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

PatternController::PatternController() {
  this->currentPattern = NULL;

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
  this->unset();
  
  // Create new pattern
  switch (patternId) {
    case 0:
      this->currentPattern = new P_Walk(this);
      break;
    case 1:
      this->currentPattern = new P_SolidGlow(this);
      break;
    case 2:
      this->currentPattern = new P_Specks(this);
      break;
    case 3:
      this->currentPattern = new P_Snake(this);
      break;
    case 4:
      this->currentPattern = new P_Flood(this);
      break;
    case 5:
      this->currentPattern = new P_FloodRainbow(this);
      break;
    default:
      this->currentPattern = NULL;
  }

  if(this->currentPattern) this->currentPattern->init();
  
  FastLED.show();  
}

void PatternController::tick(unsigned long milli) {
  if(this->currentPattern) {
    this->currentPattern->tick(milli);
    FastLED.show();  
  }
}

void PatternController::unset() {
  if(this->currentPattern) delete this->currentPattern;
  this->currentPattern = NULL;
  FastLED.clear(true);
}

void PatternController::clear() {
  FastLED.clear(false);
}

CRGB PatternController::getHex(uint8_t id) {
  switch (id) {
    case 0:
      return this->leds[0][0];
      break;
    case 1 ... 5:
      return this->leds[1][0];
      break;
    case 6 ... 9:
      return this->leds[2][0];
      break;
    case 10 ... 13:
      return this->leds[3][0];
      break;
    case 14 ... 18:
      return this->leds[4][0];
      break;
    case 19 ... 23:
      return this->leds[5][0];
      break;
    default:
      return this->leds[0][0];
  }
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

const uint8_t PatternController::map[][6] = {{
  10,6,1,255,19,14 //0
},{
  6,2,255,255,255,0 //1
},{
  255,255,3,255,1,6 //2
},{
  255,4,255,5,255,2 //3
},{
  255,255,255,255,3,255 //4
},{
  3,255,255,255,255,255 //5
},{
  7,255,2,1,0,10 //6
},{
  12,8,255,6,10,11 //7
},{
  255,255,9,255,7,12 //8
},{
  255,255,255,255,255,8 //9
},{
  11,7,6,0,14,15 //10
},{
  255,12,7,10,15,13 //11
},{
  255,255,8,7,11,255 //12
},{
  255,255,11,15,16,255 //13
},{
  15,10,0,19,21,23 //14
},{
  13,11,10,14,23,16 //15
},{
  255,13,15,23,17,255 //16
},{
  255,16,23,255,18,255 //17
},{
  255,17,255,255,255,255 //18
},{
  14,0,255,20,255,21 //19
},{
  19,255,255,255,255,255 //20
},{
  23,14,19,255,22,255 //21
},{
  255,21,255,255,255,255 //22
},{
  16,15,14,21,255,17 //23
}};
uint8_t* PatternController::getHexNeighboors(uint8_t id) {
  return map[id];
}

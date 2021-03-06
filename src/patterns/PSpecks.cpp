#include "PSpecks.h"

#define MIN_TIME 10
#define MAX_TIME 250
#define FADE_RATE 15

void P_Specks::tick(unsigned long milli) {
  uint8_t tickDelta = (milli - this->lastTime);
  this->lastTime = milli;
  this->delta += tickDelta;

  if(this->delta > FADE_RATE) {
    this->delta -= FADE_RATE;

    // Fade out existing lights
    for(uint8_t i = 0; i < HEX_COUNT; i++) {
      if(this->values[i]) {
        this->values[i] -= 1;
        this->parent->setHex(i, CHSV(this->colors[i], 200, this->values[i]));
      }
    }
  }

  this->nextSpeck = max(0, (int)this->nextSpeck - tickDelta);
  if (!this->nextSpeck) {
    this->nextSpeck = random(MIN_TIME, MAX_TIME);

    uint8_t id = random(0, HEX_COUNT-1);
    this->colors[id] = random(255);
    this->values[id] = random(200, 255);
    this->parent->setHex(id, CHSV(this->colors[id], 200, this->values[id]));
  }
}

void P_Specks::init() {  
  this->lastTime = 0;
  this->delta = 0;
  this->nextSpeck = 0;

  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    this->colors[i] = 0;
    this->values[i] = 0;
  }
}

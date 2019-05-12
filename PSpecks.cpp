#include "PSpecks.h"

#define MIN_TIME 100
#define MAX_TIME 1000
#define FADE_RATE 7

void P_Specks::tick(unsigned long milli) {
  uint8_t tickDelta = (milli - this->lastTime);
  this->lastTime = milli;
  this->delta += tickDelta;

  if(this->delta > FADE_RATE) {
    this->delta -= FADE_RATE;

    // Fade out existing lights
    for(uint8_t i = 0; i < HEX_COUNT; i++) {
      CHSV c = this->parent->getHex(i);

      if(c.v) {
        this->parent->setHex(i, CHSV(c.h, c.s, c.v - 1));
      }
    }
  }

  this->nextSpeck = max(0, this->nextSpeck - tickDelta);
  if (!this->nextSpeck) {
    this->nextSpeck = random(MIN_TIME, MAX_TIME);

    uint8_t id = random(0, HEX_COUNT);
    CHSV c = CHSV(random(255), random(180, 255), random(200, 255));
    this->parent->setHex(id, c);
  }
}

void P_Specks::init() {  
  this->lastTime = 0;
  this->delta = 0;
  this->nextSpeck = 0;
}

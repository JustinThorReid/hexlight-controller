#include "PSnake.h"

#define TIME 150
#define FADE_RATE 3

void P_Snake::tick(unsigned long milli) {
  uint8_t tickDelta = (milli - this->lastTime);
  this->lastTime = milli;
  this->delta += tickDelta;

  if(this->delta > FADE_RATE) {
    this->delta -= FADE_RATE;

    // Fade out existing lights
    for(uint8_t i = 0; i < HEX_COUNT; i++) {
      if(this->colors[i].v) {
        this->colors[i].v = max(0, (int)this->colors[i].v - 2);
        this->parent->setHex(i, this->colors[i]);
      }
    }
  }

  this->nextSpeck = max(0, (int)this->nextSpeck - tickDelta);
  if (!this->nextSpeck) {
    this->nextSpeck = TIME;

    // Move the point
    uint8_t* map = this->parent->getHexNeighboors(this->lastID);

    // Count options
    uint8_t count = 0;
    for(uint8_t i = 0; i < 6; i++) {
      if(i == this->excludeDirection) continue;
      if(map[i] < 255) count++;
    }

    if(count == 0) {
      this->startNew();
    } else {
      uint8_t direction = random(0, count)+1;
      for(uint8_t i = 0, count = 0; i < 6; i++) {
        if(i == this->excludeDirection) continue;
        if(map[i] < 255) count++;
        if(count == direction) {
          this->lastID = map[i];
          this->excludeDirection = (i+3) % 6;
          break;
        }
      }
    }

    this->colors[this->lastID] = CHSV(this->hue, 200, 255);
    this->parent->setHex(this->lastID, this->colors[this->lastID]);
  }
}

void P_Snake::startNew() {
  this->lastID = random(0, HEX_COUNT);
  this->excludeDirection = 255;
  this->hue = random(255);
}

void P_Snake::init() {  
  this->lastTime = 0;
  this->delta = 0;
  this->nextSpeck = 0;
  this->startNew();

  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    this->colors[i].setHSV(0,0,0);
  }
}

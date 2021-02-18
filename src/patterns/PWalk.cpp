#include "PWalk.h"

void P_Walk::tick(unsigned long milli) {
  unsigned int seconds = milli / 1000;

  if (seconds != this->lastSec) {
    this->lastSec = seconds;
    
    this->parent->clear();
    this->parent->setHex((uint8_t)(seconds % HEX_COUNT), CRGB::White);
  }
}

void P_Walk::init() {
  this->lastSec = 0;
  this->parent->setHex(0, CRGB::White);
}

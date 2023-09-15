#include "PSolidGlow.h"

void P_SolidGlow::tick(unsigned long milli) {
  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    float rotation1 = (milli + this->hexOffsets[i]*4*this->hexColors[i]) / 1000.0;
    float rotation2 = (milli + this->hexOffsets[i]*4*this->hexColors[i]) / 1397.0;
  
    float sVariance = (sin(rotation1) + cos(rotation2) + 2.0) / 4.0;
    float vVariance = (sin(rotation2) + cos(rotation1) + 2.0) / 4.0;
    
    uint8_t h = ((int)this->hexColors[i]) - (100 * sVariance);
    uint8_t s = max(0, (int)(this->hexOffsets[i] - 150 * sVariance));
    uint8_t v = max(0, (int)(200 - 10 * vVariance));

    this->parent->setHex(i, CHSV(h, 250, v));
  }
}

void P_SolidGlow::init() {  
  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    this->hexColors[i] = random(255);
    this->hexOffsets[i] = random(255);
  }
}

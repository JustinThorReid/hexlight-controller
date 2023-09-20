#include "PWave.h"

#define SPEED 15000

void P_Wave::setHexRow(float offset, uint8_t hexId) {
    const uint8_t hue = offset > 0 ? this->hueRight : this->hueLeft;

    const uint8_t value = floor((min(1.0f, abs(offset))) * 200);
    this->parent->setHex(hexId, CHSV(hue, 200, value));
}

P_Wave::P_Wave(PatternController *p): Pattern(p) {
    this->previousIteration = 0;
    this->hueLeft = random(255);  
    this->hueRight = random(255);

    if(this->wave != nullptr) delete(this->wave);
    this->wave = new Wave(SPEED, [&](float o, uint8_t h){this->setHexRow(o, h);});  
}

void P_Wave::tick(unsigned long milli) {
    const uint8_t iteration = floor(milli / (float)SPEED);

    if(iteration % 2) {
        if(iteration != this->previousIteration) {
            this->previousIteration = iteration;
            this->hueRight = this->hueLeft;
            this->hueLeft = random(255);  
        }

        this->wave->tick(milli);
    } 
}

void P_Wave::init() {  
  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    this->parent->setHex(i, CHSV(this->hueRight, 250, 200));
  }
}
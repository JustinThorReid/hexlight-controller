#include "PFlood.h"

#define SPEED 125
#define DELAY 1500

void P_Flood::tick(unsigned long milli) {
  uint8_t tickDelta = (milli - this->lastTime);
  this->lastTime = milli;
  this->nextEvent = max(0, (int)this->nextEvent - tickDelta);

  if(!this->nextEvent) {
    this->nextEvent = SPEED;

    // We ran out of hexes
    if(this->nextHex[0] == 255) {
      this->nextEvent = DELAY;
      this->color = CHSV(this->color.h + random(25,150), 255, 255);
      
      this->nextHex[0] = random(HEX_COUNT);
      for(uint8_t i = 0; i < HEX_COUNT; i++) { this->usedHex[i] = false; }
    } else {
      // Dim
      this->color.v = max(15, this->color.v - (int)ceil(sqrt(this->color.v)*2));
      this->color.h += 15; 
      this->color.s = max(0, this->color.s - (int)ceil(sqrt(this->color.s)*1.5));

      uint8_t nextNext[HEX_COUNT];
      for(int8_t i = 0; i < HEX_COUNT; i++) {
        nextNext[i] = 255;
      }
      
      for(uint8_t i = 0; i < HEX_COUNT && this->nextHex[i] != 255; i++) {
        uint8_t id = this->nextHex[i];
        this->parent->setHex(id, this->color);
        this->usedHex[id] = true;

        // Add to nextnext
        const uint8_t *map = this->parent->getHexNeighboors(id);
        for(uint8_t k = 0; k < 6; k++) {
          uint8_t nextID = map[k];

          // If the hex is black
          if(nextID != 255 && !this->usedHex[nextID]) {
            uint8_t j = 0;
            while(nextNext[j] != 255 && nextNext[j] != nextID) { j++; }
            nextNext[j] = nextID;
          }
        }
      }

      // Copy nextnext into next
      for(uint8_t i = 0; i < HEX_COUNT; i++) {
        this->nextHex[i] = nextNext[i];
      }
    }
  }
}

void P_Flood::init() {
  this->nextEvent = 0;

  // Clear next hex
  for(uint8_t i = 0; i < HEX_COUNT; i++) {
    this->nextHex[i] = 255;
    this->usedHex[i] = false;
  }

  // Pick initial
  this->nextHex[0] = random(HEX_COUNT);

  // Pick color
  this->color = CHSV(random(255), random(100, 255), 255);
}

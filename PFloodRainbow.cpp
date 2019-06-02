#include "PFloodRainbow.h"

#define SPEED 145
#define DELAY 1500

void P_FloodRainbow::tick(unsigned long milli) {
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
      this->color.h += 4; 

      uint8_t nextNext[HEX_COUNT];
      for(int8_t i = 0; i < HEX_COUNT; i++) {
        nextNext[i] = 255;
      }
      
      uint8_t max = random(1,3);
      for(uint8_t i = 0; i < HEX_COUNT && this->nextHex[i] != 255; i++) {
        uint8_t id = this->nextHex[i];

        if(i < max) {
          this->parent->setHex(id, this->color);
          this->usedHex[id] = true;
            
          // Add to nextnext
          uint8_t *map = this->parent->getHexNeighboors(id);
          for(uint8_t k = 0; k < 6; k++) {
            uint8_t nextID = map[k];

            // If the hex is black
            if(nextID != 255 && !this->usedHex[nextID]) {
              uint8_t j = 0;
              while(nextNext[j] != 255 && nextNext[j] != nextID) { j++; }
              nextNext[j] = nextID;
            }
          }
        } else {
          // If we are skipping it this time, add it to next next so we don't lose it
          if(id != 255 && !this->usedHex[id]) {
            uint8_t j = 0;
            while(nextNext[j] != 255 && nextNext[j] != id) { j++; }
            nextNext[j] = id;
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

void P_FloodRainbow::init() {
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

#ifndef _P_FLOODRAINBOW
#define _P_FLOODRAINBOW

#include "PatternController.h"
#include "Pattern.h"

class P_FloodRainbow: public Pattern {
  public:
    P_FloodRainbow(PatternController *p): Pattern(p) {
    };
    void tick(unsigned long milli);
    void init();

  private:
    unsigned long lastTime;
    uint16_t nextEvent;
    uint8_t nextHex[HEX_COUNT];
    uint8_t usedHex[HEX_COUNT];
    CHSV color;
};
#endif

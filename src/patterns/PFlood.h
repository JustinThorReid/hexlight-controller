#ifndef _P_FLOOD
#define _P_FLOOD

#include "PatternController.h"
#include "Pattern.h"

class P_Flood: public Pattern {
  public:
    P_Flood(PatternController *p): Pattern(p) {
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

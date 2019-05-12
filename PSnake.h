#ifndef _P_Snake
#define _P_Snake

#include "PatternController.h"
#include "Pattern.h"

class P_Snake : public Pattern {
  public:
	  P_Snake(PatternController *p): Pattern(p) {
    };
    void tick(unsigned long milli);
    void init();

  private:
    unsigned long lastTime;
    uint8_t delta;
    uint16_t nextSpeck;
    uint8_t lastID;
    uint8_t excludeDirection;
    uint8_t hue;

    CHSV colors[HEX_COUNT];

    void startNew();
};
#endif

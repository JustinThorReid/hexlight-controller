#ifndef _P_Specks
#define _P_Specks

#include "PatternController.h"
#include "Pattern.h"

class P_Specks : public Pattern {
  public:
	  P_Specks(PatternController *p): Pattern(p) {
    };
    void tick(unsigned long milli);
    void init();

  private:
    unsigned long lastTime;
    uint8_t delta;
    uint16_t nextSpeck;

    uint8_t colors[HEX_COUNT];
    uint8_t values[HEX_COUNT];
};
#endif

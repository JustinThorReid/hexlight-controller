#ifndef _P_SolidGlow
#define _P_SolidGlow

#include "PatternController.h"
#include "Pattern.h"

class P_SolidGlow : public Pattern {
  public:
	  P_SolidGlow(PatternController *p): Pattern(p) {
    };
    void tick(unsigned long milli);
    void init();

  private:
    uint8_t hexColors[HEX_COUNT];
    
    uint8_t hexOffsets[HEX_COUNT];
    uint8_t lastSec;
};
#endif

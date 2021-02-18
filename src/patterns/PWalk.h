#ifndef _P_WALK
#define _P_WALK

#include "PatternController.h"
#include "Pattern.h"

class P_Walk: public Pattern {
  public:
    P_Walk(PatternController *p): Pattern(p) {
      this->lastSec = 0;
    };
    void tick(unsigned long milli);
    void init();

  private:
    uint8_t lastSec;
};
#endif

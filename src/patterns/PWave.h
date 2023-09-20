#ifndef _P_WAVE
#define _P_WAVE

#include "PatternController.h"
#include "Pattern.h"
#include "Wave.h"

class P_Wave: public Pattern {
  public:
      P_Wave(PatternController *p);
      void tick(unsigned long milli);
      void init();

  private:
    void setHexRow(float offset, uint8_t hexId);
    Wave *wave;
    uint8_t hueLeft;
    uint8_t hueRight;
    uint8_t previousIteration;

};
#endif

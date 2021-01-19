#ifndef _P_Test
#define _P_Test

#include "PatternController.h"
#include "Pattern.h"

class P_Test : public Pattern
{
public:
  P_Test(PatternController *p) : Pattern(p){};
  void tick(unsigned long milli);
  void init();
};
#endif

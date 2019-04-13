#ifndef _PATTERN
#define _PATTERN

#include "PatternController.h"

class Pattern {
  public:
    Pattern(PatternController *p){
      this->parent = p;
    };
    virtual ~Pattern(){};

    /// milli is the number of milliseconds since pattern start
    virtual void tick(unsigned long milli) = 0;
    virtual void init() = 0;
  
  protected:
    PatternController *parent;
};
#endif

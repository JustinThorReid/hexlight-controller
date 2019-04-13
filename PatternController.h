#ifndef _PATTERN_C
#define _PATTERN_C

#define HEX_COUNT 24
#define LINE_COUNT 6

#include <FastLED.h>
class Pattern;

class PatternController {
  public:
    PatternController();
  
    void setType(uint8_t patternId);
    void tick(unsigned long milli);
    void unset();
  
    /// Set a hex to a solid color
    void PatternController::setHex(uint8_t id, CRGB color);
    /// Set all leds to black
    void PatternController::clear();
  
  private:
    Pattern *allPatterns;
    CRGB *leds[LINE_COUNT];
    Pattern *currentPattern;
};
#endif

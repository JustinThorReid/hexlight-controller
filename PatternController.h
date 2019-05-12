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
  
    /// Get a hex color (based on first LED)
    CRGB PatternController::getHex(uint8_t id);
    
    /// Set a hex to a solid color
    void PatternController::setHex(uint8_t id, CRGB color);
    /// Set all leds to black
    void PatternController::clear();
    /// Gets all neighboring HEX ids, 255 is none
    uint8_t* PatternController::getHexNeighboors(uint8_t id);
  
  private:
    CRGB *leds[LINE_COUNT];
    Pattern *currentPattern;
    const static uint8_t map[][6];
};
#endif

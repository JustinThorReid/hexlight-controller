#ifndef _PATTERN_C
#define _PATTERN_C

#define HEX_COUNT 24
#define LINE_COUNT 6

#define PATTERN_COUNT 7

#include <FastLED.h>

class Pattern;

class PatternController
{
public:
  PatternController();

  void setBrightness(uint8_t level);
  void setType(uint8_t patternId);
  /** Update the design for the current frame, milli should be time since pattern start not system start. */
  void tick(unsigned long milli);
  void unset();

  /// Set a hex to a solid color
  void setHex(uint8_t id, CRGB color);
  /// Set all leds to black
  void clear();
  /// Gets all neighboring HEX ids, 255 is none
  const uint8_t *getHexNeighboors(uint8_t id);

private:
  CRGB *leds[LINE_COUNT];
  Pattern *currentPattern;
  const static uint8_t map[][6];
};
#endif

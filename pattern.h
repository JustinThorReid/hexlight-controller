#ifndef _PATTERN
#define _PATTERN

#include <FastLED.h>

enum PatternType {
  NONE,
  SOLID,
  SOLID_MIX
};

class PatternController {
  public:
  PatternController(uint8_t pixelsPerHex, uint8_t hexCount);

  void setType(PatternType type);
  void tick(unsigned long milli);
  void unset();

  private:
  uint8_t pixelsPerHex;
  uint8_t hexCount;
  uint32_t totalPixels;
  CRGB *leds;

  PatternType currentPattern;

  void PatternSolidInit();
  void PatternSolidMixInit();
  void PatternSolidMixTick(unsigned long milli);
};
#endif

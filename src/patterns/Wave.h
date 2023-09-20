#ifndef _WAVE
#define _WAVE

#include <functional>

#define WAVE_COLUMNS 9
#define WAVE_ROWS 5

/** Pattern utility function that will call a function for each hex in the grid row by row creating a horizontal progression.
 *  The pattern does not wrap, it will instantly restart at row 0
 * 
 * @param callback Called for each hex during a tick. Offset is between [-9, 9] where 0 is the current column -0.5 is half off to the left
 */
class Wave {
  public:
    /** @param speed How many mili per cycle*/
    Wave(unsigned int speed, std::function<void (float offset, uint8_t hexId)> callback) {
      this->speed = speed;
      this->callback = callback;
    };
    /** Call each tick with the time */
    void tick(unsigned long milli);

  private:
    unsigned int speed;
    std::function<void (float, uint8_t)> callback;
};
#endif

#include "PTest.h"

void P_Test::tick(unsigned long milli)
{
}

void P_Test::init()
{
  float step = 255.0f / (float)HEX_COUNT;

  for (int i = 0; i < HEX_COUNT; i++)
    this->parent->setHex(i, CHSV(step * i, 250, 200));
}

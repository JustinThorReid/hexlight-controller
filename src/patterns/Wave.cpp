#include <cmath>

#include "Wave.h"

const uint8_t hexRowMap[9][5] = {{
    255, 18, 255, 255, 255
}, {
    255, 17, 255, 22, 255
}, {
    16, 23, 21, 255, 255
}, {
    13, 15, 14, 19, 20
}, {
    11, 10, 0, 255, 255
}, {
    12, 7, 6, 1, 255
}, {
    8, 255, 2, 255, 255
}, {
     255, 9,  255, 3, 5
}, {
     255,  255, 4,  255,  255
}};

void Wave::tick(unsigned long milli) {
    const float percentComplete = (milli % this->speed) / (float)this->speed;
    const float column = percentComplete * 10.0f - 1;

    for(uint8_t x = 0; x < 9; x++) {
        const float rowOffset = x - column;
        for(uint8_t y = 0; y < 5; y++) {
            if(hexRowMap[x][y] != 255)
                this->callback(rowOffset, hexRowMap[x][y]);
        }
    }
}
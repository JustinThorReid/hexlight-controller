#ifndef _LIGHT_SENSOR
#define _LIGHT_SENSOR

#include <arduino.h>
#include "../constants.c"

#define BRIGHTNESS_HIGH 200
#define BRIGHTNESS_MID 50
#define BRIGHTNESS_LOW 5

#define BRIGHT 4000 //4095
#define DARK 2275   //3100
#define BOUNDS 200

uint8_t lightLevel = 1;
uint8_t getLightLevel()
{
    int light = analogRead(PRESISTOR_PIN);
    if (light < BRIGHT - BOUNDS && light > DARK + BOUNDS)
        lightLevel = BRIGHTNESS_MID;
    if (light > BRIGHT)
        lightLevel = BRIGHTNESS_HIGH;
    if (light < DARK)
        lightLevel = BRIGHTNESS_LOW;

    return lightLevel;
}

#endif
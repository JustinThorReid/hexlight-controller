#ifndef _BUTTON
#define _BUTTON

#include <arduino.h>
#include "../constants.c"

bool buttonState = false;
bool checkButton()
{
    bool currentButton = (digitalRead(BUTTON_PIN) == HIGH);

    // If the button state is different than last time and it is current pressed
    bool result = (currentButton != buttonState) && currentButton;
    // Update button state for next time
    buttonState = currentButton;

    return result;
}

#endif
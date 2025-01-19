#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <FastLED.h>

// Button press handler functions
void IRAM_ATTR buttonISR();
void handleButtonPress();
void updateLEDStrip();

// Getter for current mode
int getCurrentMode();

#endif

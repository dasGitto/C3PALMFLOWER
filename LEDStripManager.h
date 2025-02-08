#ifndef LED_STRIP_MANAGER_H
#define LED_STRIP_MANAGER_H

#include <FastLED.h>

void updateLEDStrip();
void nextMode();
void nextPage();
void setMode(int modeNum);
void turnOn();
void turnOff();
int getCurrentMode();

#endif
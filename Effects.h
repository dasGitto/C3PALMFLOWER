#ifndef EFFECTS_H
#define EFFECTS_H

#include <FastLED.h>

#define NUM_LEDS 16
extern CRGB leds[NUM_LEDS];

// Effects declarations
void fireEffect();
void meteorEffect();
void strobeEffect();
void glowEffect();
void randomEffect();

#endif

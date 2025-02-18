#ifndef SETTINGS_H
#define SETTINGS_H

#include <FastLED.h>

#define LED_PIN D0
#define NUM_LEDS 30
#define BUTTON_PIN D10

extern CRGB leds[NUM_LEDS];

#define UDP_PORT 4210
#define BROADCAST_IP "255.255.255.255"

#endif

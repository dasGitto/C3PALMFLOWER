#include "Effects.h"
#include <FastLED.h>

uint8_t heat[NUM_LEDS];

void fireEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i], random8(0, 10));
    }

    for (int i = NUM_LEDS - 1; i >= 2; i--) {
        heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    if (random8() < 120) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    if (random8() < 50) {
        int sparkPos = random8(0, NUM_LEDS);
        leds[sparkPos] = CRGB(random8(180, 255), random8(100, 180), 0);
    }

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = HeatColor(heat[i]);
    }

    Serial.println("FIRE NOW");
}

void meteorEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 255, 0);
    }
}

void strobeEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 255);
    }
}

void glowEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 0);
    }
}

void randomEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(random(0, 255), random(0, 255), random(0, 255));
    }
}

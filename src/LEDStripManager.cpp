#include "LEDStripManager.h"
#include <FastLED.h>
#include "Settings.h"

extern CRGB leds[NUM_LEDS];

enum Mode { 
  FIRE,
  METEOR,
  STROBE,
  GLOW,
  RANDOM 
};

Mode currentMode = FIRE;
uint8_t heat[NUM_LEDS];

void nextMode() { currentMode = static_cast<Mode>((currentMode + 1) % 5); }
void nextPage() { /* Implement page change logic */ }
void setMode(int modeNum) { currentMode = static_cast<Mode>(modeNum); }
void turnOn() { /* Implement LED on behavior */ }
void turnOff() { FastLED.clear(); FastLED.show(); }

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

void updateLEDStrip() {
  FastLED.clear();

  if (currentMode == FIRE) fireEffect();
  else if (currentMode == METEOR) meteorEffect();
  else if (currentMode == STROBE) strobeEffect();
  else if (currentMode == GLOW) glowEffect();
  else if (currentMode == RANDOM) randomEffect();

  FastLED.show();
}

int getCurrentMode() {
  return currentMode;
}

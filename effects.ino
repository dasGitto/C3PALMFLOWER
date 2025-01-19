#include <FastLED.h>

// Declare the heat array if it is not already declared in another file
extern uint8_t heat[NUM_LEDS];  // External declaration if it's in another file

// Declare the LED array using FastLED
extern CRGB leds[NUM_LEDS];

void fireEffect() {
  // Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, 10));  // Gradually reduce heat
  }

  // Heat from each cell drifts 'up' and diffuses a little
  for (int i = NUM_LEDS - 1; i >= 2; i--) {
    heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;  // Spread heat upward
  }

  // Randomly ignite new sparks near the bottom (add some randomness for "sparkles")
  if (random8() < 120) {
    int y = random8(7);  // Random position near the bottom
    heat[y] = qadd8(heat[y], random8(160, 255));  // Add intense heat (bright spark)
  }

  // Simulate sparkling effect by randomly adding some very bright pixels
  if (random8() < 50) {  // Random chance for a sparkle
    int sparkPos = random8(0, NUM_LEDS); // Random position for the sparkle
    leds[sparkPos] = CRGB(random8(180, 255), random8(100, 180), 0); // Sparkle color (orange to yellow)
  }

  // Map heat values to colors using FastLED's HeatColor function
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = HeatColor(heat[i]);  // Map heat to color (fire-like gradient)
  }

  FastLED.show();  // Update the LED strip
}

void meteorEffect();
void strobeEffect();
void glowEffect();
void randomEffect();

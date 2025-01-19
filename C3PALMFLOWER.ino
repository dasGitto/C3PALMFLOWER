#include <Arduino.h>
#include <FastLED.h>
#include "ButtonHandler.h"
#include "Effects.h"

// Define pins and number of LEDs
#define BUTTON_PIN D0 // Pin for the button
#define PIN D10       // Pin for the LED strip
#define NUM_LEDS 16   // Number of LEDs in the strip

// Create an array to represent the LED strip
CRGB leds[NUM_LEDS];

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(100);
    Serial.println("Serial monitor initialized");

    // Configure button pin and attach interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);

    // Initialize the FastLED library
    FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();  // Turn off all LEDs initially
    FastLED.show();

    // Check if waking up from deep sleep
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        updateLEDStrip();  // Restore the last LED state
        Serial.println("Wakeup from sleep");
        Serial.println("Current mode is " + String(getCurrentMode())); // Get the current mode
    }
}

void loop() {
    // Handle button press and update LED strip accordingly
    handleButtonPress();
}

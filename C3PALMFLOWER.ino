#include <FastLED.h>
#include <Arduino.h>

#define BUTTON_PIN D0 // Pin for the button
#define PIN D10        // Pin for the LED strip
#define NUM_LEDS 16   // Number of LEDs in the strip

// Declare the LED array using FastLED
CRGB leds[NUM_LEDS];  

extern void handleButtonPress(); // Declare the function from another file
extern void updateLEDStrip();    // Declare the update function from another file
extern void IRAM_ATTR buttonISR(); 

void setup() {
    // Initial serial communication at 115200 baud
    Serial.begin(115200);
    delay(100);
    Serial.println("Serial monitor initialized");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);

    FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS);  // Initialize the FastLED strip
    FastLED.clear();  // Turn off all LEDs initially
    FastLED.show();

    // Check if waking up from deep sleep
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        updateLEDStrip(); // Restore the last LED state
        Serial.println("Wakeup from sleep");
        Serial.println("Current mode is " + "<get currentMode here>")
    }
}

void loop() {
    handleButtonPress(); // Call the handler from the other file
}

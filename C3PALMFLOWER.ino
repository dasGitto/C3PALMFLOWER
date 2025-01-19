#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define BUTTON_PIN D0 // Pin for the button
#define PIN D7        // Pin for the LED strip
#define NUM_LEDS 16  // Number of LEDs in the strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

extern void handleButtonPress(); // Declare the function from another file
extern void updateLEDStrip();    // Declare the update function from another file
extern void IRAM_ATTR buttonISR();

void setup() {
    // initial serial communication at 115200 baud
    Serial.begin(115200);
    delay(100);
    Serial.println ("Serial monitor initialized");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);

    strip.begin();  // Initialize the NeoPixel strip
    strip.show();   // Set all pixels to 'off'

    // Check if waking up from deep sleep
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        updateLEDStrip(); // Restore the last LED state
        Serial.println("Wakeup from sleep");
    }
}

void loop() {
    handleButtonPress(); // Call the handler from the other file
}
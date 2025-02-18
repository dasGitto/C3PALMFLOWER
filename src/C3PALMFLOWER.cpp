#include <Arduino.h>
#include <FastLED.h>
#include "ButtonHandler.h"
#include "LEDStripManager.h"
#include "SyncManager.h"
#include "Settings.h"

// Create an array to represent the LED strip
CRGB leds[NUM_LEDS];

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(100);
    Serial.println("Serial monitor initialized");

    // Connect to WiFi
    startSyncManager();

    // Configure button pin and attach interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);

    // Initialize the FastLED library
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
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
        // 🔹 Detect button input first
    ActionType buttonAction = detectButtonInput();
    if (buttonAction != NONE) {
        switch (buttonAction) {
            case SINGLE_TAP:
                nextMode();
                break;
            case DOUBLE_TAP:
                nextPage();
                break;
            case HOLD_3S:
                toggleSyncMode();  // Toggle sync mode instead of LED update
                break;
            case HOLD_10S:
                turnOff();
                break;
            default:
                break;
        }
    }

    // 🔹 Then check for network sync input
    SyncAction syncAction;
    int modeNum;
    syncAction = readNetworkInput();

    if (syncAction == SET_MODE) {
        setMode(modeNum);
    }

    if (buttonAction || syncAction) {
      updateLEDStrip();
    }
}

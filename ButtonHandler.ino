#include <Arduino.h>
#include <esp_sleep.h>
#include <Adafruit_NeoPixel.h>

// Enum for modes
enum Mode { FIRE, METEOR, STROBE, GLOW, RANDOM };

// Shared constants
const int DEBOUNCE_DELAY = 200;         // Debounce delay in ms
const unsigned long SLEEP_HOLD_TIME = 3000; // Time to trigger deep sleep in ms
const unsigned long WAKE_HOLD_TIME = 2000;  // Time to wake from deep sleep

// State variables
volatile unsigned long lastPressTime = 0;
volatile unsigned long lastReleaseTime = 0;
volatile bool buttonPressed = false;
volatile unsigned long buttonHoldStart = 0;
volatile int currentPage = 0;
volatile int currentMode = FIRE;
volatile int pressCount = 0;

extern Adafruit_NeoPixel strip; // Use the strip from Base.ino

void IRAM_ATTR buttonISR() {
    unsigned long now = millis();

    if (digitalRead(BUTTON_PIN) == LOW) {
        // Button pressed
        if (now - lastReleaseTime > DEBOUNCE_DELAY) {
            buttonPressed = true;
            lastPressTime = now;
            pressCount++;
            buttonHoldStart = now;

            
        }
    } else {
        // Button released
        lastReleaseTime = now;
        buttonHoldStart = 0;
    }
}

void handleButtonPress() {
    unsigned long now = millis();

    // Check if the button is being held
    if (buttonHoldStart > 0 && (now - buttonHoldStart > SLEEP_HOLD_TIME)) {
        strip.clear();        // Turn off the LED strip
        strip.show();
        Serial.println("Going to sleep");
        // Enable wakeup on BUTTON_PIN for a falling edge (button press)
        esp_deep_sleep_enable_gpio_wakeup(1ULL << BUTTON_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
        esp_deep_sleep_start();
    }

    if (buttonPressed) {
        buttonPressed = false;

        // Handle double press
        if (pressCount == 2 && (now - lastPressTime <= DEBOUNCE_DELAY)) {
            currentPage = (currentPage + 1) % 3; // Assume 3 pages
            pressCount = 0;
            Serial.println("Dobule tap");
        }
        // Handle single press
        else if (pressCount == 1 && (now - lastPressTime <= DEBOUNCE_DELAY)) {
            currentMode = (currentMode + 1) % 5; // Assume 5 modes per page
            pressCount = 0;
            
            Serial.println("Single tap");
        }

        updateLEDStrip();
    }
}

void updateLEDStrip() {
    // Logic to update the LED strip based on the current state
    strip.clear();
    for (int i = 0; i < strip.numPixels(); i++) {
        if (currentMode == FIRE) strip.setPixelColor(i, strip.Color(255, 0, 0));
        else if (currentMode == METEOR) strip.setPixelColor(i, strip.Color(0, 255, 0));
        else if (currentMode == STROBE) strip.setPixelColor(i, strip.Color(0, 0, 255));
        else if (currentMode == GLOW) strip.setPixelColor(i, strip.Color(255, 255, 0));
        else if (currentMode == RANDOM) strip.setPixelColor(i, random(0, 255), random(0, 255), random(0, 255));
    }
    strip.show();
}
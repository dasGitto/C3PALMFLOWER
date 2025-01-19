#include "ButtonHandler.h"
#include <Arduino.h>
#include <FastLED.h>
#include "Effects.h"

#define BUTTON_PIN D0
#define DEBOUNCE_DELAY 200
#define SLEEP_HOLD_TIME 3000
#define WAKE_HOLD_TIME 2000

// Modes
enum Mode { FIRE, METEOR, STROBE, GLOW, RANDOM };

// State variables
volatile unsigned long lastPressTime = 0;
volatile unsigned long lastReleaseTime = 0;
volatile bool buttonPressed = false;
volatile unsigned long buttonHoldStart = 0;
volatile int currentPage = 0;
volatile int currentMode = FIRE;
volatile int pressCount = 0;

extern CRGB leds[NUM_LEDS];

void IRAM_ATTR buttonISR() {
    unsigned long now = millis();

    if (digitalRead(BUTTON_PIN) == LOW) {
        if (now - lastReleaseTime > DEBOUNCE_DELAY) {
            buttonPressed = true;
            lastPressTime = now;
            pressCount++;
            buttonHoldStart = now;
        }
    } else {
        lastReleaseTime = now;
        buttonHoldStart = 0;
    }
}

void handleButtonPress() {
    unsigned long now = millis();

    if (buttonHoldStart > 0 && (now - buttonHoldStart > SLEEP_HOLD_TIME)) {
        FastLED.clear();
        FastLED.show();
        Serial.println("Going to sleep");
        esp_deep_sleep_enable_gpio_wakeup(1ULL << BUTTON_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
        esp_deep_sleep_start();
    }

    if (buttonPressed) {
        buttonPressed = false;

        if (pressCount == 2 && (now - lastPressTime <= DEBOUNCE_DELAY)) {
            currentPage = (currentPage + 1) % 3;
            pressCount = 0;
            Serial.println("Double tap");
        } else if (pressCount == 1 && (now - lastPressTime <= DEBOUNCE_DELAY)) {
            currentMode = (currentMode + 1) % 5;
            pressCount = 0;
            Serial.println("Single tap");
        }

        updateLEDStrip();
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

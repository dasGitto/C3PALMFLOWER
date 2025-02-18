#include "ButtonHandler.h"
#include <Arduino.h>
#include "esp_sleep.h"
#include <FastLED.h>
#include "SyncManager.h"
#include "Settings.h"

#define DEBOUNCE_DELAY 200
#define SYNC_HOLD_TIME 3000
#define SLEEP_HOLD_TIME 10000
#define WAKE_HOLD_TIME 2000

// State variables
volatile unsigned long lastPressTime = 0;
volatile unsigned long lastReleaseTime = 0;
volatile bool buttonPressed = false;
volatile unsigned long buttonHoldStart = 0;
volatile int pressCount = 0;

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

ActionType detectButtonInput() {
    unsigned long now = millis();

    if (buttonHoldStart > 0 && (now - buttonHoldStart > SLEEP_HOLD_TIME)) {
        FastLED.clear();
        FastLED.show();
        Serial.println("Going to sleep");
        esp_deep_sleep_enable_gpio_wakeup((1ULL << BUTTON_PIN), ESP_GPIO_WAKEUP_GPIO_LOW);
        esp_deep_sleep_start();
        return HOLD_10S;
    } else if (buttonHoldStart > 0 && (now - buttonHoldStart > SYNC_HOLD_TIME)) {
        return HOLD_3S;
    }

    if (buttonPressed) {
        buttonPressed = false;

        if (pressCount == 2) {
            pressCount = 0;
            return DOUBLE_TAP;
        } else if (pressCount == 1) {
            pressCount = 0;
            return SINGLE_TAP;
        }
    }

    return NONE;
}

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

enum ActionType { NONE, SINGLE_TAP, DOUBLE_TAP, HOLD_2S, HOLD_3S, HOLD_10S };

void IRAM_ATTR buttonISR();
ActionType detectButtonInput();

#endif

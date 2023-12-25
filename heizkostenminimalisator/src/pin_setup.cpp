#include "pin_setup.h"
#include <Arduino.h>

void setup_pins(){
    pinMode(PIN_OVEN_DOOR, INPUT_PULLUP);
}
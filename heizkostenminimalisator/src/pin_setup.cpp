#include "pin_setup.h"
#include <Arduino.h>

void setup_pins(){
    pinMode(PIN_STOVE_IN, INPUT_PULLUP);
}
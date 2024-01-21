#include "state_definitions.h"


void print_state(uint8_t state){
    switch (state)
    {
    case STATE_NORMAL_BOOT:
        Serial.println("NORMAL_BOOT");
        break;
    case STATE_READ_T:
        Serial.println("READ_T");
        break;
    case STATE_DOOR_OPENED:
        Serial.println("DOOR_OPENED");
        break;
    case STATE_BURNING:
        Serial.println("BURNING");
        break;
    case STATE_BURN_OFF:
        Serial.println("BURN_OFF");
        break;
    case STATE_COOL_DOWN:
        Serial.println("COOL_DOWN");
        break;
    default:
        Serial.println("UNDEFINED");
        break;
    }
}

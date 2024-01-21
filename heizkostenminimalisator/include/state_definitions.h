#ifndef STATE_DEFINITIONS_H
#define STATE_DEFINITIONS_H

#define STATE_NORMAL_BOOT 0U
#define STATE_READ_T 2U
#define STATE_DOOR_OPENED 3U
#define STATE_BURNING 4U
#define STATE_BURN_OFF 5U
#define STATE_COOL_DOWN 6U
#include <Arduino.h>
void print_state(uint8_t state);


#endif
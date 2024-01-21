#ifndef STATE_VAR_STRUCT_H
#define STATE_VAR_STRUCT_H
#include "state_definitions.h"
#include <Arduino.h>

#define NUM_TEMP_MEASUREMENTS 5
//// TODO: Adapt:
#define TEMP_CHECK_PERIOD 5*60  // in seconds -> every 5 Minutes the temperature should be checked if higher than X degrees. This is just for extra safety.
//////////

typedef struct StateVariables{ //this has to be declared as RTC_DATA_ATTR!!
    hw_timer_t * thermo_timer;
    hw_timer_t * sleep_timer;
    unsigned char state = STATE_NORMAL_BOOT; // state_variable
    bool task_measure_temperature; // this indicates a task (read temp), has to be done, regardless of the cur state
    //bool setup;
    double highest_temperature = -1; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
    double temperature_measurements[NUM_TEMP_MEASUREMENTS]; // This array gets updated every five minutes with the measured temperature
    double temperature_closure_slope = 0; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
    double temperature_closure_offset = 0; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
} StateVariables;

#endif
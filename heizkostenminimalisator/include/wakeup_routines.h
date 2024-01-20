#ifndef WAKEUP_ROUTINES_H
#define WAKEUP_ROUTINES_H
#include <Arduino.h>
#include "state_definitions.h"
#define INTR_READ_T 1U
#define INTR_OVEN_DOOR 2U
#define INTR_UNDEFINED 3U
#define NUM_TEMP_MEASUREMENTS 5
#define BTN_STOP_ALARM    0

//// TODO: Adapt:
#define TEMP_CHECK_PERIOD 5*60  // in seconds -> every 5 Minutes the temperature should be checked if higher than X degrees. This is just for extra safety.
//////////

typedef struct StateVariables{ //this has to be declared as RTC_DATA_ATTR!!
    hw_timer_t * thermo_timer;
    hw_timer_t * sleep_timer;
    //SemaphoreHandle_t timerSemaphore;
    //portMUX_TYPE state_mux;
    //uint32_t isr_counter;
    unsigned char state = STATE_NORMAL_BOOT; // state_variable
    bool task_measure_temperature; // this indicates a task (read temp), has to be done, regardless of the cur state
    //bool setup;
    double highest_temperature = -1; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
    double temperature_measurements[NUM_TEMP_MEASUREMENTS]; // This array gets updated every five minutes with the measured temperature
    double temperature_closure_slope = 0; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
    double temperature_closure_offset = 0; // this variable gets reset, every time the door opens or the ESP starts in NORMAL_BOOT
} StateVariables;

extern RTC_DATA_ATTR StateVariables state_variables;


void IRAM_ATTR onTimer();
void IRAM_ATTR sleepTimerISR();

void setupWakeUpRoutines();
void delayedSleepEnable(int32_t delay_sec); // sets up a timer, which when triggered sets the ESP to deepsleep
void delayedSleepDisable(); // disables the sleep timer
bool delayedSleepIsSet(); //returns true, if a timer has already been set
//void append_intermediate_task(uint8_t intermediate_task);
#endif
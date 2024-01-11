#ifndef WAKEUP_ROUTINES_H
#define WAKEUP_ROUTINES_H
#include <Arduino.h>
#define STATE_NORMAL_BOOT 0U
#define INTR_READ_T 1U
#define INTR_OVEN_DOOR 2U
#define INTR_UNDEFINED 3U
#define TEMP_CHECK_PERIOD 30  // in seconds -> every 60 Minutes the temperature should be checked if higher than X degrees. This is just for extra safety.
#define NUM_TEMP_MEASUREMENTS 5
#define BTN_STOP_ALARM    0

typedef struct StateVariables{ //this has to be defined as RTC_DATA_ATTR!!
    hw_timer_t * thermo_timer;
    //SemaphoreHandle_t timerSemaphore;
    //portMUX_TYPE state_mux;
    //uint32_t isr_counter;
    unsigned char state = STATE_NORMAL_BOOT; // state_variable
    bool setup;
    double temperature_measurements[NUM_TEMP_MEASUREMENTS] = {0,0,0,0,0}; // This array gets updated every five minutes with the measured temperature
} StateVariables;


void IRAM_ATTR onTimer();
void IRAM_ATTR sleepTimerISR();

void setupWakeUpRoutines(StateVariables *state_vars);
void sleepysloopy();

#endif
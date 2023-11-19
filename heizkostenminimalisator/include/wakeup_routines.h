#ifndef WAKEUP_ROUTINES_H
#define WAKEUP_ROUTINES_H
#include <Arduino.h>
#define STATE_NORMAL_BOOT 0U
#define STATE_TEMP_CHECK 1U
#define STATE_OVEN_DOOR 2U
#define SELF_CHECK_RATE 10  // in seconds -> every 10 Minutes the temperatur should be checked if higher than X degrees. This is just for extra safety.

#define BTN_STOP_ALARM    0

struct WakeUpSettings{ //this has to be defined as RTC_DATA_ATTR!!
    hw_timer_t * thermo_timer;
    SemaphoreHandle_t timerSemaphore;
    portMUX_TYPE state_mux;
    uint32_t isr_counter;
    unsigned char state; // pointer to the state_variable
    bool setup;
};


void ARDUINO_ISR_ATTR onTimer();
unsigned char handleWakeupRoutines();
void setupWakeUpRoutines(WakeUpSettings* w_u_settings);

#endif
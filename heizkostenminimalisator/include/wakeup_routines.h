#ifndef WAKEUP_ROUTINES_H
#define WAKEUP_ROUTINES_H
#include <Arduino.h>
#include <esp_sleep.h>
#include "driver/rtc_io.h"

#include "state_definitions.h"
#include "state_var_struct.h"
#include "pin_setup.h"
#include "thermocouple.h"

#define INTR_READ_T 1U
#define INTR_OVEN_DOOR 2U
#define INTR_UNDEFINED 3U
#define BTN_STOP_ALARM    0




//extern RTC_DATA_ATTR StateVariables state_variables;
//RTC_DATA_ATTR StateVariables state_variables;

void IRAM_ATTR onTimer();
void IRAM_ATTR sleepTimerISR();

void setupWakeUpRoutines();
StateVariables* get_state_vars();
void delayedSleepEnable(int32_t delay_sec); // sets up a timer, which when triggered sets the ESP to deepsleep
void delayedSleepDisable(); // disables the sleep timer
bool delayedSleepIsSet(); //returns true, if a timer has already been set
//void append_intermediate_task(uint8_t intermediate_task);
#endif
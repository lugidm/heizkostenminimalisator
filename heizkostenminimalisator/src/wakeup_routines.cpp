// Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/deep-sleep-stub.html


#include <esp_sleep.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"


RTC_DATA_ATTR struct StateVariables* state_variables;

//this function defines the wakeups but also the interrupts (every )
void setupWakeUpRoutines(StateVariables* state_vars){
    state_variables = state_vars;
    esp_sleep_enable_timer_wakeup(TEMP_CHECK_PERIOD*1000000);
    esp_sleep_enable_ext0_wakeup(PIN_OVEN_DOOR, 1);
    state_variables->thermo_timer = timerBegin(0, 80, true);  // second argument is the dividor. The timer interrupts with a frequency of 80/dividor MHz

    timerAttachInterrupt(state_variables->thermo_timer, &onTimer, true);
    timerAlarmWrite(state_variables->thermo_timer, 1000000*TEMP_CHECK_PERIOD, true);
    timerAlarmEnable(state_variables->thermo_timer);
}

// this function is weak linked -> it gets overwritten easily. esp_default_wake... has to be called immidiately in the beginnning
void RTC_IRAM_ATTR esp_wake_deep_sleep(void){  
    esp_default_wake_deep_sleep();
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : state_variables->state = INTR_OVEN_DOOR; break;
    case ESP_SLEEP_WAKEUP_TIMER : state_variables->state = INTR_READ_T; &onTimer; break; // only when the timer wakes the ESP, we go into a ISR, otherwise the normal loop is started
    case ESP_SLEEP_WAKEUP_UNDEFINED : state_variables->state = INTR_UNDEFINED; break;
    default : state_variables->state = INTR_UNDEFINED; break;
    }
}

void sleepysloopy(){
    delay(500);
    esp_deep_sleep_start();
}

void delayedSleepEnable(int32_t delay_sec){ //int32 because otherwise it is possible to get an overflow (*10000000 when appliying seconds)
    state_variables->sleep_timer = timerBegin(1, 80, true);
    timerAttachInterrupt(state_variables->sleep_timer, &sleepTimerISR, true);
    timerAlarmWrite(state_variables->sleep_timer, delay_sec*1000000, false); //not a recurring interrupt!
    timerAlarmEnable(state_variables->sleep_timer);
}

void delayedSleepDisable(){
    timerAlarmWrite(state_variables->sleep_timer, -1, false);
    timerAlarmDisable(state_variables->sleep_timer);
}

void IRAM_ATTR onTimer(){ // This is the function that gets called every TEMP_CHECK_PERIOD seconds it is not nice to put everything in the ISR but since we interrupt every 5 Minutes, this should do:)
    Thermocouple thmc = Thermocouple();
    double current_temp = thmc.read_temperature(1000); // this also stores errors!! This has to be checked later in the procedure
    for(uint8_t i = NUM_TEMP_MEASUREMENTS; i>0; i--){
        state_variables->temperature_measurements[i] = state_variables->temperature_measurements[i-1]; // shift everything to the right
    }
    state_variables->temperature_measurements[0] = current_temp;
}

void IRAM_ATTR sleepTimerISR(){  // This wrapper function gets called by the interrupt. Its only purpose is to let put the ESP32 into sleep mode add here any necessary pre-sleep function-calls
    esp_deep_sleep_start();
}
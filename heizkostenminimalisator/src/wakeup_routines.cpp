// Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/deep-sleep-stub.html
#include "wakeup_routines.h"

RTC_DATA_ATTR StateVariables state_variables;

//this function defines the wakeups but also the interrupts (every )
void setupWakeUpRoutines(){
    state_variables.state = STATE_NORMAL_BOOT;
    state_variables.task_measure_temperature = true;
    for (uint8_t i = 0; i<NUM_TEMP_MEASUREMENTS; i++){
        state_variables.temperature_measurements[i] = 0;
    }
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    esp_sleep_enable_timer_wakeup(TEMP_CHECK_PERIOD*1000000);
    esp_sleep_enable_ext0_wakeup(PIN_OVEN_DOOR, 1);
    rtc_gpio_pullup_en(PIN_OVEN_DOOR);
    state_variables.thermo_timer = timerBegin(0, 80, true);  // second argument is the dividor. The timer interrupts with a frequency of 80/dividor MHz

    timerAttachInterrupt(state_variables.thermo_timer, &onTimer, true);
    timerAlarmWrite(state_variables.thermo_timer, 1000000*TEMP_CHECK_PERIOD, true);
    timerAlarmEnable(state_variables.thermo_timer);
}

StateVariables* get_state_vars(){
    return &state_variables;
}
//this function is weak linked -> it gets overwritten easily. esp_default_wake... has to be called immidiately in the beginnning
void RTC_IRAM_ATTR esp_wake_deep_sleep(void){  
    esp_default_wake_deep_sleep();
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        state_variables.state = STATE_DOOR_OPENED;
        state_variables.highest_temperature = -1;
        state_variables.temperature_closure_slope = 0;
        state_variables.temperature_closure_offset = 0;
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        if (state_variables.state == STATE_NORMAL_BOOT || state_variables.state == STATE_COOL_DOWN)  // when the esp wakes up in STATE_COOL_DOWN, the procedure should be finished. therefore, a "reset" is appropriate
        {
            state_variables.state = STATE_READ_T;
        }
        onTimer();
        break;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
        state_variables.highest_temperature = -1;
        state_variables.temperature_closure_slope = 0;
        state_variables.temperature_closure_offset = 0;
        state_variables.state = STATE_READ_T;  //NORMAL_BOOT
        break;
    default:
        break;
    }
}


void delayedSleepEnable(int32_t delay_sec){ //int32 because otherwise it is possible to get an overflow (*10000000 when appliying seconds)
    state_variables.sleep_timer = timerBegin(1, 80, true);
    timerAttachInterrupt(state_variables.sleep_timer, &sleepTimerISR, true);
    timerAlarmWrite(state_variables.sleep_timer, delay_sec*1000000, false); //not a recurring interrupt!
    timerAlarmEnable(state_variables.sleep_timer);
}

void delayedSleepDisable(){
    timerAlarmWrite(state_variables.sleep_timer, -1, false);
    timerAlarmDisable(state_variables.sleep_timer);
}

bool delayedSleepIsSet(){
    return state_variables.sleep_timer != NULL && timerAlarmEnabled(state_variables.sleep_timer);
}

void IRAM_ATTR onTimer(){ // This is the function that gets called every TEMP_CHECK_PERIOD seconds it is not nice to put everything in the ISR but since we interrupt every 5 Minutes, this should do:)
    state_variables.task_measure_temperature = true;
    //append_intermediate_task(TASKS_READ_T);
}

void IRAM_ATTR sleepTimerISR(){  // This wrapper function gets called by the interrupt. Its only purpose is to let put the ESP32 into sleep mode add here any necessary pre-sleep function-calls
    esp_deep_sleep_start(); /////////////////TODO: mitigate this to a normal loop function call!!!!!
}

/*
void append_intermediate_task(uint8_t intermediate_task)
{
    for (uint8_t i = 0; i < sizeof(state_variables.intermediate_tasks); i++)
    {
        if (state_variables.intermediate_tasks[i] == -1)
        {
            state_variables.intermediate_tasks[i] == intermediate_task;
        }
    }
}

void delete_intermediate_task(uint8_t id)
{
    state_variables.intermediate_tasks[id] = -1;
    for (uint8_t i = id; i < sizeof(state_variables.intermediate_tasks) - 1; i++)
    {
        state_variables.intermediate_tasks[i] = state_variables.intermediate_tasks[i + 1];
    }
}

bool intermediate_task_available(uint8_t intermediate_task)
{
    for (uint8_t i = 0; i < sizeof(state_variables.intermediate_tasks); i++)
    {
        if (state_variables.intermediate_tasks[i] != -1)
        {
            return
        }
        state_variables[i] = state_variables[i + 1];
    }
}*/
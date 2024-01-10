// Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/deep-sleep-stub.html


#include <esp_sleep.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"


RTC_DATA_ATTR struct WakeUpSettings* settings;
RTC_DATA_ATTR Thermocouple temp_measurement_dev;
hw_timer_s* timer;

unsigned char handleWakeupRoutines(){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    Serial.println("");
    Serial.println(wakeup_reason);
    char state = 0;
    switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : state = INTR_OVEN_DOOR; break;
    case ESP_SLEEP_WAKEUP_TIMER : state = INTR_READ_T; break;
    case ESP_SLEEP_WAKEUP_UNDEFINED : state = INTR_UNDEFINED; break;
    default : state = INTR_UNDEFINED; break;
    }
    return state;
}
//this function defines the wakeups but also the interrupts (every )
void setupWakeUpRoutines(WakeUpSettings* w_u_settings, Thermocouple* t_m_d){
    temp_measurement_dev = *t_m_d;
    esp_sleep_enable_timer_wakeup(TEMP_CHECK_PERIOD*1000000);
    esp_sleep_enable_ext0_wakeup(PIN_OVEN_DOOR, 1);
    timer = timerBegin(0, 80, true);  // second argument is the dividor. The timer interrupts with a frequency of 80/dividor MHz
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000*TEMP_CHECK_PERIOD, true);
    timerAlarmEnable(timer);
}

// this function is weak linked -> it gets overwritten easily like this: esp_default_wake... has to be called immidiately in the beginnning
void RTC_IRAM_ATTR esp_wake_deep_sleep(void){  
    esp_default_wake_deep_sleep();


}

void sleepysloopy(){
    delay(500);
    esp_deep_sleep_start();
}

void IRAM_ATTR onTimer(){ // This is the function that gets called every TEMP_CHECK_PERIOD seconds it is not nice to put everything in the ISR but since we interrupt every 5 Minutes, this should do:)
    temp_measurement_dev.read_temperature()
}
// Look at: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/deep-sleep-stub.html


#include <esp_sleep.h>
#include "wakeup_routines.h"
#include "pin_setup.h"

RTC_DATA_ATTR struct WakeUpSettings* settings;

unsigned char handleWakeupRoutines(){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    Serial.println("");
    Serial.println(wakeup_reason);
    char state = 0;
    switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : state = STATE_OVEN_DOOR; break;
    case ESP_SLEEP_WAKEUP_TIMER : state = STATE_TEMP_CHECK; break;
    case ESP_SLEEP_WAKEUP_UNDEFINED : state = STATE_UNDEFINED; break;
    default : state = STATE_UNDEFINED; break;
    }
    return state;
}

void setupWakeUpRoutines(WakeUpSettings* w_u_settings){
    esp_sleep_enable_timer_wakeup(SELF_CHECK_RATE*1000000);
    esp_sleep_enable_ext0_wakeup(PIN_STOVE_IN, 1);
}

// this function is weak linked -> it gets overwritten easily like this: esp_default_wake... has to be called immidiately in the beginnning
void RTC_IRAM_ATTR esp_wake_deep_sleep(void){  
    esp_default_wake_deep_sleep();


}

void sleepysloopy(){
    delay(500);
    esp_deep_sleep_start();
}
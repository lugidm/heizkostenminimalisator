#include <esp_sleep.h>
#include "wakeup_routines.h"
#include "thermocouple.h"
#include "pin_setup.h"

RTC_DATA_ATTR struct WakeUpSettings* settings;


void ARDUINO_ISR_ATTR onTimer(){
    Serial.println("onTimerWasCalled");
    Serial.flush();
    //portENTER_CRITICAL_ISR(&settings->state_mux);
    settings->state = STATE_TEMP_CHECK;
    //portEXIT_CRITICAL_ISR(&settings->state_mux);
    Serial.println("onTimerWasLeaving");
}


unsigned char handleWakeupRoutines(){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    Serial.println("");
    Serial.println(wakeup_reason);
    char state = 0;
    switch(wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0 : state = STATE_OVEN_DOOR; break;
    case ESP_SLEEP_WAKEUP_TIMER : state = STATE_TEMP_CHECK; break;
    default : state = STATE_NORMAL_BOOT; break;
    }
    return state;
}

void setupWakeUpRoutines(WakeUpSettings* w_u_settings){
    esp_sleep_enable_timer_wakeup(SELF_CHECK_RATE*1000000);
    esp_sleep_enable_ext0_wakeup(PIN_STOVE_IN, 1);
}
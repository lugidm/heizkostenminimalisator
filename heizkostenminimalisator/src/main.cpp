#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"

RTC_DATA_ATTR struct WakeUpSettings wake_up_settings;
//RTC_DATA_ATTR portMUX_TYPE settings_spinlock portMUX_INITIALIZER_UNLOCKED;

//RTC_DATA_ATTR uint8_t state = STATE_NORMAL_BOOT; //this is the only thing that is stored during deepsleep!
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  setup_pins();

  setupWakeUpRoutines(&wake_up_settings);
  unsigned char current_state = handleWakeupRoutines();
  if(current_state == STATE_NORMAL_BOOT){
    Serial.println("NORMAL BOOT");
    esp_deep_sleep_start();
  }
  else if(current_state == STATE_TEMP_CHECK){
    Serial.println("STATE TEMP");
    esp_deep_sleep_start();
  }
  else {
    Serial.println("STATE_GPIO");
    esp_deep_sleep_start();
  }
  //esp_sleep_enable_timer_wakeup(100000);
  // = handleWakeupRoutines(); //checks if normal boot or not-normal boot

  Serial.println("Entering DeepSleep in 1.2sec!");
  delay(1200);
  
}

void loop() {
  Serial.println("now in loop");
  delay(10000);
  // put your main code here, to run repeatedly:
}
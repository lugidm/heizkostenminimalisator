#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"
#include "motor_control.h"

RTC_DATA_ATTR struct WakeUpSettings wake_up_settings;
//RTC_DATA_ATTR portMUX_TYPE settings_spinlock portMUX_INITIALIZER_UNLOCKED;


//RTC_DATA_ATTR uint8_t state = STATE_NORMAL_BOOT; //this is the only thing that is stored during deepsleep!
void setup() {
    Serial.println("now in setup");
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  setup_pins();
  Thermocouple temperature_measurement_device = Thermocouple();
  Motor motor = Motor();
  setupWakeUpRoutines(&wake_up_settings);
  unsigned char current_state = handleWakeupRoutines();
  if(current_state == STATE_TEMP_CHECK){
    Serial.println("STATE TEMP CHECK");
    if (!temperature_measurement_device.burning()){
      Serial.println("temperature sensor tells us that there is no fire");
      sleepysloopy();
    }
    
  }
  else if(current_state == STATE_OVEN_DOOR){
    Serial.println("OVEN DOOR OPENED -> longer check");
  }  
  else if(current_state == STATE_UNDEFINED){ // that is normal boot
    Serial.println("UNDEFINED WAKE UP / NORMAL BOOT");
    motor.open_completely();
    
  }
}

void loop() {
    Serial.println("now in loop");
  Serial.println("Entering DeepSleep in 1.2sec!");
  delay(1200);
  sleepysloopy();

  // put your main code here, to run repeatedly:
}
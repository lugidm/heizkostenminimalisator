#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"
#include "motor_control.h"


RTC_DATA_ATTR StateVariables state_variables;
RTC_DATA_ATTR double measured_temperature[NUM_TEMP_MEASUREMENTS] = {0,0,0,0,0}; // In this array the last 5 measured temperatures are stored!. [0] is the latest, [5] is the oldest
Motor motor;

void setup() {
  if(state_variables.state = STATE_NORMAL_BOOT){
    //this is the first bootup -> nothing is loaded yet 
  }
  else{
    setupWakeUpRoutines(&state_variables);
  }
  Serial.begin(9600);
  delay(1000);
  setup_pins();
  motor = Motor();
  if(state_variables.state == STATE_NORMAL_BOOT){ //this could also be a power-outage!
    Serial.println("UNDEFINED WAKE UP / NORMAL BOOT -> opening air inlet");
    motor.open_completely();
    //after that we will stay awake for at least 30 Minutes
  }
}

void loop() {
  int on_counter = 0;
  switch (state_variables.state)
  {
  case STATE_NORMAL_BOOT:
    delayedSleepEnable(30*60); // after 30 minutes, if nothing happens, we can go to sleep again:)
    break;
  
  default:
    break;
  }
  Serial.println("now in loop");
  Serial.println("Entering DeepSleep in 1.2sec!");
  delay(1200);
  sleepysloopy();

  // put your main code here, to run repeatedly:
}
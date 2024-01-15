#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"
#include "motor_control.h"
//#define DEBUG  //uncomment this line when you want serial output!

RTC_DATA_ATTR StateVariables state_variables;
RTC_DATA_ATTR double measured_temperature[NUM_TEMP_MEASUREMENTS] = {0,0,0,0,0}; // In this array the last 5 measured temperatures are stored!. [0] is the latest, [5] is the oldest
Thermocouple thermocouple;
Motor motor;
unsigned int door_opened_time;


void setup() {
  if(state_variables.state = STATE_NORMAL_BOOT){
    //this is the first bootup -> nothing is loaded yet 
    setupWakeUpRoutines(&state_variables);
  }
  #ifdef DEBUG
  Serial.begin(9600);
  delay(1000);
  #endif
  setup_pins();
  motor = Motor();
  thermocouple = Thermocouple()
}

void loop() {
  
  int on_counter = 0;
  switch (state_variables.state)
  {
  case STATE_DOOR_OPENED:
    motor.open_completely();
    delayedSleepEnable(30*60); // after 30 minutes, if nothing happens, we can go to sleep again:)
    break;
  case STATE_NORMAL_BOOT:
    state_variables.state = STATE_DOOR_OPENED;
    break;
  case STATE_IDLE:
    break; // do nothing but to check constantly
  case STATE_READ_T:
    if(state_variables.state == STATE_READ_T){
      esp_deep_sleep_start();

    }
    break;
  default:
    break;
  }

  if(state_variables.measure_temperature){
    state_variables.measure_temperature = false;
    thermocouple.add_temperature_measurement(&state_variables);
    
  }
  #ifdef DEBUG
  Serial.println("now in loop");
  Serial.println("Entering DeepSleep in 1.2sec!");
  #endif
  delay(1000);
  sleepysloopy();

  // put your main code here, to run repeatedly:
}
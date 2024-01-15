#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"
#include "motor_control.h"
//#define DEBUG  //uncomment this line when you want serial output!

RTC_DATA_ATTR StateVariables state_variables;
Thermocouple thermocouple;
Motor motor;
unsigned int door_opened_time;
void setup() {
  if(state_variables.state = STATE_NORMAL_BOOT){
    //this is the first bootup -> nothing is loaded yet 
    setupWakeUpRoutines();
  }
  #ifdef DEBUG
  Serial.begin(9600);
  delay(1000);
  #endif
  setup_pins();
  motor = Motor();
  thermocouple = Thermocouple();
}

void loop() {
  if (state_variables.task_measure_temperature)
    {
      state_variables.task_measure_temperature = false;
      thermocouple.add_temperature_measurement(&state_variables);
    }
  switch (state_variables.state)
  {
  case STATE_DOOR_OPENED:
    motor.open_completely();

    //delayedSleepEnable(30 * 60); // after 30 minutes, if nothing happens, we can go to sleep again:)
    if (thermocouple.temperature_rising_significantly(state_variables.temperature_measurements))
    {
      state_variables.state = STATE_BURNING;
      delayedSleepDisable();
    }
    break;
  case STATE_NORMAL_BOOT:
    state_variables.state = STATE_DOOR_OPENED;
    break;
  case STATE_IDLE:
    //delayedSleepEnable(30 * 60);
    break; // do nothing but to check constantly
  case STATE_READ_T:
    if (state_variables.state == STATE_READ_T && !state_variables.task_measure_temperature)
    {
      if (thermocouple.temperature_rising_significantly(state_variables.temperature_measurements))
      {
        state_variables.state = STATE_BURNING;
      }
      else
      {
        esp_deep_sleep_start();
      }
    }
  case STATE_BURNING:
    motor.adjust(MOTOR_OPENING_BURNING);
    if(thermocouple.cur_highest_temperature(state_variables.temperature_measurements) > state_variables.highest_temperature){
      state_variables.highest_temperature = thermocouple.cur_highest_temperature(state_variables.temperature_measurements);
    }
    if(thermocouple.temperature_sinking_significantly(state_variables.temperature_measurements)){
      state_variables.state = STATE_BURN_OFF;
    }
    else{
      esp_deep_sleep_start();
    }
    break;
  case STATE_BURN_OFF:
    if(state_variables.temperature_closure_slope == 0){ // the calculation of the rate has not been done yet
      state_variables.temperature_closure_slope = (MOTOR_OPENING_COOLDOWN-MOTOR_OPENING_BURNING)/(state_variables.highest_temperature - TEMP_COOL_DOWN);
      state_variables.temperature_closure_offset = MOTOR_OPENING_BURNING - state_variables.temperature_closure_slope * state_variables.highest_temperature;
    }
    if(state_variables.temperature_measurements[0] > 0) // NO ERRORS
    motor.adjust(state_variables.temperature_measurements[0]*state_variables.temperature_closure_slope + state_variables.temperature_closure_offset);
    esp_deep_sleep_start();
  default:
    break;
  }

    
  #ifdef DEBUG
  Serial.println("now in loop");
  Serial.println("Entering DeepSleep in 1.2sec!");
  #endif
  delay(1000);
  sleepysloopy();

  // put your main code here, to run repeatedly:
  }
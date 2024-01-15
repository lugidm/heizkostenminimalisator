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
double highest_temperature = -1;

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
    delayedSleepEnable(30 * 60); // after 30 minutes, if nothing happens, we can go to sleep again:)
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
    delayedSleepEnable(30 * 60);
    break; // do nothing but to check constantly
  case STATE_READ_T:
    if (state_variables.state == STATE_READ_T && !state_variables.task_measure_temperature)
    { // the second time we pass through the loop we check for a temperature-rise
      if (thermocouple.temperature_rising_significantly(state_variables.temperature_measurements))
      {
        motor.open_completely();
        state_variables.state = STATE_BURNING;
      }
      else
      {
        esp_deep_sleep_start();
      }
    }
  case STATE_BURNING:
    if(thermocouple.cur_highest_temperature(state_variables.temperature_measurements) > highest_temperature){
      highest_temperature = thermocouple.cur_highest_temperature(state_variables.temperature_measurements);
    }
    esp_deep_sleep_start(); // we have to wake up, the next time, the temperature gets measured!
    break;
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
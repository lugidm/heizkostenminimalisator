#define UNIT_TESTS //uncomment this line for normal behaviour! this simplifies some functions for testing
#define DEBUG  //uncomment this line when you want serial output!

#include <Arduino.h>
#include "wakeup_routines.h"
#include "pin_setup.h"
#include "thermocouple.h"
#include "motor_control.h"
#include "display.h"
////// TODO: ADAPT AS WISHED ///////
#define ATTENTION_PERIOD 0.1 // this is a period given in minutes after which the Microcontroller goes to sleep if e.g. the oven door was only opened accidentaly 
#define COOL_DOWN_PERIOD 30 // After this period, given in minutes, the esp will go to sleep and therefore, the motor will be set to closed completely
//////
StateVariables* state_var_ptr;
Thermocouple thermocouple;
Motor motor;
//OutputDisplay output_display;
uint32_t state_time_counter;

unsigned int door_opened_time;

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello World");
  #endif
  esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("EXT0");
    break;
    case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("TIMER");
    break;
    case ESP_SLEEP_WAKEUP_UNDEFINED:
    Serial.println("UNDEF");
    break;
    default:
    Serial.println("ehh bo");
    break;
    }

  state_var_ptr = get_state_vars();
  #ifdef DEBUG
  print_state(state_var_ptr->state);
  #endif
  if(state_var_ptr->state == STATE_NORMAL_BOOT){
    //this is the first bootup -> nothing is loaded yet 
    setupWakeUpRoutines();
  }
  setup_pins();
  motor = Motor();
  thermocouple = Thermocouple();
  //output_display = OutputDisplay();
  state_time_counter = 0;
}


void loop() {
  if (state_var_ptr->task_measure_temperature)
    {
      state_var_ptr->task_measure_temperature = false;
      thermocouple.add_temperature_measurement(state_var_ptr);
    }
  #ifdef DEBUG
  Serial.println("In Loop");
  delay(1000);
  #endif
  /*output_display.writeState(state_var_ptr->state, state_time_counter);
  output_display.writeMotor(motor.get_cur_position().cur_closure);
  output_display.writeTemp(state_var_ptr->temperature_measurements);*/
  #ifdef DEBUG
  print_state(state_var_ptr->state);
  #endif
  switch (state_var_ptr->state)
  {
  case STATE_DOOR_OPENED:

    motor.open_completely();
    if (!delayedSleepIsSet()){
      delayedSleepEnable(ATTENTION_PERIOD * 60); // after XX minutes, we can go to sleep if nothing happened. After that, the voltage of the motor should drop to zero -> closed completely!!!
      esp_deep_sleep_start();
    }
    if (thermocouple.temperature_rising_significantly(state_var_ptr->temperature_measurements))
    {
      state_time_counter = 0;
      state_var_ptr->state = STATE_BURNING;
      delayedSleepDisable();
    }
    break;

  case STATE_NORMAL_BOOT:
    state_time_counter = 0;
    state_var_ptr->state = STATE_DOOR_OPENED;
    break;

  case STATE_READ_T:  // state where ESP woke up to read T
    if (thermocouple.temperature_rising_significantly(state_var_ptr->temperature_measurements))
    {
      state_time_counter = 0;
      state_var_ptr->state = STATE_BURNING;
    }
    else
    {
      esp_deep_sleep_start();
    }
    break;

  case STATE_BURNING:
    motor.adjust(MOTOR_OPENING_BURNING);
    if(thermocouple.cur_highest_temperature(state_var_ptr->temperature_measurements) > state_var_ptr->highest_temperature){
      state_var_ptr->highest_temperature = thermocouple.cur_highest_temperature(state_var_ptr->temperature_measurements);
    }
    if(thermocouple.temperature_sinking_significantly(state_var_ptr->temperature_measurements)){
      state_time_counter = 0;
      state_var_ptr->state = STATE_BURN_OFF;
    }
    break;

  case STATE_BURN_OFF:
    if(thermocouple.temperature_rising_significantly(state_var_ptr->temperature_measurements)){
      state_time_counter = 0;
      state_var_ptr->highest_temperature = 0;
      state_var_ptr->temperature_closure_slope = 0;
      state_var_ptr->temperature_closure_offset = 0;
      state_var_ptr->state = STATE_BURNING;
      break;
    }
    if(state_var_ptr->temperature_measurements[0]>0 && state_var_ptr->temperature_measurements[0]<TEMP_COOL_DOWN){
      state_time_counter = 0;
      state_var_ptr->state = STATE_COOL_DOWN;
    }
    else if(state_var_ptr->temperature_measurements[0] > 0){ // NO ERRORS
      motor.adjust(state_var_ptr->temperature_measurements[0]*state_var_ptr->temperature_closure_slope + state_var_ptr->temperature_closure_offset);
    }
    if(state_var_ptr->temperature_closure_slope == 0){ // the calculation of the rate has not been done yet
      state_var_ptr->temperature_closure_slope = (MOTOR_OPENING_COOLDOWN-MOTOR_OPENING_BURNING)/(state_var_ptr->highest_temperature - TEMP_COOL_DOWN);
      state_var_ptr->temperature_closure_offset = MOTOR_OPENING_BURNING - state_var_ptr->temperature_closure_slope * state_var_ptr->highest_temperature;
    }
    break;

  case STATE_COOL_DOWN:
    if(thermocouple.temperature_rising_significantly(state_var_ptr->temperature_measurements)){
      delayedSleepDisable();
      state_time_counter = 0;
      state_var_ptr->highest_temperature = 0;
      state_var_ptr->temperature_closure_slope = 0;
      state_var_ptr->temperature_closure_offset = 0;
      state_var_ptr->state = STATE_BURNING;
      break;
    }
    else if(!delayedSleepIsSet()){
      delayedSleepEnable(COOL_DOWN_PERIOD*60);
    }
    if (motor.get_cur_position().cur_closure != MOTOR_OPENING_COOLDOWN){ // this is true for the first time in COOL_DOWN
      state_var_ptr->highest_temperature = 0;
      state_var_ptr->temperature_closure_slope = 0;
      state_var_ptr->temperature_closure_offset = 0;
      motor.adjust(MOTOR_OPENING_COOLDOWN);
    }
    break;

  default:
    break;
  }

    
  #ifdef DEBUG
  Serial.println("end of loop");
  #endif
  state_time_counter++;
  delay(1000);
  }

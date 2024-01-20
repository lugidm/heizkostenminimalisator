#include "thermocouple.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MAX31855.h>

Thermocouple::Thermocouple() : thermocouple(MAXCLK, MAXCS, MAXDO) {
  //this->thermocouple((int8_t)MAXCLK, (int8_t)MAXCS, (int8_t)MAXDO);
  //Adafruit_MAX31855 try_pout = Adafruit_MAX31855();
  //try_pout = Adafruit_MAX31855(;
  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor..");
  if (!this->thermocouple.begin()) {
      Serial.println("ERROR.");
  while (1) delay(10);
  }
  // OPTIONAL: Can configure fault checks as desired (default is ALL)
  // Multiple checks can be logically OR'd together.
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);  // short to GND fault is ignored    return true;
}


//this function reads the temperature (multiple times and averages the values over the averaging time)
double Thermocouple::read_temperature(uint16_t averaging_cycles){//no sleep in this function because it gets called in a ISR!!!
  uint16_t avg_counter = 0;
  uint8_t retry_counter = 0;
  double averaged_val = AVG_BEGIN_VAL;
  while(avg_counter < averaging_cycles){
    while (retry_counter < READ_RETRIES){
      double c = thermocouple.readCelsius();
      if (isnan(c)) { // This is only when readCelsius did not work
        uint8_t e = thermocouple.readError();
        if (e & MAX31855_FAULT_OPEN) c=FAULT_OPEN;
        if (e & MAX31855_FAULT_SHORT_GND) c=FAULT_SHORT_GND;
        if (e & MAX31855_FAULT_SHORT_VCC) c=FAULT_SHORT_VCC;
        retry_counter++;
        averaged_val = c;
      } else {
        retry_counter = 0;
        if(averaged_val == AVG_BEGIN_VAL){
          averaged_val = c;
        }
        else{
          averaged_val = (averaged_val+c)/2;
        }
      }
    }

  avg_counter++;
  }
  return averaged_val;
}

bool Thermocouple::burning(double* temperature_measurements){
  return temperature_measurements[0]>40;
}

bool Thermocouple::temperature_rising_significantly(double* temperature_measurements){
  bool rising = true;
  for (uint8_t i = 0; i < int((15*60)/TEMP_CHECK_PERIOD); i++){// Check over the last 15 minutes!!!
    rising = rising && (temperature_measurements[i] > temperature_measurements[i+1] &&  //check if rising in the last 15 minutes
                        temperature_measurements[i] > 0 && temperature_measurements[i+1] > 0 &&
                        temperature_measurements[i] != FAULT_OPEN && temperature_measurements[i+1] != FAULT_OPEN &&
                        temperature_measurements[i] != FAULT_SHORT_GND && temperature_measurements[i+1] != FAULT_SHORT_GND &&
                        temperature_measurements[i] != FAULT_SHORT_VCC && temperature_measurements[i+1] != FAULT_SHORT_VCC);
  }
  rising = rising && (temperature_measurements[0] > temperature_measurements[int((15*60)/TEMP_CHECK_PERIOD + 0.5)] + DTEMP_SIGNIFICANT_RISING); // the last check is for significancy
  return rising;
}

bool Thermocouple::temperature_sinking_significantly(double *temperature_measurements){
  bool sinking = true;
  sinking = (temperature_measurements[0] != FAULT_OPEN && temperature_measurements[1] != FAULT_OPEN &&
          temperature_measurements[0] != FAULT_SHORT_GND && temperature_measurements[1] != FAULT_SHORT_GND &&
          temperature_measurements[0] != FAULT_SHORT_VCC && temperature_measurements[1] != FAULT_SHORT_VCC);
  for (uint8_t i = 0; i < int((15*60)/TEMP_CHECK_PERIOD + 0.5); i++){// Check over the last 15 minutes!!!
    sinking = sinking && (temperature_measurements[i] < temperature_measurements[i+1] &&  //check if sinking in the last 15 minutes
                        temperature_measurements[i] > 0 && temperature_measurements[i+1] > 0 &&
                        temperature_measurements[i] != FAULT_OPEN && temperature_measurements[i+1] != FAULT_OPEN &&
                        temperature_measurements[i] != FAULT_SHORT_GND && temperature_measurements[i+1] != FAULT_SHORT_GND &&
                        temperature_measurements[i] != FAULT_SHORT_VCC && temperature_measurements[i+1] != FAULT_SHORT_VCC);
  }
  sinking = sinking && (temperature_measurements[0]+ DTEMP_SIGNIFICANT_FALLING < temperature_measurements[int((15*60)/TEMP_CHECK_PERIOD + 0.5)]); // the last check is for significancy
  return sinking;
}


void Thermocouple::add_temperature_measurement(StateVariables* state_variables){
    double current_temp = this->read_temperature(1000); // this also stores errors! This has to be checked later in the procedure
    for(uint8_t i = NUM_TEMP_MEASUREMENTS; i>0; i--){
        state_variables->temperature_measurements[i] = state_variables->temperature_measurements[i-1]; // shift everything to the right
    }
    state_variables->temperature_measurements[0] = current_temp;
}

double Thermocouple::cur_highest_temperature(double* temperature_measurements){
  double return_val = FAULT_OPEN;
  for(int i = 0; i < NUM_TEMP_MEASUREMENTS; i++){
    if(return_val< temperature_measurements[i]){
      return_val = temperature_measurements[i];
    }
  }
  return return_val;
}
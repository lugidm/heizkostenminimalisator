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
}

bool Thermocouple::burning(double* temperature_measurements){
  return temperature_measurements[0]>40;
}

boolean Thermocouple::temperature_rising_significantly(double* temperature_measurements){
  return (temperature_measurements[0] != FAULT_OPEN && temperature_measurements[1] != FAULT_OPEN &&
          temperature_measurements[0] != FAULT_SHORT_GND && temperature_measurements[1] != FAULT_SHORT_GND &&
          temperature_measurements[0] != FAULT_SHORT_VCC && temperature_measurements[1] != FAULT_SHORT_VCC &&
          temperature_measurements[0] > temperature_measurements[1] + SIGNIFICANT_TEMPERATURE_RISE && 
          temperature_measurements[1] > 0);
}

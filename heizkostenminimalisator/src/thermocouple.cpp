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


char Thermocouple::read_temperature(){
    Serial.print("Internal Temp = ");
   Serial.println(this->thermocouple.readInternal());


   double c = thermocouple.readCelsius();

   if (isnan(c)) {

     Serial.println("Thermocouple fault(s) detected!");

     uint8_t e = thermocouple.readError();

     if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");

     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");

     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");

   } else {

     Serial.print("C = ");

     Serial.println(c);

   }

   //Serial.print("F = ");

   //Serial.println(thermocouple.readFahrenheit());

 

   delay(1000);
   return c;
}

bool Thermocouple::burning(){
  return true;
}

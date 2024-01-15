#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H
#include <Adafruit_MAX31855.h>
#include "wakeup_routines.h"
#define MAXDO   19
#define MAXCS   23
#define MAXCLK  5
#define READ_RETRIES 5  // when a read failed, the read-function will retry x times to read
#define FAULT_SHORT_GND -55.44
#define FAULT_SHORT_VCC -55.55
#define FAULT_OPEN -55.66
#define AVG_BEGIN_VAL -66.6
#define SIGNIFICANT_TEMPERATURE_RISE 10 //degrees celsius or Kelvin

class Thermocouple{
    private:
        Adafruit_MAX31855 thermocouple;
    public: 
        Thermocouple();
        double read_temperature(uint16_t averaging_time); // averaging time in milliseconds
        bool burning(double *temperature_measurements);
        bool temperature_rising_significantly(double *temperature_measurements);
        void add_temperature_measurement(StateVariables* state_variables);
        double cur_highest_temperature(double *temperature_measurements);
    };



#endif
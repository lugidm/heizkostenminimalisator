#include <Adafruit_MAX31855.h>
#define MAXDO   19
#define MAXCS   23
#define MAXCLK  5

class Thermocouple{
    private:
        Adafruit_MAX31855 thermocouple;
    public: 
        bool init_thermocouple(){}
        char read_temperature(){}
    };

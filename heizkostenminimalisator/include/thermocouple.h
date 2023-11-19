#include <Adafruit_MAX31855.h>
#define MAXDO   19
#define MAXCS   23
#define MAXCLK  5

class Thermocouple{
    private:
        Adafruit_MAX31855 thermocouple;
    public: 
        Thermocouple();
        char read_temperature();
        bool burning();
    };

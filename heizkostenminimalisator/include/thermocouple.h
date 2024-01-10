#include <Adafruit_MAX31855.h>
#define MAXDO   19
#define MAXCS   23
#define MAXCLK  5
#define READ_RETRIES 5  // when a read failed, the read-function will retry x times to read
#define FAULT_SHORT_GND -55.44
#define FAULT_SHORT_VCC -55.55
#define FAULT_OPEN -55.66
#define AVG_BEGIN_VAL -66.6
class Thermocouple{
    private:
        Adafruit_MAX31855 thermocouple;
    public: 
        Thermocouple();
        double read_temperature(uint16_t averaging_time); // averaging time in milliseconds
        bool burning();
    };

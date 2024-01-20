# heizkostenminimalisator
Software to control the inlet air of a stove with temperature sensor and (in a later version communication over MQTT)

Programmed in VSCode with PlatformIO.


## Basic Procedure
* The temperature is measured every TEMP_CHECK_PERIOD, triggered by an interrupt (and also wake-up routine)

* The oven door-switch has to be connected to an RTC GPIO-PIN https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html (defined in pin_setup.h)

  * When the door is **open, the pin measures inf. resistance => pin is internally pulled up. Therefore, the interrupt is triggered when input-pin is 1 -> check this in `ẁakeup_routines.cpp` line 17**

* As for now, the state, temperature etc. will be displayed on a external display https://eckstein-shop.de/WaveShare-096inch-OLED-Display-Module-White-on-Black-128x64-SPI-I2C.

* The setup of the display is defined in `display.h` (see `display_setup.dxf`)

* ![display_setup](./documentation/display_setup.png)

* **Possible extension: Check for 5 Minutes on a HTML-Server to check whether someone wants to change some parameters**

* **Possible extension: Communicate over MQTT if activated**

  

## STATE CHART
![plot](./documentation/State_chart.png?raw=true "State Chart")

## CONSTANTS
- defined in various files (`pin_setup.h,` `main.cpp`, `motor_control.h`, `wakeup_routines.h`, `thermocouple.h`)
- **The ones marked with /// TODO: .... have to be changed by the user**

## COOL_DOWN

The motor opening gets set according to slope between the highest measured temperature and the cool down temperature and the two motor-openings:

![plot](./documentation/slope.png?raw=true "State Chart")

Motor_opening = current temperature
$$
M_{current} = T_{current} * slope + offset
$$



## Project Components
### [Thermocouple Digital Converter](https://cdn-shop.adafruit.com/datasheets/MAX31855.pdf)
SPI-output as 14 bit signed (integer)
### [Selfmade Voltage Stepper](https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWEBmAHAJmgdgGzoRmACzICcpkORIC6NNkNApgLRhgBQA5iKnCOlSpe-ZMhxQoHAMYgcQ8DkZ9GYJZJjxIydOT36DE9tGRYEWUlVJYwqBGDqbInAE4jV6lQIWqtHIpCkNKR0aozywmHgUNASbGDQpAhEWJCoYKR8GYLIAsjQEJDccgHgISWM6OjUjEUAShXggo3sjpKlDhoxCMURKHRmdDoaHADuwXSWE+BENWPTU31TRW5LEn3DjAhKK3IKUWuSVfAcDYMgU17L7VtdMD08fYLCG+IjqwpT59dgfh+R6iePnAfgaCHKYFwNE6kIktRApSQ8PuHAA9jMSjdyAJarBnNt8EcMcgOEA)

![plot](./documentation/voltage_stepper.png?raw=true "State Chart")

Is driven by PIN_MOTOR_OUT (analog) between the voltages pwm_closed to pwm_open and gives following voltage to PWM-duty-cycle ratio (on the x-axis)

![plot](./documentation/voltage_output.png?raw=true "State Chart")


## FLASH image to ESP32:
Ubuntu:
	1. check where the ESP32 is connecting to in /dev/tty**** . Normally (/dev/ttyUSB0)
	2. add your current user to dialout - group (sudo adduser <username> dialout)
	3. change the owner of /dev/tty** to the current user: sudo chown username /dev/ttyUSB0
	Windows:
	1. Check the port in device-manager

Both:
	Check the right port (COM6 or /dev/ttyUSB0) is taken in plaformio.io - file :)

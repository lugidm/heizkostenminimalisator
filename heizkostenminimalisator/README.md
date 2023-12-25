# heizkostenminimalisator
Software to control the inlet air of a stove with temperature sensor and communication over MQTT


## Basic Procedure
1. Startup
	* initialize all variables and create a interrupt for the stove door (first check if it is already open -> 2.)
	* __Possible extension: Check for 5 Minutes on a HTML-Server to check whether someone wants to change some parameters__
	* __Possible extension: Communicate over MQTT if activated__
2. Interrupt happened (stove door opened)
	* Check if we are in fire-state (logging temperature etc.) -> if not continue normally, otherwise, somebody might have re-lit the fire -> ???
	* Open air inlet completely -> 10V on PIN_MOTOR_OUT until reference signal on MOTOR_INPUT_PIN matches READ_AIR_INLET_OPEN_VOLTAGE
	* Start reading from PIN_TEMP_SENSOR if it does not change in 30 minutes close air inlet and go back to deep sleep (wait for interrupt)
	* If temp-signal changes more then **5°** start logging the temperature in a [120x1] int-Array to conduct temperature curve analysis
	* Every 5 minutes, conduct temp-curve analysis, if the curve is truely falling, divide current temperature in 8 closing segments (e.g. 160°-200° = completely open, 120°-160° = 87.7%open etc.)
	* When temperature becomes steady again (air inlet should `hopefully` be at that point around 12% open) close it completely
	* Keep logging and doing curve analysis for another 30 minutes to be sure, the fire is not relit again 

## STATE CHART
![plot](./documentation/State_chart.png?raw=true "State Chart")

## CONSTANTS
- PIN_MOTOR_OUT `integer`
- WRITE_AIR_INLET_CLOSE_VOLTAGE `float`
- WRITE_AIR_INLET_OPEN_VOLTAGE `float`
- READ_AIR_INLET_OPEN_VOLTAGE `float`

## Annahmen (von Lugi)
30minuten zuluft auf sobald ofentür geöffnet wird  
ca. 2h abbranddauer -> int-array größe ~ 120  

motorstellung: pin 25  
Pin 16: Kontakt fürs ofentierl inf. ohm = OFFEN  

Beim Einschalten Ofentür auf!! (wegen Stromausfall)

Bein Interrupt auch

## Project Components
### [Thermocouple Digital Converter](https://cdn-shop.adafruit.com/datasheets/MAX31855.pdf)
SPI-output as 14 bit signed (integer)
### [Selfmade Voltage Stepper](https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWEBmAHAJmgdgGzoRmACzICcpkORIC6NNkNApgLRhgBQA5iKnCOlSpe-ZMhxQoHAMYgcQ8DkZ9GYJZJjxIydOT36DE9tGRYEWUlVJYwqBGDqbInAE4jV6lQIWqtHIpCkNKR0aozywmHgUNASbGDQpAhEWJCoYKR8GYLIAsjQEJDccgHgISWM6OjUjEUAShXggo3sjpKlDhoxCMURKHRmdDoaHADuwXSWE+BENWPTU31TRW5LEn3DjAhKK3IKUWuSVfAcDYMgU17L7VtdMD08fYLCG+IjqwpT59dgfh+R6iePnAfgaCHKYFwNE6kIktRApSQ8PuHAA9jMSjdyAJarBnNt8EcMcgOEA)

Is driven by PIN_MOTOR_OUT (analog) between the voltages WRITE_AIR_INLET_CLOSE_VOLTAGE to WRITE_AIR_INLET_OPEN_VOLTAGE


## FLASH image to ESP32:
Ubuntu:
	1. check where the ESP32 is connecting to in /dev/tty**** . Normally (/dev/ttyUSB0)
	2. add your current user to dialout - group (sudo adduser <username> dialout)
	3. change the owner of /dev/tty** to the current user: sudo chown username /dev/ttyUSB0
Windows:
	1. Check the port in device-manager

Both:
	Check the right port (COM6 or /dev/ttyUSB0) is taken in plaformio.io - file :)

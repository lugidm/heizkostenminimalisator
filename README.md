# heizkostenminimalisator
Software to control the inlet air of a stove with temperature sensor and communication over MQTT

## Basic Procedure
1. Startup
		*initialize all variables and create a interrupt for the stove doo (first check if it is already open -> 2.)
2. Interrupt happened (stove door opened)
	* Check if we are in fire-state (logging temperature etc.) -> if not continue normally, otherwise, somebody might have re-lit the fire -> ???
	* Open air inlet completely -> 10V on MOTOR_OUTPUT_PIN until reference signal on MOTOR_INPUT_PIN matches READ_AIR_INLET_OPEN_VOLTAGE
	* Start reading from PIN_TEMP_SENSOR if it does not change in 30 minutes close air inlet and go back to deep sleep (wait for interrupt)
	* If temp-signal changes more then **5°** start logging the temperature in a [120x1] int-Array to conduct temperature curve analysis
	* Every 5 minutes, conduct temp-curve analysis, if the curve is truely falling, divide current temperature in 8 closing segments (e.g. 160°-200° = completely open, 120°-160° = 87.7%open etc.)
	* When temperature becomes steady again (air inlet should `hopefully` be at that point around 12% open) close it completely
	* Keep logging and doing curve analysis for another 30 minutes to be sure, the fire is not relit again 
  
## KONSTANTEN
- MOTOR_OUTPUT_PIN `integer`
- WRITE_AIR_INLET_CLOSE_VOLTAGE `float`
- WRITE_AIR_INLET_OPEN_VOLTAGE `float`
- READ_AIR_INLET_OPEN_VOLTAGE `float`


  
## Project Components
### [Thermocouple Digital Converter](https://cdn-shop.adafruit.com/datasheets/MAX31855.pdf)
SPI-output wird in 14 bit signed dargestellt (integer)
### [Selfmade Voltage Stepper](https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWEBmAHAJmgdgGzoRmACzICcpkORIC6NNkNApgLRhgBQA5iEXCOlSpe-ZOkYSOAJwF9wORn0ZgFUBpA4BjEctVKBQtTHiRkETKRwJSgrAixEEkIjlRJjkTgCUQl+Yyp-NTonRmRodzVOABNfHCD0ORVGOmimADMAQwBXABsAF2kdIKJUFMSokw4Ad14ygQr9QWENPlIaGyDXYWTwKGh4thhkIhUcCmRrVGIcc3CIDR4qZU7lhupJHzWwQRBtnaMRcDoJfoQa4t7rOjBcKG49wzEQOzpnyVrruI6bog2Lr5+brfDQyYFAp4ndRFYG9cHxFLwDg+V7fMqMPynOSRCQRB7A5qPYTIZAI+5gwx+VGY8BI2Kw1QEyDCVIZHIFDj0wziYQEhDxVlZPKFCkswx8slgKoAewEEECikg5Fp-TgYH5+GCcpQHDQKBAADEIJEpYw2CAACLZfIATwAOgBnADCNs0uSYHCAA)

Wird über MOTOR_OUTPUT_PIN (analog) von WRITE_AIR_INLET_CLOSE_VOLTAGE bis WRITE_AIR_INLET_OPEN_VOLTAGE angesteuert


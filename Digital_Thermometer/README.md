## Digital Thermometer with LCD Display (I2C)

A digital thermometer project that reads temperature from an LM75 sensor and displays it on a 16x2 LCD via I2C, implemented on an ATmega16A microcontroller in the Atmel Studio environment.

## About the project

The project demonstrates working with microcontroller peripherals and I2C communication protocol:

--- I2C (TWI) communication with LM75 temperature sensor and PCF8574 I2C LCD backpack

--- Reading temperature data and converting it to Celsius

--- Displaying temperature on an LCD screen

--- Implementing a state machine for handling I2C operations

## Technical features

**Microcontroller:** ATmega16A

**Programming language:** C (MISRA-C style)

**Development Environment:** Atmel Studio

**Sensors:** LM75 temperature sensor

**Display:** 16x2 LCD with PCF8574 I2C interface

**Communication protocol:** I2C (TWI)

## Project structure

Digital_Thermometer/

├── main.c              # Main source code

├── schematic.txt       # Connection diagram

└── Digital_Thermometer.atsln  # Atmel Studio solution file

## Additional components:

**Boost resistors:** 4.7 Kohm between SCL and VCC, as well as between SDA and VCC

**LCD Backlight Resistor:** 220 ohms between P3 and VCC

**Quartz resonator:** 8 MHz between XTAL1 and XTAL2

**Capacitors:** 22 pF from XTAL1 to GND, 22 pF from XTAL2 to GND

**Reset button:** 10 kohms between RST and VCC, button between RST and GND

**Filter condenser:** 100 nF between VCC and GND

## Using

1. Assemble the circuit as described above.
2. Open the project in Atmel Studio.
3. Compile and flash the microcontroller.
4. The temperature will be displayed on the LCD display.

## Author

**Sergeykisa228**

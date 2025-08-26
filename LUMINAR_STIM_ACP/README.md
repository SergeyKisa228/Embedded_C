# LUMINAR_STIM_ACP - ATmega16A Smart Lamp

A smart lamp project with brightness control via a potentiometer and several operating modes, implemented on an ATmega16A microcontroller in the Atmel Studio environment.

## About the project

The project demonstrates working with microcontroller peripherals on bare metal without using HAL:
- PWM (TIM1) to control the brightness of the LED
- ADC for reading values from a potentiometer
- Handling interrupts from the button and ADC
- Implementation of a finite state machine for switching operating modes

## Technical features

- **Microcontroller**: ATmega16A
- **Programming language**: C (MISRA-C style)
- **Development Environment**: Atmel Studio
- **Operating modes**:
- Manual brightness control
  - Blinking mode with adjustable frequency
  - Smooth brightness change
 
## Project structure
LUMINAR_STIM_ACP/

├── main.c **# The main source code**

├── schematic.txt **# Сonnection diagram**

└── LUMINAR_STIM_ACP.atsln **# The file for Atmel Studio**

## Using

1. Assemble the circuit as described above
2. Open the project in Atmel Studio
3. Compile and flash the microcontroller
4. Adjust the brightness with the potentiometer
5. Switch the operating modes with the button

## Operating modes

- **Manual mode**: Brightness directly depends on the position of the potentiometer
- **Flashing mode**: The LED flashes at a frequency controlled by the potentiometer
- **Breathing mode**: The LED smoothly changes brightness according to the sinusoidal law

## Author

**SergeyKisa228** 

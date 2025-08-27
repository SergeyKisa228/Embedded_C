## Digital Voltmeter for STM32F103C8T6

This project is a digital voltmeter based on the STM32F103C8T6 microcontroller (Blue Pill), 
which measures the external analog voltage and transmits the measurement results to a computer via a serial UART port.

## Main Features

**Voltage measurement in the range:** 0-3.3V

**ADC resolution:** 12 bits (4096 values)

**Measurement frequency:** 1000 Hz

**Data transfer interface:** UART (9600 baud)

**Accuracy:** ±0.01V (after calibration)

## Configuring a project in STM32CubeIDE

***1. Creating a project***

    --- File → Create → The STM32 project

    --- Select the STM32F103C8T6 microroller

    --- Create a project called "Voltmeter"

***2. Configure peripherals in the .ioc file***

**ADC1:**

    --- Channel: ADC_CHANNEL_0 (PA0)

    --- External trigger conversion source: Timer recording 2 
        Comparison of 2 events

    --- DMA settings: Cyclic mode, High priority

    --- Sampling time: 71.5 cycles

**TIM2:**

    --- Channel2: CH2 output comparison

    --- Mode: Enable matching

    --- Pulse: 5 

    --- Prescaler: 6399

    --- Counter period: 9

**USART1:**

    --- Mode: Asynchronous

    --- Baud Rate: 9600 baud

**Setting the clock frequency:**

    --- SYSCLK: 64 MHz

    --- APB1: 32 MHz

    --- APB2: 16 MHz

    --- ADC pre-scaler: /2

***3. Configuring the linker to support float in printf***

    --- Project → Properties → C/C++ Build → Settings

    --- Tool Settings → MCU Linker → Miscellaneous

    --- In the "Other flags" field, add: -u _printf_float

## Using
**a)** Connect the measured voltage to pin PA0 (do not exceed 3.3V!)

**b)** Connect the USB-UART converter to your computer

**c)** Open the serial terminal with the settings:

    --- Baud Rate: 9600

    --- Data Bits: 8

    --- Stop Bits: 1

    --- Parity: None

    --- Flow Control: None

**d)** The voltmeter will output voltage values in the format: **Voltage: X.XX V**

## Implementation features

--- Using DMA for automatic transmission of ADC data

--- TIM2 timer for accurate measurement periodicity

--- Averaging 16 measurements to improve accuracy

--- Calibrating the ADC at startup

--- Surge protection at the software level

## Technical details
1. Operating frequency: 64 MHz

2. ADC reference voltage: 3.3V

3. Resolution: 0.0008V (3.3V/ 4096)

4. Measurement period: 1 ms

5. Withdrawal period: 1 s

## Important notes
--- Do not apply a voltage higher than 3.3V to the PA0 input!

--- Use a voltage divider to measure higher voltages.

--- Use a stable 3.3V power supply

## Author

**SergeyKisa228**

## Links:

**STM32F103C8T6** documentation - https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html

**STM32CubeIDE** - https://www.st.com/en/development-tools/stm32cubeide.html

## Multitasking system on FreeRTOS for STM32F103C8T6

This project demonstrates the creation of a multitasking system on FreeRTOS for the STM32F103C8T6 microcontroller (Blue Pill).
The system includes three tasks running in parallel: LED control, button polling, and data output to the UART.

## Project Features

--- Using FreeRTOS for Multitasking

--- Inter-task communication via Queues

--- Synchronization of access to the UART using mutexes

--- Anti-rattling button treatment

--- Debugging information output via UART

## Hardware components

1. STM32F103C8T6 board (Blue Pill)

2. LED (built-in on PC13 or external)

3. Button

4. 220 ohm resistor (for external LED)

5. USB-UART Converter (CH340G, CP2102 or FT232RL)

6. ST-Link/V2 programmer-debugger

## Assembly instructions

1. Clone the repository:

2. Open the project in STM32CubeIDE:

    --- File → Import → Existing Projects into Workspace

    --- Select the project folder

3. Assemble the project:

    --- Project → Build All

4. Connect the equipment according to the diagram

5. Flash the microcontroller:

    --- Run → Debug for debugging

    --- Run → Run to run

## Setting up a project

The project was created in STM32CubeIDE using the latest technologies:

***Controller:*** STM32F103C8T6

***FreeRTOS:*** CMSIS_V2 interface

***GPIO:***

    --- PC13: Output (LED)

    --- PA0: Push-Up input

***USART1:*** Asynchronous mode (115200 baud, 8N1)

***Time base source:*** TIM4

## How the project works

The project consists of three tasks:

1. vTaskLED: The LED flashes on the PC13 pin with an interval of 500 ms

2. vTaskButton: Polls the button on pin PA0, sends the click counter to the queue

3. vTaskUART: Accepts data from the queue and outputs the "Button pressed X times" message to the UART

Data is transferred between tasks via a message queue (queueButton), and access to the UART is protected by a mutex (uartMutexHandle).

## Author

**SergeyKisa228**

## Links:

**STM32F103C8T6** documentation - https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html

**FreeRtos** - https://www.freertos.org/

**STM32CubeIDE** - https://www.st.com/en/development-tools/stm32cubeide.html

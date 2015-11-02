# ROCKCLOCK
the clock that rocks

## Purpose
The rockclock project realizes a clock display based on a 60 element RGB colored LED ring. Therefore its not only a clock, but
also some nice eyecandy.

## Usage
Simply power up the system with a powersupply with at least 2A current. The rockclock powers up and searchs for a valid DCF77 signal while
the clock starts with time 0:00. The 12'o'clock point is shown as white dot on the LED ring, hour is yellow, minutes is green and the second
is displayed in cyan color.

## Tech

### Prerequisites
* STM32F051 controller (STM32 Discovery reference board)
* 60 element WS2812 LED ring
* HC-05 bluetooth USART bridge
* DCF77 receiver module
* GCC Arm 4.9.3 toolchain
* libopencm3

### Hardware
The central hardware is the STM32F051 discovery board, where all other components are connected.
The board is powered over the USB debug connector.
#### Wiring
##### WS2812 LED ring
The WS2812 LED ring is a 5V device, therefore a level shifter is needed for the data connection to STM32.
The 74HCT245 level shifter is connected to Port GPIOB, Pin 0 of STM32 and 5V output is connected to LED data in chain. If further
LED chains are needed the STM32 ports GPIOB,1 to GPIOB,7 are usable.

##### Bluetooth communication
For communication with other systems a bluetooth module is used. The bluetooth module is a HC05 type. The HC05 is connected directly to
the STM32 over following port pins:
Function|Port
--------|----
 RX | GPIOA,Pin 2 
 TX | GPIOA, Pin 3 
 Config Mode | GPIOA, Pin 1 

##### DCF77 receiver
The DCF77 reciever module should provide a TTL digital output. It is connected to Port GPIOC, Pin0.

##### Debug LED support
The software uses the LED on discovery board for debugging purposes, it is connected to GPIOC, Pin 8.

### Software
#### Environment
The software is designed around STM32 platform and has following dependencies:
* libopencm3 for low level STM32 register access
* GNU ARM gcc cross compiler chain ver. 4.9.3
* Eclipse internal build

#### Architecture
The architecture of the rockclock is a component based model. These components are layered, so that the hardware platform could be replaced
without changing the application itself.

##### Architecture convenience
* First level components are ordered in subdirectories:
Component | Description 
----------|------------
app|The main application
com|communication subsystem
hal|Hardware abstraction
os|Operating system
services|Different Services
* Subcomponents have the following file layout:
File pattern| Purpose
------------|--------
component.c|Implementation of the component
component.h|Internal declarations of the component
component_config.h|Component configuration definitions
component_if.h|Component outer interface

##### HAL
The HAL abstracts the hardware dependent parts. It contains everything which is platform specific.

##### OS
The OS realizes simple operating system services like a scheduler.

##### Services
The Service component provides servcies for other componets like bit operations, PRNG.

##### COM
The COM component contains the communication stack for different protocols

##### APP
The APP component realizes the functional application itself.



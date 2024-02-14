# Open-Firmware-for-PCAN-USB-devices
This code allows simulating the operation of the device PCAN-USB and using it with the original driver for PCAN-USB
This code is a rework based on the LUFA library; however, its advantage is that it is not tied to any external libraries other than the ones standard for the Atmega family. Specifically, it was developed for the Arduino Uno platform.

To use it, you only need to have AVRDUDE, AVR Studio 4, and the AVR toolchain installed.

Compile the code and upload it to the Atmega16U2 as part of the Arduino Uno platform using the ICSP interface

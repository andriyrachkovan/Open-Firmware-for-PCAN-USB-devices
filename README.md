# Open Firmware for PCAN-USB Devices

This code enables the simulation of PCAN-USB device operations and allows its use with the original PCAN-USB driver.

## Usage

To utilize this firmware, follow the steps below:

1. **Compile the Code:**
   - Compile the firmware code using the provided makefile.
   - Utilize the standard AVR toolchain for seamless integration.

2. **Upload to Arduino Uno Platform:**
   - Compile the code and upload it to the Atmega16U2 as part of the Arduino Uno platform using the ICSP interface.

## Features

- **Library Independent:** This firmware is reworked based on the LUFA library, providing flexibility while not relying on external libraries other than the standard ones for the Atmega family.
- **Arduino Uno Platform:** Specifically developed for the Arduino Uno platform.

## Requirements

Ensure you have the following tools installed:

- AVRDUDE for uploading the firmware.
- AVR Studio 4 for development.
- AVR toolchain for compiling the firmware.

## Contributing

Feel free to contribute to the development of this firmware. Check out the [contribution guidelines](CONTRIBUTING.md) for more details.

## License

This firmware is released under the [CC0 1.0 Universal (CC0 1.0) Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/). See the [LICENSE](LICENSE) file for details.

Contact us at [andriy.rachkovan@gmail.com] if you have any questions or need assistance.

Happy coding!

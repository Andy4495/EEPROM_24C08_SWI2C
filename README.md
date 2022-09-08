# EEPROM_24C08_SWI2C Library

[![Arduino Compile Sketches](https://github.com/Andy4495/EEPROM_24C08_SWI2C/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/EEPROM_24C08_SWI2C/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/EEPROM_24C08_SWI2C/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/EEPROM_24C08_SWI2C/actions/workflows/CheckMarkdownLinks.yml)

This library interfaces with the 24C08 external I2C 8Kx1 EEPROM. It uses a software I2C implementation, and can therefore use any 2 I/O pins for interfacing with the EEPROM. It only provides simple 1-byte read and write commands. It does not currently support sequential reads or writes.

The library uses the "Polling on ACK" method described in section 5.1.3 of the [datasheet][1] to check if the chip has completed a previous write cycle, and therefore does not have any hardcoded delays.

This library depends on a specific [SWI2C library][5].

## Usage

_Be sure to review the example sketch included with the library._

First, **include** the library header file:

```cpp
#include <EEPROM_24C08_SWI2C.h>
```

Note that this library depends on the [SWI2C][5] library.

Next, **instantiate** an EEPROM_24C08_SWI2C object:

```cpp
EEPROM_24C08_SWI2C eep(uint8_t sda_pin, uint8_t scl_pin, uint8_t device_address);
```

If using a separate I/O pin to control WC (Write Control) on the EEPROM, then use the following constructor:

```cpp
EEPROM_24C08_SWI2C eep(uint8_t sda_pin, uint8_t scl_pin, uint8_t device_address, uint8_t wc_pin);
```

`sda_pin` is the pin number for the SDA signal, `scl_pin` is the pin number for the SCL signal, and `wc_pin` is the pin number for the WC signal (if used).

`device_address` is the 7-bit I2C address for memory address 0x0000 of the EEPROM. This is typically 0x50 for 24C08, but may be 0x54 if the E2 signal is pulled high on the EEPROM chip.

Then **initialize** the object (typically within `setup()`):

```cpp
eep.begin();
```

Remember to use pullup resistors on the SDA and SCL lines.

### Library Methods

```cpp
void write(int address, byte data);

int writeAndVerify(int address, byte data); // Returns 0 if write successful, 1 if unsuccessful

byte read(int address);
```

## References

+ [SWI2C library][5]
+ 24C08 [datasheet][1]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]: https://www.st.com/resource/en/datasheet/m24c08-r.pdf
[5]:https://github.com/Andy4495/SWI2C
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[//]: # ([200]: https://github.com/Andy4495/EEPROM_24C08_SWI2C)

[//]: # (Dead link from previous version of README: http://www.bgmicro.com/pdf/m24c08.pdf)

EEPROM_24C08_SWI2C Library
==========================

This library interfaces with the 24C08 external I2C 8Kx1 EEPROM. It uses a software I2C implementation, and can therefore use any 2 I/O pins for interfacing with the EEPROM. It only provides simple 1-byte read and write commands.

This library depends on a specific [SWI2C library][5].

Usage
-----

_Be sure to review the example sketch included with the library._

First, **include** the library header file:

    #include <SWI2C.h>
    #include <EEPROM_24C08_SWI2C.h>

Note that `<SWI2C.h>` is required, since this library depends on the [SWI2C][5] library.

Next, **instantiate** an EEPROM_24C08_SWI2C object:

    EEPROM_24C08_SWI2C eep(uint8_t sda_pin, uint8_t scl_pin, uint8_t device_address);

`sda_pin` is the pin number for the SDA signal, `scl_pin` is the pin number for the SCL signal.

`device_address` is the 7-bit I2C address for memory address 0x0000 of the EEPROM. This is typically 0x50 for 24C08, but may be 0x54 if the E2 signal is pulled high on the EEPROM chip.

Then **initialize** the object (typically within `setup()`):

    eep.begin();

Remember to use pullup resistors on the SDA and SCL lines.

#### Library Methods ####

    void write(int address, byte data);

    int writeAndVerify(int address, byte data); // Returns 0 if write successful, 1 if unsuccessful

    byte read(int address);



References
---------------------

+ [SWI2C library][5]
+ 24C08 [datasheet][1]

[1]:http://www.bgmicro.com/pdf/m24c08.pdf
[5]:https://github.com/Andy4495/SWI2C

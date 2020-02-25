/* -----------------------------------------------------------------
   EEPROM_24C08_SWI2C Library
   https://github.com/Andy4495/EEPROM_24C08_SWI2C

   This library requires that the following library also be installed:
   https://github.com/Andy4495/SWI2C

   02/24/20 - A.T. - Original

*/

#ifndef EEPROM_24C08_SWI2C_H
#define EEPROM_24C08_SWI2C_H

#include "Arduino.h"
#include <SWI2C.h>



class EEPROM_24C08_SWI2C {
public:
  EEPROM_24C08_SWI2C(uint8_t sda_pin, uint8_t scl_pin, uint8_t addr);
  ~EEPROM_24C08_SWI2C();
  void begin();
  void write(int address, byte data);
  int writeAndVerify(int address, byte data);
  byte read(int address);

private:
  SWI2C* eep0;
  SWI2C* eep1;
  SWI2C* eep2;
  SWI2C* eep3;
};

#endif

/* -----------------------------------------------------------------
   BQ27441_SWI2C Library
   https://github.com/Andy4495/BQ27441_SWI2C

   This library requires that the following library also be installed:
   https://github.com/Andy4495/SWI2C


   10/06/2018 - A.T. - Original
   10/17/2018 - A.T. - Use the new SWI2C library method "write2write2bToRegister()"
                     - Implement specific methods for each command opcode

*/

#include "EEPROM_24C08_SWI2C.h"
EEPROM_24C08_SWI2C::EEPROM_24C08_SWI2C(uint8_t sda_pin, uint8_t scl_pin, uint8_t addr) {
  // addr is optional, and defaults to BQ27441_I2C_ADDRESS
  eep0 = new SWI2C(sda_pin, scl_pin, addr);
  eep1 = new SWI2C(sda_pin, scl_pin, addr | 0x01);
  eep2 = new SWI2C(sda_pin, scl_pin, addr | 0x02);
  eep3 = new SWI2C(sda_pin, scl_pin, addr | 0x03);

}

EEPROM_24C08_SWI2C::~EEPROM_24C08_SWI2C() {
  delete eep0;
  delete eep1;
  delete eep2;
  delete eep3;
}

void EEPROM_24C08_SWI2C::begin() {
  eep0->begin();
  eep1->begin();
  eep2->begin();
  eep3->begin();
}

void EEPROM_24C08_SWI2C::write(int address, byte data) {
  // Future enhancement: instead of delaying at end, check if device is ready
  // before writing, and only delay if device not ready.
  int upper_bits;

  upper_bits = (address & 0x0300) >> 8; // Extract A9 and A8
  switch(upper_bits) {
    case 0:
      eep0->writeToRegister(address & 0xFF, data);
      break;
    case 1:
      eep1->writeToRegister(address & 0xFF, data);
      break;
    case 2:
      eep2->writeToRegister(address & 0xFF, data);
      break;
    case 3:
      eep3->writeToRegister(address & 0xFF, data);
      break;
    }
    delay(6);
}

int EEPROM_24C08_SWI2C::writeAndVerify(int address, byte data) {
  write(address, data);
  if (read(address) == data) return 0; // Verify successful
  else return 1;                       // Verify failed
}

byte EEPROM_24C08_SWI2C::read(int address) {
  // Extract upper 2 address bits
  // choose proper object based on address bits
  // read byte
  int upper_bits;
  byte data;

  upper_bits = (address & 0x0300) >> 8; // Extract A9 and A8
  switch(upper_bits) {
    case 0:
      eep0->read1bFromRegister(address & 0xFF, &data);
      break;
    case 1:
      eep1->read1bFromRegister(address & 0xFF, &data);
      break;
    case 2:
      eep2->read1bFromRegister(address & 0xFF, &data);
      break;
    case 3:
      eep3->read1bFromRegister(address & 0xFF, &data);
      break;
    }
    return data;
}

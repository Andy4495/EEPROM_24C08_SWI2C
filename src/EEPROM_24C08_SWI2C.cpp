/* -----------------------------------------------------------------
   EEPROM_24C08_SWI2C Library
   https://github.com/Andy4495/EEPROM_24C08_SWI2C

   This library requires that the following library also be installed:
   https://github.com/Andy4495/SWI2C


   02/25/2020 - A.T. - Original

*/

#include "EEPROM_24C08_SWI2C.h"
EEPROM_24C08_SWI2C::EEPROM_24C08_SWI2C(uint8_t sda_pin, uint8_t scl_pin,
                                       uint8_t addr) {
  eep0 = new SWI2C(sda_pin, scl_pin, addr);
  eep1 = new SWI2C(sda_pin, scl_pin, addr | 0x01);
  eep2 = new SWI2C(sda_pin, scl_pin, addr | 0x02);
  eep3 = new SWI2C(sda_pin, scl_pin, addr | 0x03);
  _wc_pin = NO_PIN;
}

EEPROM_24C08_SWI2C::EEPROM_24C08_SWI2C(uint8_t sda_pin, uint8_t scl_pin,
                                       uint8_t addr, uint8_t wc_pin) {
  eep0 = new SWI2C(sda_pin, scl_pin, addr);
  eep1 = new SWI2C(sda_pin, scl_pin, addr | 0x01);
  eep2 = new SWI2C(sda_pin, scl_pin, addr | 0x02);
  eep3 = new SWI2C(sda_pin, scl_pin, addr | 0x03);
  _wc_pin = wc_pin;
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
  if (_wc_pin != NO_PIN) {
    digitalWrite(_wc_pin, HIGH);
    pinMode(_wc_pin, OUTPUT);
  }
}

void EEPROM_24C08_SWI2C::write(int address, byte data) {
  int upper_bits;
  SWI2C* eep;
  uint8_t ack_bit;

  upper_bits = (address & 0x0300) >> 8; // Extract A9 and A8
  switch(upper_bits) { // select the correct SWI2C object based on A9/A8
    case 0:
      eep = eep0;
      break;
    case 1:
      eep = eep1;
      break;
    case 2:
      eep = eep2;
      break;
    case 3:
      eep = eep3;
      break;
    }
    // Enable the WC Write Control pin if needed:
    if (_wc_pin != NO_PIN) {
      digitalWrite(_wc_pin, LOW);
    }
    // Poll for ACK before writing, to see if EEPROM has completed previous
    // write cycle. This allows us to avoid a hard-coded delay at end of write.
    // See section 5.1.3 of datasheet
    //   Send device ID and check for ACK or NACK
    //     If NACK, then try again
    //     If ACK, then continue with the write cycle as normal
    // The following eep->() calls are equivalent to eep->writeToRegister(address & 0xFF, data);
    do {
      eep->startBit();
      eep->writeAddress(0);
      ack_bit = eep->checkAckBit();
    } while (ack_bit == HIGH);
    eep->writeRegister(address & 0xFF);
    eep->checkAckBit();
    eep->writeByte(data);
    eep->checkAckBit();
    eep->stopBit();

    // Disable WC Write Control pin if needed:
    if (_wc_pin != NO_PIN) {
      digitalWrite(_wc_pin, HIGH);
    }

    // delay(6);  // delay not needed, since we are polling before writing
}

// This function busy-waits until write operation is complete, and is
// therefore considerably slower than just writing.
int EEPROM_24C08_SWI2C::writeAndVerify(int address, byte data) {
  int upper_bits;
  SWI2C* eep;
  uint8_t ack_bit;

  upper_bits = (address & 0x0300) >> 8; // Extract A9 and A8
  switch(upper_bits) { // select the correct SWI2C object based on A9/A8
    case 0:
      eep = eep0;
      break;
    case 1:
      eep = eep1;
      break;
    case 2:
      eep = eep2;
      break;
    case 3:
      eep = eep3;
      break;
  }

  write(address, data);
  // Wait for write cycle to complete. See section 5.1.3 of datasheet.
  do {
    eep->startBit();
    eep->writeAddress(0);
    ack_bit = eep->checkAckBit();
  } while (ack_bit == HIGH);
  eep->stopBit();

  if (read(address) == data) return 0; // Verify successful
  else return 1;                       // Verify failed
}

byte EEPROM_24C08_SWI2C::read(int address) {
  // Extract upper 2 address bits
  // choose proper object based on address bits
  // read byte
  int upper_bits;
  byte data;
  SWI2C* eep;
  uint8_t ack_bit;

  upper_bits = (address & 0x0300) >> 8; // Extract A9 and A8
  switch(upper_bits) {  // select the correct SWI2C object based on A9/A8
    case 0:
      eep = eep0;
      break;
    case 1:
      eep = eep1;
      break;
    case 2:
      eep = eep2;
      break;
    case 3:
      eep = eep3;
      break;
    }
    // Check to make sure we aren't still in write cycle before reading
    // See section 5.1.5 of datasheet
    // The following is equivalent to eep->read1bFromRegister(address & 0xFF, &data);
    do {
      eep->startBit();
      eep->writeAddress(0);
      ack_bit = eep->checkAckBit();
    } while (ack_bit == HIGH);
    eep->writeRegister(address & 0xFF);
    eep->checkAckBit();
    eep->startBit();
    eep->writeAddress(1); // 1 == Read bit
    eep->checkAckBit();
    data = eep->read1Byte();
    eep->checkAckBit(); // Master needs to send NACK when done reading data
    eep->stopBit();

    return data;
}

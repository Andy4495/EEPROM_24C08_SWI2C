/* -----------------------------------------------------------------
   EEPROM_24C08_SWI2C Library Example Program
   https://github.com/Andy4495/EEPROM_24C08_SWI2C

   This library requires that the following library also be installed:
   https://github.com/Andy4495/SWI2C


   02/25/2020 - A.T. - Original

*/

#include "SWI2C.h"
#include "EEPROM_24C08_SWI2C.h"

EEPROM_24C08_SWI2C eep(6, 5, 0x50);
//  EEPROM_24C08_SWI2C eep(6, 5, 0x50, 7);  // Constructor if controlling WC pin on EEPROM chip

byte data;
int i, j;
int result; 

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Startup");
  delay(1000);

  eep.begin();

  Serial.println("SWI2C Initialized");

  Serial.print("Read a byte from address 0x005: ");
  data = eep.read(5);
  Serial.println(data, HEX);



  for (i = 0; i < 1024; i++) {
    data = i;
    eep.write(i, data);
    /*
    result = eep.writeAndVerify; 
    if (result != 0) {
      Serial.print("Verify Failed: "); 
      Serial.print(i, HEX); 
      Serial.print(", "); 
      Serial.println(data, HEX); 
    }
    */
  }

  for (i = 0; i < 1024 / 16; i++) {
    Serial.print(i * 16, HEX);
    Serial.print(": ");
    for (j = 0; j < 16; j++) {
      data = eep.read(i * 16 + j);
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
  }

}

void loop(void)
{

}

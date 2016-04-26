/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int address = 0x50;

    /***
    Need to divide by 4 because analog inputs range from
    0 to 1023 and each byte of the EEPROM can only hold a
    value from 0 to 255.
  ***/

  //int value = analogRead(0) / 4;
  
  
void setup() {
    // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
}

void loop() {
  // set a value to write to register	
  int value = 0;
  
  // write the value to current register
  EEPROM.write(address, value);
  
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  // print the value read from the register to the serial terminal
  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  
  // increment the value read from register
  value++;
 
  /***
    Write the value to the appropriate byte of the EEPROM.
    these values will remain there when the board is
    turned off.
  ***/
  EEPROM.write(address, value);
  
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  // print the value read from the register to the serial terminal
  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();

  /***
    Advance to the next address, when at the end restart at the beginning.

    Larger AVR processors have larger EEPROM sizes, E.g:
    - Arduno Duemilanove: 512b EEPROM storage.
    - Arduino Uno:        1kb EEPROM storage.
    - Arduino Mega:       4kb EEPROM storage.

    Rather than hard-coding the length, you should use the pre-provided length function.
    This will make your code portable to all AVR processors.
  ***/
  
  address= address + 1;
  if (address== EEPROM.length()) {
    address= 0;
  }

  /***
    As the EEPROM sizes are powers of two, wrapping (preventing overflow) of an
    EEPROM address is also doable by a bitwise and of the length - 1.

    ++address&= EEPROM.length() - 1;
  ***/

  delay(1000);
}

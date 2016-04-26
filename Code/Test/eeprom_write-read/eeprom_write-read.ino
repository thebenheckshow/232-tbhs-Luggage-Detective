/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
//int address = 0x50;
    
void setup() {
    // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(A2, OUTPUT);  
}

void loop() {
  // set a value to write to register	
  int value = 0;
  
  digitalWrite(A2, HIGH);
  
  for (int registerCount = 0; registerCount < 256; registerCount++){
	EEPROM.write(registerCount, registerCount);
  }
  
  for (int registerCount = 0; registerCount < 256; registerCount++){
	// read a byte from the current address of the EEPROM
	value = EEPROM.read(registerCount);
	// print the value read from the register to the serial terminal
	Serial.print(registerCount);
	Serial.print("\t");
	Serial.print(value, DEC);
	Serial.println();
	delay(10);
  }
  
  digitalWrite(A2, LOW);
  
  delay(1000);
}

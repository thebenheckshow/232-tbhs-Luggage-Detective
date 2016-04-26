#include <Wire.h>

#define INTERRUPT_1 0
#define ACCELEROMETER 0x1C

void setup()
{
  Wire.begin();         // join i2c bus (address optional for master)
  Serial.begin(9600);   // start serial for output
  pinMode(INTERRUPT_1, INPUT); // INTERRUPT_1
}

void loop()
{
  /* 
  // start the communication ACCELEROMETER
  Wire.beginTransmission(ACCELEROMETER);
  Serial.println(" 19: Wire.beginTransmission(ACCELEROMETER); ");  
  // send the R/W bit 0 = write
  Wire.write(0);
  Serial.println(" 22: Wire.write(0); ");
  // end transmission
  Wire.endTransmission();
  Serial.println(" 25: Wire.endTransmission(); ");

  // start the communication ACCELEROMETER
  Wire.beginTransmission(ACCELEROMETER);
  Serial.println(" 29: Wire.beginTransmission(ACCELEROMETER); ");  
  // send the address to read
  Wire.write(0x02); // 0x01 = OUT_X_MSB
  Serial.println(" 32: Wire.write(0x01); ");
  // end transmission
  Wire.endTransmission();
  Serial.println(" 35: Wire.endTransmission(); ");
    
  // start the communication ACCELEROMETER
  Wire.beginTransmission(ACCELEROMETER);
  Serial.println(" 39: Wire.beginTransmission(ACCELEROMETER); ");   
  // send the R/W bit 1 = read
  Wire.write(1);
  Serial.println(" 42: Wire.write(0x01); ");
  // end transmission
  Wire.endTransmission();
  Serial.println(" 45: Wire.endTransmission(); ");
  
  //while(Wire.available() == 0);
  
  int OUT_X_MSB = Wire.read();
  
  Serial.print(" 51: OUT_X_MSB = ");
  Serial.println(OUT_X_MSB);
  */
  
  // start the communication ACCELEROMETER
  Wire.beginTransmission(ACCELEROMETER);
  Serial.println(" Wire.beginTransmission(ACCELEROMETER); ");  
  // send the R/W bit 0 = write
  Wire.write(0);
  Serial.println(" Wire.write(0); ");
  // send the address to read
  Wire.write(0x02); // 0x01 = OUT_X_MSB
  Serial.println(" Wire.write(0x01); ");
  // send the R/W bit 1 = read
  Wire.write(1);
  Serial.println(" Wire.write(1); ");
  
  int OUT_X_MSB = Wire.read();
  Serial.print(" OUT_X_MSB = ");
  Serial.println(OUT_X_MSB);
  
  // end transmission
  Wire.endTransmission();
  Serial.println(" Wire.endTransmission(); ");
  
  
 /* Wire.requestFrom(ACCELEROMETER, 1);    // request 1 bytes from slave device 

  while (Wire.available())   // slave may send less than requested
  {
    int OUT_X_MSB = Wire.read();
    Serial.print(" OUT_X_MSB = ");
    Serial.println(OUT_X_MSB);	
  }*/
  
  delay(1000);
}
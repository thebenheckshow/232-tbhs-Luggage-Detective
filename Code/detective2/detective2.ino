#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

float newX = 0;
float newY = 0;
float newZ = 0;

float lastX = 0;
float lastY = 0;
float lastZ = 0;

float tolX = 10;
float tolY = 10;
float tolZ = 10;

float capX = 0;
float capY = 0;
float capZ = 0;

float aveX = 0;
float aveY = 0;
float aveZ = 0;

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 8;
const int ledPin = 2;
const int button = 4;

int state = 0;
int logTarget = 0;
int flag = 0;

unsigned long sampleNumber = 0;
int tenthSecond = 0;

File dataFile;
String dataString;

void setup() {

  pinMode(ledPin, 1);
  pinMode(button, INPUT);
  digitalWrite(button, 1);    //Set internal pullup
  

  Serial.begin(9600);

  Serial.println("Adafruit MMA8451 test!");
  
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }

  blink(2);

  delay(200);
  
  mma.setRange(MMA8451_RANGE_2_G);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("FAIL");
    return;
  }
  Serial.println("OK!");

  if (SD.exists("movedata.csv")) {
    Serial.println("Erasing existing file");
    SD.remove("movedata.csv");
  }

  if (digitalRead(button) == 0) {       //Button held down on boot? Log to serial
    logTarget = 1; 
    blink(4);  
    delay(250);
    Serial.println("Serial terminal mode");    
    Serial.println("Please RELEASE BUTTON to continue");
    while(digitalRead(button) == 0) {
      delay(1);   
    }
    state = 1;
    delay(250);    
  }
  else {
    logTarget = 0;
    state = 1; 
    blink(2);
    Serial.println("SD File Write Mode");   
    delay(500);       
  } 

  Serial.println("LOGGING BEGIN");
}

void loop() {

  switch(state) {
    
    case 1:
      getData();
    break;    
    case 9:
      blink(1);
    break;  
    
  }

  if (digitalRead(button) == 0 and state == 1) {   //Button pressed while logging data?
    state = 9;
  }

  delay(100);
  
}

void getData() {

  sensors_event_t event; 
  mma.getEvent(&event);

  lastX = newX;
  lastY = newY;
  lastZ = newZ;

  newX = event.acceleration.x;
  newY = event.acceleration.y;
  newZ = event.acceleration.z - 9;

  if (flag) {   
    flag = 0;        
  }
  else {

    if (newX > (lastX + tolX) or newX < (lastX - tolX)) {
      flag = 1;   
    }
    if (newY > (lastY + tolY) or newY < (lastY - tolY)) {
      flag = 1;  
    }
    if (newZ > (lastZ + tolZ) or newZ < (lastZ - tolZ)) {
      flag = 1;    
    }  
      
  }
 
  if (flag == 1) {
    capX = newX;
    capY = newY;
    capZ = newZ;    
  }
  else {
    aveX += newX;
    aveY += newY;
    aveZ += newZ;    
  }

  tenthSecond += 1;

  if (tenthSecond == 10) {              //One second has passed?
    tenthSecond = 0;

    dataString = "";

    dataString += String(sampleNumber++);
    addComma();
    
    int o = mma.getOrientation();       //Get the orientation at this second

    switch(o) {
      case 0 :
      dataString += "P^B";
      break;
      case 1 :
      dataString += "P^F";
      break;
      case 2 :
      dataString += "PVF";
      break;
      case 3 :
      dataString += "PVB";
      break;  
      case 4 :
      dataString += "L<F";
      break;
      case 5 :
      dataString += "L<B";
      break;
      case 6 :
      dataString += "L>F";
      break;
      case 7 :
      dataString += "L>B";
      break;       
    }

    //dataString += String(o);    
    addComma();

    if (capX and capY and capZ) {       //Did an X-TREME event occur during this second?

      dataString += String(capX);
      addComma();     
      dataString += String(capY);
      addComma(); 
      dataString += String(capZ);
      addComma(); 
      dataString += "1";               
    }
    else {                              //Otherwise, average out the boring values

      dataString += String(aveX / 10);
      addComma();      
      dataString += String(aveY / 10);
      addComma();  
      dataString += String(aveZ / 10);
      addComma(); 
      dataString += "0";             
    }
    
    logData(logTarget);
           
    capX = 0;
    capY = 0;
    capZ = 0;    
    aveX = 0;
    aveY = 0;
    aveZ = 0;    
  }

}

void addComma() {

  dataString += ",";
  
}

void logData(unsigned char whereTo) {

  if (whereTo) {
    Serial.println(dataString);    
  }
  else {
    dataFile = SD.open("movedata.csv", FILE_WRITE);
    dataFile.println(dataString);
    dataFile.close();       
  }

}

void blink(unsigned time) {

  for (int x = 0 ; x < time ; x++) {

    digitalWrite(ledPin, 1);
    delay(100);
    digitalWrite(ledPin, 0);
    delay(100);
        
  }

 
}

void temp() {

    // read three sensors and append to the string:
    for (int analogPin = 0; analogPin < 3; analogPin++) {
      int sensor = analogRead(analogPin);
      dataString += String(sensor);
      if (analogPin < 2) {
        dataString += ",";
      }
    }  
  
}






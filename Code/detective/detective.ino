
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// include the SD library:
#include <SPI.h>
#include <SD.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

float newX = 0;
float newY = 0;
float newZ = 0;

float lastX = 0;
float lastY = 0;
float lastZ = 0;

float tolX = 20;
float tolY = 20;
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

unsigned long sampleNumber = 0;
int tenthSecond = 0;

File dataFile;

void setup(void) {

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

  Serial.print("\nInitializing SD card...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("FAIL");
    return;
  } else {
    Serial.println("OK!");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);

  if (digitalRead(button) == 0) {       //Button held down on boot? Log to serial
    logTarget = 1; 
    blink(4);  
    delay(250);
    Serial.println("Serial terminal mode");    
    Serial.println("Please RELEASE BUTTON to continue");
    while(digitalRead(button) == 0) {
      delay(1);   
    }
    state = 9;
    delay(250);    
  }
  else {
    logTarget = 0;
    state = 9; 
    blink(2);
    dataFile = SD.open("datalog.txt", FILE_WRITE);   
    delay(500);       
  } 
}

void loop() {

  dataFile.println("HELLO!");

  switch(state) {
    
    case 1:
      logData();
    break;    
    case 9:
      blink(1);
    break;  
    
  }

  if (digitalRead(button) == 0 and state == 1) {   //Button pressed while logging data?

    dataFile.close();
    blink(2);
    state = 9;
  }

  delay(100);

}


void logData() {

  sensors_event_t event; 
  mma.getEvent(&event);

  lastX = newX;
  lastY = newY;
  lastZ = newZ;

  newX = event.acceleration.x;
  newY = event.acceleration.y;
  newZ = event.acceleration.z - 9;

  int flag = 0;

  if (newX > (lastX + tolX) or newX < (lastX - tolX)) {
    flag = 1;   
  }
  if (newY > (lastY + tolY) or newY < (lastY - tolY)) {
    flag = 1;  
  }
  if (newZ > (lastZ + tolZ) or newZ < (lastZ - tolZ)) {
    flag = 1;    
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
    if (logTarget) {
      writeDataSerial();   
    }
    else {
      writeDataSD();      
    }
           
    capX = 0;
    capY = 0;
    capZ = 0;    
    aveX = 0;
    aveY = 0;
    aveZ = 0;    
  }

}

void writeDataSerial() {

    Serial.print(sampleNumber++, DEC);
    Serial.print(",");

    int o = mma.getOrientation();       //Get the orientation at this second
    Serial.print(o, DEC);
    Serial.print(",");

    if (capX and capY and capZ) {       //Did an X-TREME event occur during this second?

      Serial.print(capX);
      Serial.print(",");      
      Serial.print(capY);
      Serial.print(",");   
      Serial.print(capZ);         
    }
    else {                              //Otherwise, average out the boring values

      Serial.print(aveX / 10);
      Serial.print(",");      
      Serial.print(aveY / 10);
      Serial.print(",");   
      Serial.print(aveZ / 10);     
    }
    
    Serial.println("");  

}

void writeDataSD() {

    dataFile.print(sampleNumber++, DEC);
    dataFile.print(",");

    int o = mma.getOrientation();       //Get the orientation at this second
    dataFile.print(o, DEC);
    dataFile.print(",");

    if (capX and capY and capZ) {       //Did an X-TREME event occur during this second?

      dataFile.print(capX);
      dataFile.print(",");      
      dataFile.print(capY);
      dataFile.print(",");   
      dataFile.print(capZ);         
    }
    else {                              //Otherwise, average out the boring values

      dataFile.print(aveX / 10);
      dataFile.print(",");      
      dataFile.print(aveY / 10);
      dataFile.print(",");   
      dataFile.print(aveZ / 10);     
    }
    
    dataFile.println("");  

}

void blink(unsigned time) {

  for (int x = 0 ; x < time ; x++) {

    digitalWrite(ledPin, 1);
    delay(100);
    digitalWrite(ledPin, 0);
    delay(100);
        
  }

 
}





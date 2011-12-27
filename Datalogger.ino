// Ported to SdFat from the native Arduino SD library example by Bill Greiman
// On the Ethernet Shield, CS is pin 4. SdFat handles setting SS
const int chipSelect = 8;
/*
 SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Bill Greiman 11 Apr 2011
 This example code is in the public domain.

 */
#include <SdFat.h>
#include <TimerOne.h>

#define INIT_CHAN1 DDRD = DDRD //init pins as INPUT
#define INIT_CHAN2 DDRB = DDRB & (0xFF - 0x06)
#define INIT_CHAN3 DDRC = DDRC & (0xFF - 0x3F)

#define READ_CHAN1 PIND //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC //pins A0, A1

#define MAX_CNT 300 //seconds

SdFat sd;
SdFile outFile;
volatile long cnt = 0;
char filename[] = "tX.log";
byte data[5];
int value = 0;

void of(){
  if( value >= 0 && value <= 90 ) {
    filename[1] = value + 65;
    value++;
  }
  if (!outFile.open(filename, O_RDWR | O_CREAT | O_TRUNC)) {
    sd.errorHalt("opening test.txt for write failed");
  }
}

void stopRecording() {
  //toggle();
  cnt++;
}

void setup() {
  // Serial.begin(9600);
  INIT_CHAN1;
  INIT_CHAN3;

  if (!sd.init(SPI_FULL_SPEED, chipSelect)) sd.initErrorHalt();

  of();

  Timer1.initialize();
  Timer1.attachInterrupt(stopRecording);
}

void loop() {
  if(cnt >= MAX_CNT) {
    unsigned long s = outFile.fileSize();
    //Serial.println(s);
    outFile.close();
    of();
    cnt = 0;
  } else {
    data[0] = READ_CHAN3;
    data[1] = READ_CHAN1;
    outFile.write(data, 2);
  }
}



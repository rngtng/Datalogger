#include <SdFat.h>
#include <TimerOne.h>

#define INIT_CHAN1 DDRD = DDRD & 0x00           //b0000 0000  - init pins as INPUT
#define INIT_CHAN2 DDRB = DDRB & (0xFF - 0x06) // b1111 1001
#define INIT_CHAN3 DDRC = DDRC & (0xFF - 0x3F) // b1100 0000

#define READ_CHAN1 PIND //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC //pins A0, A1

#define MAX_CNT 300 //seconds

SdFat sd;
SdFile outFile;

volatile long cnt = 0;
char filename[] = "tX.log";
int value = 0;

byte data[5];

const int chipSelect = 8; //where SD writer is connected to

void inc() {
  cnt++;
}

void setup() {
  INIT_CHAN1;
  INIT_CHAN3;

  if (!sd.init(SPI_FULL_SPEED, chipSelect)) sd.initErrorHalt();

  nextFile();

  Timer1.initialize();
  Timer1.attachInterrupt(inc);
}

void loop() {
  if(cnt >= MAX_CNT) {
    outFile.close();
    nextFile();
  } else {
    data[0] = READ_CHAN3;
    data[1] = READ_CHAN1;
    outFile.write(data, 2);
  }
}

void nextFile(){
  if( value >= 0 && value <= 90 ) {
    filename[1] = value + 65;
    value++;
  }
  if (!outFile.open(filename, O_RDWR | O_CREAT | O_TRUNC)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  cnt = 0;
}


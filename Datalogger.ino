#include <SD.h>
#include <EEPROM.h>
#include <TimerOne.h>

// 270K -> 10s
// 27K -> 1s
// 27 -> 1ms
// #define MEGA_SOFT_SPI 1

#define READ_CHAN1 PIND & 0x0F //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC & 0x03 //pins 9, 10

#define MAX_CNT 180 //seconds

#define EEPROM_ADR 0

File outFile;
char filename[] = "turtleX.log";
volatile long cnt = 0;

void toggle() {
  digitalWrite(A5, HIGH);
  delay(100);
  digitalWrite(A5, LOW);
  delay(100);
}

void stopRecording() {
  toggle();
  cnt++;
}

void setup() {
  //EEPROM.write(EEPROM_ADR, 0);

  DDRD = 0; //init pins as INPUT
  //pinMode(9, INPUT);
  //pinMode(10, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);
  pinMode(A5, OUTPUT);

  toggle();

  int value = EEPROM.read(EEPROM_ADR);
  if( value >= 0 && value <= 90 ) {
    filename[6] = value + 65; //add 48 to make it ASCII
  }
  EEPROM.write(EEPROM_ADR, ++value);

  SD.begin(8);

  outFile = SD.open(filename, O_WRITE | O_CREAT);

  Timer1.initialize();
  Timer1.attachInterrupt(stopRecording);
}

void loop()
{
  if(cnt >= MAX_CNT) {
    outFile.close();
    digitalWrite(A5, HIGH);
  } else {
    byte port = READ_CHAN3;
    byte data = READ_CHAN1;
    //byte port = READ_CHAN2;
    outFile.write(port);
    outFile.write(data);
  }
}


#include <SD.h>
#include <EEPROM.h>
#include <TimerOne.h>

#define INIT_CHAN1 DDRD = DDRD & (0x0F - 0x0F) //init pins as INPUT
#define INIT_CHAN2 DDRB = DDRB & (0x0F - 0x06)
#define INIT_CHAN3 DDRC = DDRC & (0x0F - 0x03)

#define READ_CHAN1 PIND & 0x0F //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC & 0x03 //pins A0, A1

#define EEPROM_ADR 0
#define MAX_CNT 60 //seconds

File outFile;
char filename[] = "turtleX.log";
volatile long cnt = 0;
byte data[2];

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
  INIT_CHAN1;
  INIT_CHAN3;

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

void loop() {
  if(cnt >= MAX_CNT) {
    outFile.close();
    outFile = SD.open(filename, O_WRITE | O_CREAT);
    digitalWrite(A5, HIGH);
  } else {
    data[0] = READ_CHAN3;
    data[1] = READ_CHAN1;
    outFile.write(data, 2);
  }
}



#include <SD.h>
#include <TimerOne.h>
#include <ByteBuffer.h>
#include <EEPROM.h>

// #define MEGA_SOFT_SPI 1

#define READ_CHAN1 PIND & 0x0F //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC & 0x03 //pins 9, 10

#define SEND_BUFFER_SIZE 1000
#define MAX_COUNT 100000  //4,294,967,295

#define EEPROM_ADR 0

unsigned long cnt = 0;

File file;
ByteBuffer buffer;

char filename[] = "turtleX.log";

int ledState = LOW;

void measure() {
  byte data = READ_CHAN1;
  byte port = READ_CHAN3;
  buffer.put(data);
  buffer.put(port);
}

void toggle() {
  ledState = (ledState == LOW) ? HIGH : LOW;
  digitalWrite(A5, ledState);
  delay(100);
}

void setup() {
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
  filename[6] = value + 48; //add 48 to make it ASCII
  EEPROM.write(EEPROM_ADR, ++value);

  toggle();

  buffer.init(SEND_BUFFER_SIZE);

  toggle();

  SD.begin(8);

  toggle();

  file = SD.open(filename, FILE_WRITE);

  toggle();

  //prepare Timer
  Timer1.initialize(20); // 0.02us period = 50kHz
  Timer1.attachInterrupt(measure);

  digitalWrite(A5, LOW);
}

void loop()
{
  if(cnt >= MAX_COUNT ) {
    file.close();
    digitalWrite(A5, HIGH);
  }
  else if(buffer.getSize() > 0) {
    byte data = buffer.get();
    file.write(data);
    cnt++;
  }
}


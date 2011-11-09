
#include <SD.h>
#include <TimerOne.h>

// #define MEGA_SOFT_SPI 1

#define READ_CHAN1 PIND & 0x0F //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10

#define SEND_BUFFER_SIZE 1000
#define MAX_COUNT 10000000  //4,294,967,295

unsigned long cnt = 0;

File file;
ByteBuffer buffer;

void measure() {
  byte data = READ_CHAN1;
  byte port = READ_CHAN2;
  buffer.put(data);
  buffer.put(port);
}

void setup()
{
  DDRD = 0; //init pins as INPUT
  pinMode(9, INPUT);
  pinMode(10, INPUT);

  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);
  pinMode(A5, OUTPUT);
  
  digitalWrite(A5, HIGH);
  delay(100);

  buffer.init(SEND_BUFFER_SIZE);

  SD.begin(8);
  
  digitalWrite(A5, LOW);
  delay(100);

  file = SD.open("ex4.txt", FILE_WRITE);

  digitalWrite(A5, HIGH);
  delay(100);
  digitalWrite(A5, LOW);
  //prepare Timer
  Timer1.initialize(20); // 0.02us period = 50kHz
  Timer1.attachInterrupt(measure);
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


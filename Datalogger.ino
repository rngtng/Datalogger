
#include <SD.h>
#include <TimerOne.h>
#include <ByteBuffer.h>
// #include <EEPROM.h>

// #define MEGA_SOFT_SPI 1

#define READ_CHAN1 PIND & 0x0F //pins 0 - 7
#define READ_CHAN2 PINB & 0x06 //pins 9, 10
#define READ_CHAN3 PINC & 0x03 //pins 9, 10

#define SEND_BUFFER_SIZE 10

#define MAX_COUNT 200000 //number of Bytes to write MAX: 2M

#define EEPROM_ADR 0

long cnt = 0;
long input = 0;

File outFile;
ByteBuffer buffer;

char filename[] = "turtleX.log";

int ledState = LOW;

void toggle() {
  digitalWrite(A5, HIGH);
  delay(100);
  digitalWrite(A5, LOW);
  delay(100);
}

void measure() {
  buffer[pointer++] = READ_CHAN1;
  buffer[pointer++] = READ_CHAN3;
  
  input += 2;
  //buffer.put(data);
  //buffer.put(port);
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

  //int value = EEPROM.read(EEPROM_ADR);
  filename[6] = '0'; //value + 48; //add 48 to make it ASCII
  //EEPROM.write(EEPROM_ADR, ++value);

  toggle();

  buffer.init(SEND_BUFFER_SIZE);

  toggle();

  SD.begin(8);

  toggle();

  if(SD.exists(filename)) {
    SD.remove(filename);
  }
  outFile = SD.open(filename, O_WRITE | O_CREAT);

  toggle();

  //prepare Timer
  //Timer1.initialize(20); // microseconds -> 0.02ms period -> 50kHz
  Timer1.initialize(50); // microseconds -> 0.02ms period -> 50kHz
  Timer1.attachInterrupt(measure);
}

#define WRITE_FLUSH 5

void loop()
{
  if(cnt >= MAX_COUNT ) {
    digitalWrite(A5, HIGH);
    outFile.close();
  }
  else if(input >= WRITE_FLUSH) {
    byte array[WRITE_FLUSH];
    for(int k = 0; k < WRITE_FLUSH; k++) {
      //array[k] = buffer.get();
      input --;
    }
    outFile.write(array, WRITE_FLUSH);
    //outFile.write(1);
    cnt += WRITE_FLUSH;
  }
  /*
  else if(buffer.getSize() >= 1) {
    byte data = buffer.get();
    outFile.write(data);
    cnt += 1;
  }*/
}


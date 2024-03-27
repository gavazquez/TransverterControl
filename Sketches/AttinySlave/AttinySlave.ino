//This code has to be uploaded to the attiny13 so it can be controlled by an arduino. 
//It only tells the arduino in what position is the selector
//Uses attiny13 microcore board. Upload it using the arduino ISP programmer and connect MISO,MOSI,CLK,RESET accordingly

//Override default TX pin that is on MOSI (LE) position of the ADF and use the pin that is on DATA position
#define UART_TX_PIN B,1

#include <EEPROM.h>
#include <avr/io.h>

byte val1=255;
byte val2=255;
byte lastVal1=255;
byte lastVal2=255;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() 
{  
  DDRB &= ~(1 << PB3);
  DDRB &= ~(1 << PB4);
  //Activate internal pull-up resistors
  PORTB |= (1 << PB3);
  PORTB |= (1 << PB4);
  
  // Check if there exist any OSCCAL value in EEPROM addr. 0
  // If not, run the oscillator tuner sketch first
  uint8_t cal = EEPROM.read(0);
  if (cal < 0x80)
    OSCCAL = cal;

  Serial.begin();
}

void loop() 
{  
  byte pinBValue = PINB; 
  byte read1 = pinBValue & B00001000;		//isolate value of PB3
  byte read2 = pinBValue & B00010000;		//isolate value of PB4

  read1 = (read1 > 0) ? 1 : 0;
  read2 = (read2 > 0) ? 1 : 0;

  if (read1 != lastVal1 || read2 != lastVal2)
    lastDebounceTime = millis();
    
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (read1 != val1 || read2 != val2) {
      val1 = read1;
      val2 = read2;

      printValue();
    }
  }

  lastVal1=read1;
  lastVal2=read2;
}

//Send the selector position trough serial. It will be serialized as a CHAR!
void printValue(){
    int combined = (val1 << 1) | val2;
    if (combined == 3)
      Serial.print(0, DEC);
    else if (combined == 0) 
      Serial.print(3, DEC);
    else      
      Serial.print(combined, DEC);
}

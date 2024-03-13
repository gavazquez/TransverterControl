//This code has to be uploaded to the attiny13 so you can specify the frequencies based on the selector. 
//You don't need an arduino to control it.
//Uses attiny13 microcore board. Upload it using the arduino ISP programmer and connect MISO,MOSI,CLK,RESET accordingly

#include <EEPROM.h>
#include <avr/io.h>

byte val1=255;
byte val2=255;
byte lastVal1=255;
byte lastVal2=255;

unsigned long lastDebounceTime = 0;
byte debounceDelay = 50;

void setup() 
{ 
  //Input
  DDRB &= ~(1 << PB3);
  DDRB &= ~(1 << PB4);
  //Activate internal pull-up resistors
  PORTB |= (1 << PB3);
  PORTB |= (1 << PB4);

  //Output
  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
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

      int selector = (val1 << 1) | val2;
      if (selector == 3) selector = 0;
      else if (selector == 0)  selector = 3;
      switch(selector) {
        case 0: //200
          WriteRegister(0x580005);
          WriteRegister(0xCC803C);
          WriteRegister(0x4B3);
          WriteRegister(0x19004E42);
          WriteRegister(0x8008011);
          WriteRegister(0x500000);
        break;
        case 1: //200
          WriteRegister(0x580005);
          WriteRegister(0xCC803C);
          WriteRegister(0x4B3);
          WriteRegister(0x19004E42);
          WriteRegister(0x8008011);
          WriteRegister(0x500000);
        break;
        case 2: //166.25 (33.750)
          WriteRegister(0x580005);
          WriteRegister(0xCC803C);
          WriteRegister(0x4B3);
          WriteRegister(0x19004E42);
          WriteRegister(0x8008011);
          WriteRegister(0x428000);
        break;
        case 3: //150 (50)
          WriteRegister(0x580005);
          WriteRegister(0xCC803C);
          WriteRegister(0x4B3);
          WriteRegister(0x19004E42);
          WriteRegister(0x8008011);
          WriteRegister(0x3C0000);
        break;
      }
    }
  }

  lastVal1=read1;
  lastVal2=read2;
}

void WriteRegister(long regData){
  PORTB &= ~(1 << PB2);
	for(byte i=0; i<32; i++)
	{
		if(((regData<<i)&0x80000000)==0x80000000)
		  PORTB |= (1 << PB1);
		else
		  PORTB &= ~(1 << PB1);	

    PORTB |= (1 << PB2);
    PORTB &= ~(1 << PB2);
	}
  PORTB |= (1 << PB0);
  PORTB &= ~(1 << PB0);
}
#include <SimpleAdf.h>
#include <SPI.h>
#include <SoftwareSerial.h>

unsigned int clk = 13; //SCK (pin 7 attiny (or PB2))
unsigned int data = 12; //MISO (pin 6 attiny (or PB1))
unsigned int le = 11; //MOSI (pin 5 attiny (or PB0))
pinConfig pins = {clk, data, le};

float lastSetFreq = 0;

char transValChar[] = "0";
byte transVal = 0;
byte lastTransVal = 255;

adf4351Config listeningConfig = {true, 12.3, 40, 1, false, true, fourDiv5, fundamental, false, 1, 
{lowNoise, digitalLockDetect, false, twoDot5, intN, tenNs, positive, false, false, false},
{high, int3ns, true, true, fastLock, 150},
{false, true, auxDivided, false, minus4, true, minus1, 40},
{digitalLock}
};

adf4351Config txConfig = {true, 12.3, 40, 1, false, true, fourDiv5, fundamental, false, 1, 
{lowNoise, digitalLockDetect, false, twoDot5, fracN, tenNs, positive, false, false, false},
{high, frac6ns, false, false, fastLock, 150},
{false, false, auxDivided, false, minus4, true, minus1, 40},
{digitalLock}
};

Adf4351 rxAdf(listeningConfig, pins);
Adf4351 txAdf(txConfig, pins);

SoftwareSerial transvSerial(data, data);

void setup() { 
  transvSerial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(le, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(clk, LOW);
	digitalWrite(data, LOW);
	digitalWrite(le, LOW);
}

void loop() {
  if (transvSerial.readBytesUntil('\0', transValChar, sizeof(char)) > 0) 
    transVal = atoi(transValChar);

  if (transVal != lastTransVal)
    SetFrequency(transVal);

  lastTransVal = transVal;
}

void SetFrequency(byte transverterVal) {  
    pinMode(data, OUTPUT);
	  digitalWrite(data, LOW);
    switch (transverterVal) {
      case 0:
        txAdf.SetFreq(200);
        digitalWrite(LED_BUILTIN, LOW);
      break;
      case 1:
        txAdf.SetFreq(166.25);
        digitalWrite(LED_BUILTIN, HIGH);
      break;
      case 2:
        txAdf.SetFreq(159); //41
        digitalWrite(LED_BUILTIN, HIGH);
      break;
      case 3:
        txAdf.SetFreq(141.075); //58.925
        digitalWrite(LED_BUILTIN, HIGH);
      break;
    }
    pinMode(data, INPUT);
}
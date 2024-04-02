//This will ONLY work if the attiny has the sketch AttinySlave inside!
//This sketch transmit a tx tone of a given duration in a range of frequencies.
//This is very useful to discover the TX frequency on satcom while using a SDR to see if the frequency appears there

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

adf4351Config adfConfig = {true, 20, 40, 1, false, true, eightDiv9, fundamental, false, 1, 
{lowNoise, digitalLockDetect, false, twoDot5, fracN, tenNs, positive, false, false, false},
{low, frac6ns, false, false, off, 150},
{false, false, auxDivided, false, minus4, true, plus5, 200},
{digitalLock}
};

Adf4351 adf(adfConfig, pins);
SoftwareSerial transvSerial(data, data);

void setup() { 
  transvSerial.begin(115200);
  Serial.begin(115200);

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
        adf.SetFreq(200);
        digitalWrite(LED_BUILTIN, LOW);
      break;
      default:
        ScanBeacon();
        digitalWrite(LED_BUILTIN, HIGH);
      break;
    }
    pinMode(data, INPUT);
}

void ScanBeacon() {  

  //Put here the base frequency that you will configure on your radio. Once that's set DO NOT configure any offset in the TX of the radio.
  float baseFreq = 255.35;

  float from = 290; //Frequency to start  
  float to = 301; //Frequency to finish  
  short txDurationMs = 250; //Duration of the PTT on a given frequency in milliseconds
  float step = 0.005; //Frequency step

  long start = millis();
  Serial.println("START");
  PrintFreq(baseFreq, (from - baseFreq), 200 - (from - baseFreq));
  adf.SetFreq(200 - (from - baseFreq));

  for (float i = from; i < to; i+=step) {
      float freqOffset= i - baseFreq;
      float adfFreq = 200 - freqOffset;
      
      PrintFreq(baseFreq, freqOffset, adfFreq);
      adf.SetFreq(adfFreq);
      delay(txDurationMs);
  }

  PrintFreq(baseFreq, (to - baseFreq), 200 - (from - baseFreq));
  adf.SetFreq(200 - (to - baseFreq));
  
  Serial.print("FINISHED in: ");
  Serial.print((millis()-start)/1000.0);
  Serial.print(" Seconds");
}

void PrintFreq(float baseFreq, float freqOffset, float adfFreq) {
      Serial.print(baseFreq+freqOffset, 3);
      Serial.print("--");
      Serial.print(freqOffset);
      Serial.print("-(");
      Serial.print(adfFreq);
      Serial.println(")");
}
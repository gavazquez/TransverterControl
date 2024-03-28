#include "Arduino.h"
#include "SimpleAdf.h"
#include <math.h>

Adf4351::Adf4351(adf4351Config &adfConf, pinConfig &pinConf) {
	_adfConf = adfConf;
	_pinConf = pinConf;
}

//Call this method if you need to update the configuration so everything is recalculated next time you set a frequency
void Adf4351::UpdateConf(adf4351Config &adfConf) {
	_adfConf = adfConf;
}

//Pfd Freq must always be max but below 32 for fractional freq or 90 for integer freq
byte Adf4351::getPfdFreq() {
  return _adfConf.refFreqMhz * (((1.0+(_adfConf.refDoubler))/(_adfConf.rCounter*(1.0+_adfConf.referenceDivider))));
}

//The fractional modulus is the ratio of the PFD freq to the channel spacing
//This value depends highly on the channel spacing you are requesting and the reference frequency
int Adf4351::getMod(int pfdFreq, int outputDivider) {
  return (int)round((1000.0 * (pfdFreq/_adfConf.channelSpacing)));
}

//The Output generator operates between 2200 and 2400Mhz so if your requestedFreq is outside that range
//We must divide it. Min freq is 34.375. Max is 4400
byte getOutputDivider(float desiredFreqMhz) {	  
  if (desiredFreqMhz >= 2200) return 1;
  if (desiredFreqMhz >= 1100 && desiredFreqMhz < 2200) return 2;
  if (desiredFreqMhz >= 550 && desiredFreqMhz < 1100)	return 4;
  if (desiredFreqMhz >= 275 && desiredFreqMhz < 550) return 8;
  if (desiredFreqMhz >= 137.5 && desiredFreqMhz < 275) return 16;
  if (desiredFreqMhz >= 68.75 && desiredFreqMhz < 137.5) return 32;
  return 64;
}

byte getOutputDividerForRegister(int outputDivider) {	  
	switch(outputDivider)
	{
		case 1: return 0;
		case 2: return 1;
		case 4: return 2;
		case 8: return 3;
		case 16: return 4;	
		case 32: return 5;
		case 64: return 6;
	}											
}

//Calculate greatest common divisor
int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

//Gets the FRAC value for the adf
int getFrac(float numeratorFracDivision, int integerDivisionFactor, int mod) {
  return (int)round((numeratorFracDivision-integerDivisionFactor)*mod);
}

//The numerator of the fractional division or also called "N"
//The INT is basically this number except it's decimals
float Adf4351::getNumeratorFractionalDivision(float desiredFreqMhz, byte outputDivider, byte pfdFreq) {
  if (_adfConf.feedbackSignal == fundamental)  
	return (float)(desiredFreqMhz*outputDivider)/pfdFreq;
  return (float)desiredFreqMhz/pfdFreq;
}

//Calculates the values of the registers and sends them to the ADF
void Adf4351::SetFreq(float desiredFreqMhz) {
	byte outputDivider = getOutputDivider(desiredFreqMhz);
	byte pfdFreq = getPfdFreq();
	int mod = getMod(pfdFreq, outputDivider);
	float numerator = getNumeratorFractionalDivision(desiredFreqMhz, outputDivider, pfdFreq);
	int integer = (int)numerator;
	int frac = getFrac(numerator, integer, mod);

	if (_adfConf.applyGcd) {
		int gcdNum = gcd(mod, frac);
		mod = mod/gcdNum;
		frac = frac/gcdNum;
	}

	if (mod == 1) mod = 2;

	//Cleanup stored registers
	for(int i = 5; i > -1; i--)
		registers[i] = 0;

	//Check adf datasheet to check the bit positions
	registers[0] = ((0 & 0x1L) << 31) | ((integer & 0xFFFFL) << 15) | ((frac & 0xFFFL) << 3) | ((0 & 0x7L) << 0);
	registers[1] = ((0 & 0x7L) << 29) | ((_adfConf.phaseAdjust & 0x1L) << 28) | ((_adfConf.prescaler & 0x1L) << 27)
			| ((_adfConf.phaseValue & 0xFFFL) << 15)| ((mod & 0xFFFL) << 3)| ((1 & 0x7L) << 0);						
	registers[2] = ((0 & 0x1L) << 31) | ((_adfConf.R2.noiseSpurMode & 0x3L) << 29) | ((_adfConf.R2.muxOut & 0x7L) << 26)
			| ((_adfConf.refDoubler & 0x1L) << 25)| ((_adfConf.referenceDivider & 0x1L) << 24)
			| ((_adfConf.rCounter & 0x3FFL) << 14) | ((_adfConf.R2.doubleBuffer & 0x1L) << 13) | ((_adfConf.R2.chargePumpCurrent & 0xFL) << 9)
			| ((_adfConf.R2.ldf & 0x1L) << 8)| ((_adfConf.R2.ldp & 0x1L) << 7)| ((_adfConf.R2.pdPolarity & 0x1L) << 6)
			| ((_adfConf.R2.powerdown & 0x1L) << 5)| ((_adfConf.R2.chargePump3state & 0x1L) << 4)
			| ((_adfConf.R2.counterReset & 0x1L) << 3)| ((2 & 0x7L) << 0);
	registers[3] = ((0 & 0xFFL) << 24) | ((_adfConf.R3.bandClock & 0x1L) << 23) | ((_adfConf.R3.abp & 0x1L) << 22)
			| ((_adfConf.R3.chargeCancellation & 0x1L) << 21)| ((0 & 0x3L) << 19)| ((_adfConf.R3.cycleSlipReduction & 0x1L) << 18)
			| ((0 & 0x1L) << 17)| ((_adfConf.R3.clockDivMode & 0x3L) << 15)| ((_adfConf.R3.clockDividerValue & 0xFFFL) << 3)
			| ((3 & 0x7L) << 0);
			
	registers[4] = ((0 & 0xFFL) << 24) | ((_adfConf.feedbackSignal & 0x1L) << 23) | ((getOutputDividerForRegister(outputDivider) & 0x7L) << 20)
			| ((_adfConf.R4.bandSelectClockDivider & 0xFFL) << 12)| ((_adfConf.R4.vcoPowerDown & 0x1L) << 11)
			| ((_adfConf.R4.muteTillLockDetect & 0x1L) << 10)| ((_adfConf.R4.auxOutputSelect & 0x1L) << 9)
			| ((_adfConf.R4.auxOutputEnable & 0x1L) << 8)| ((_adfConf.R4.auxOutputPower & 0x3L) << 6)
			| ((_adfConf.R4.rfOutputEnable & 0x1L) << 5)| ((_adfConf.R4.outputPower & 0x3L) << 3)
			| ((4 & 0x7L) << 0);
			
	registers[5] = ((0 & 0xFFL) << 24) | ((_adfConf.R5.lockDetect & 0x3L) << 22) | ((0 & 0x1L) << 21)
			| ((3 & 0x3L) << 19)| ((0 & 0xFFFFL) << 3)
			| ((5 & 0x7L) << 0);

	//Write them in inverse order as specified in datasheet
	for(int i = 5; i > -1; i--)
		WriteRegister(registers[i]);
}

//Writes a single register to the ADF.
//According to the datasheet 25ns are needed between calls.
//If using arduinos faster than 16mhz you will need to wait between clk high-low and le high-low
void Adf4351::WriteRegister(long regData) {
	digitalWrite(_pinConf.lePin, LOW);
	for(byte i=0; i<32; i++)
	{
		if(((regData<<i)&0x80000000)==0x80000000)
			digitalWrite(_pinConf.dataPin,1);
		else
			digitalWrite(_pinConf.dataPin,0);
		
		digitalWrite(_pinConf.clkPin, HIGH);
		digitalWrite(_pinConf.clkPin, LOW);
	}
	
	digitalWrite(_pinConf.lePin, HIGH);
	digitalWrite(_pinConf.lePin, LOW);
}

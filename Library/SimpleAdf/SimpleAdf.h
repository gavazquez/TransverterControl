#ifndef SimpleAdf_h
#define SimpleAdf_h
	
#include "Arduino.h"
#include "SimpleAdfR2.h"
#include "SimpleAdfR3.h"
#include "SimpleAdfR4.h"
#include "SimpleAdfR5.h"
		
//This scales the frequency of the VCO that goes into the PFD
// If frequency > 3600Mhz you must use 8/9
enum Prescaler {
  fourDiv5 = 0,
  eightDiv9 = 1
};

//This scales the frequency of the VCO that goes into the PFD
// If frequency > 3600Mhz you must use 8/9
enum FeedbackSignal {
  divided = 0,
  fundamental = 1
};

struct adf4351Config {
  bool applyGcd;                    //Apply greatest common divider for the calculation
  float channelSpacing;             //In khz
  int refFreqMhz;                   //Reference freq on the circuit in Mhz
  int rCounter;                     //Divides the refFreqMhz. Keep it low so the PFD is max but always below 32 or 90. Min 1, Max 1023
  bool refDoubler;                  //Doubles the freq of the refFreqMhz.
  bool referenceDivider;            //Divides the refFreqMhz by 2
  Prescaler prescaler;
  FeedbackSignal feedbackSignal;
  bool phaseAdjust;                 //Enable phase adjust
  int phaseValue;                   //Phase value if phase adjust is on. Recommended val is 1. Min 0, Max 4095
  adf4351R2 R2;
  adf4351R3 R3;
  adf4351R4 R4;
  adf4351R5 R5;
};

struct pinConfig{
	int clkPin;
	int dataPin;
	int lePin;
};
		
class Adf4351
{	
	public:
		Adf4351(adf4351Config &adfConf, pinConfig &pinConf);
		
		long registers[6];
		//Sets the desired frequency
		void SetFreq(float desiredFreqMhz);		
		//Call this method if you need to update the configuration so everything is recalculated next time you set a frequency
		void UpdateConf(adf4351Config &adfConf);
	private:
		adf4351Config _adfConf;
		pinConfig _pinConf;
		byte getPfdFreq();
		int getMod(int pfdFreq, int outputDivider);
		float getNumeratorFractionalDivision(float desiredFreqMhz, byte outputDivider, byte pfdFreq);
		void WriteRegister(long regData);
};

#endif
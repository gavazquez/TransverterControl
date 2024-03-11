#ifndef SimpleAdfR4_h
#define SimpleAdfR4_h

//Selects if the aux output is used for the RF dividers or the fundamental VCO freq.
enum AuxOutputSelect
{
  auxDivided = 0,
  auxFundamental = 1
};

//Output power for aux and RF
enum OutputPower
{
  minus4 = 0,
  minus1 = 1,
  plus2 = 2,
  plus5 = 3
};

struct adf4351R4 {
  bool vcoPowerDown;                //If set to true it powers down the vco. Leave it as False
  bool muteTillLockDetect;        	//If set to true it mutes the RF output until you get a lock
  AuxOutputSelect auxOutputSelect;
  bool auxOutputEnable;             //Enable or disable aux output
  OutputPower auxOutputPower;       //Aux output power
  bool rfOutputEnable;              //Enable or disable rf output. Leave it true
  OutputPower outputPower;          //Main output power
  int bandSelectClockDivider;		//Band select clock divider
};

#endif
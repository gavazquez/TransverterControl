#ifndef SimpleAdfR2_h
#define SimpleAdfR2_h

//Use low noise or low spur frequencies
enum NoiseSpurMode {
  lowNoise = 0,
  reserved0 = 1,
  reserved1 = 2,
  LowSpur = 3
};

//Multiplexer output.
enum Muxout {
  threeStateOutput = 0,
  dvdd = 1,
  dgnd = 2,
  rDivider = 3,
  nDivider = 4,
  analogLockDetect = 5,
  digitalLockDetect = 6,
  reserved = 7
};

//Charge pump current settings
enum ChargePumpCurrent {
  zeroDot31 = 0,
  zeroDot63 = 1,
  zeroDot94 = 2,
  oneDot25 = 3,
  oneDot56 = 4,
  oneDot88 = 5,
  twoDot19 = 6,
  twoDot5 = 7,
  twoDot81 = 8,
  threeDot13 = 9,
  threeDot44 = 10,
  threeDot75 = 11,
  fourDot06 = 12,
  fourDot38 = 13,
  fourDot69 = 14,
  five = 15
};

//Lock detect function. Use 0 for fractional requested frequencies or 1 for integer frequencies.
//Basically the number of PFD cycles monitored by the lock detect to confirm if a lock is set. 
enum Ldf {
  fracN = 0,  //40 cycles
  intN = 1    //5 cycles
};

//Lock detect precision. Comparision window for Ldf. Faster lock times = sixNs. More precision = tenNs
enum Ldp {
  tenNs = 0,
  sixNs = 1
};

//Phase detector polarity
enum PdPolarity {
  negative = 0,
  positive = 1 //Most common
};

struct adf4351R2 {
  NoiseSpurMode noiseSpurMode;
  Muxout muxOut;
  bool doubleBuffer;                //IF set to true you will need to write R0 after modifying the output divider. Leave it at false
  ChargePumpCurrent chargePumpCurrent;
  Ldf ldf;
  Ldp ldp;
  PdPolarity pdPolarity;
  bool powerdown;                   //If set to true it turns off the system
  bool chargePump3state;            //Puts the charge pump into three-state mode. Leave it at false
  bool counterReset;                //Reset bit for N and R ounter. Leave it at false
};
	
#endif
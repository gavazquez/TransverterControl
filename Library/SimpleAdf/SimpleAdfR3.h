#ifndef SimpleAdfR3_h
#define SimpleAdfR3_h

//High = faster selection of band. Use 0 for pfd < 125khz. If 0, band select clock divider <= 254
enum BandClockMode {
  low = 0,
  high = 1 //Most common
};

//Use 0 for fractional requested frequencies or 1 for integer requested frequencies
enum AntibackslashPushWidth
{
  frac6ns = 0,
  int3ns = 1
};

//Use 0 for fractional requested frequencies or 1 for integer requested frequencies
enum ClockDividerMode
{
  off = 0,
  fastLock = 1,                   //General use
  resync = 2,                     //Use this value if phase and freq is important such as digital beamforming
  reserved_ = 3,
};

struct adf4351R3 {
  BandClockMode bandClock;
  AntibackslashPushWidth abp;
  bool chargeCancellation;        //Reduces PFD spurs in integer mode. In fractional set to false
  bool cycleSlipReduction;        //You need to have referenceDivider and chargePumpCurrent at minimum to make it work if set to true
  ClockDividerMode clockDivMode;
  int clockDividerValue;          //If using fastlock, this value determines the duration of wide bandwidth mode. Min 0, Max 4095
};

#endif
#ifndef SimpleAdfR5_h
#define SimpleAdfR5_h

//Lock detect pin operation
enum LockDetectPinOp
{
  low_ = 0,
  digitalLock = 1,                  //Most useful
  low__ = 2,
  high_ = 3
};

struct adf4351R5 {
  LockDetectPinOp lockDetect;
};

#endif
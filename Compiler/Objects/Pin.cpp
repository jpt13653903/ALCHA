#include "Pin.h"
//------------------------------------------------------------------------------

PINS Pins;
//------------------------------------------------------------------------------

PIN::PIN(const char* Name): OBJECT(otPin, Name){
 ArrayDepth = 0;
 Index      = 0;

 Standard  = "CMOS";
 Voltage   = 3.3;
 Current   = 0.0;

 Direction = Bidirectional;

 MinDelay  = 0.0;
 MaxDelay  = 0.0;

 Frequency = 0.0;
 Phase     = 0.0;
 Jitter    = 0.0;
}
//------------------------------------------------------------------------------

PIN::~PIN(){
 if(Index) delete[] Index;
}
//------------------------------------------------------------------------------

int PIN::Compare(TREE_NODE* Right){
 PIN* right = (PIN*)Right;

 int Result = Name.Compare(right->Name);
 if(!Result) return Result;

 int j;
 for(j = 0; j < ArrayDepth && j < right->ArrayDepth; j++){
  if(Index[j] < right->Index[j]) return -1;
  if(Index[j] > right->Index[j]) return  1;
 }
 if(ArrayDepth < right->ArrayDepth) return -1;
 if(ArrayDepth > right->ArrayDepth) return  1;
 return 0;
}
//------------------------------------------------------------------------------

PIN* PINS::FindName(const char* Name){
 OBJECT Key(otPin, Name);
 return (PIN*)Find(&Key);
}
//------------------------------------------------------------------------------

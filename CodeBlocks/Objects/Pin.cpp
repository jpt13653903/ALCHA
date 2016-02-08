#include "Pin.h"
//------------------------------------------------------------------------------

PINS Pins;
//------------------------------------------------------------------------------

PIN::PIN(const byte* Name): OBJECT(otPin, Name){
 ArrayDepth = 0;
 Indices    = 0;

 Standard  = "LVCMOS";
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
 if(Indices) delete[] Indices;
}
//------------------------------------------------------------------------------

int PIN::Compare(TREE_NODE* Right){
 PIN* right = (PIN*)Right;

 int Result = Name.Compare(right->Name);
 if(Result) return Result;

 int j;
 for(j = 0; j < ArrayDepth && j < right->ArrayDepth; j++){
  if(Indices[j] < right->Indices[j]) return -1;
  if(Indices[j] > right->Indices[j]) return  1;
 }
 if(ArrayDepth < right->ArrayDepth) return -1;
 if(ArrayDepth > right->ArrayDepth) return  1;
 return 0;
}
//------------------------------------------------------------------------------

PIN* PINS::FindName(const byte* Name, int ArrayDepth, int* Indices){
 PIN Key(Name);
 Key.ArrayDepth = ArrayDepth;
 Key.Indices    = Indices;

 PIN* Result = (PIN*)Find(&Key);

 Key.Indices = 0; // Prevent premature deletion

 return Result;
}
//------------------------------------------------------------------------------

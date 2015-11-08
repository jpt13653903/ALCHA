#include "Pin.h"
//------------------------------------------------------------------------------

PIN::PIN(): OBJECT(otPin){
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

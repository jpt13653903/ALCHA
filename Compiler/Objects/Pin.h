//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of ALCHA
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

#ifndef Pin_h
#define Pin_h
//------------------------------------------------------------------------------

#include "Object.h"
//------------------------------------------------------------------------------

struct PIN: public OBJECT{
 // Physical attributes
 STRING Standard;  // The logic standard: CMOS, TTL, HSUL, etc.
 STRING Number;    // Physical pin number
 STRING Pair;      // Physical pin number of negative of the differential pair
 float  Voltage;   // Nominal logic standard voltage [V]
 float  Current;   // Nominal pin drive strength [A] (0 => use bank default)

 // Logical attributes
 enum{Output, Input, Bidirectional} Direction;

 // Timing-relates attributes
 double MinDelay;  // Physical minimum external trace delay [s] (default = 0)
 double MaxDelay;  // Physical maximum external trace delay [s] (default = 0)

 // Clock-related attributes
 double Frequency; // Input frequency of a clock pin [Hz] (0 if not clock pin)
 double Phase;     // Clock phase [degrees]
 double Jitter;    // Clock peak-to-peak jitter [s]

 PIN();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

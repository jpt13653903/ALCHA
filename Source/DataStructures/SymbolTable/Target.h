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

#ifndef Target_h
#define Target_h
//------------------------------------------------------------------------------

#include "Object.h"
#include "MyString.h"
//------------------------------------------------------------------------------

struct TARGET: public OBJECT{
  enum TYPE{Project, Simulation} Type;

  STRING Vendor;
  STRING Series;
  STRING Device;
  STRING Board;

  enum LANGUAGE{VHDL, Verilog} Language;

  double Time; // Simulation time

  TARGET();
 ~TARGET();

  void Display();
};
//------------------------------------------------------------------------------

extern TARGET Target; // There is only one target object
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
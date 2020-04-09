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

#include "Pin.h"
//------------------------------------------------------------------------------

using namespace NETLIST;
//------------------------------------------------------------------------------

PIN::PIN(int Line, const std::string& Filename, const char* Name) : SYNTHESISABLE(Line, Filename, Name, TYPE::Pin){
  Driver  = 0;
  Enabled = 0;
}
//------------------------------------------------------------------------------

PIN::~PIN(){
  if(Driver ) delete Driver;
  if(Enabled) delete Enabled;
}
//------------------------------------------------------------------------------

void PIN::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Pin: ");

  Indent++;
  DisplayParameters(Indent);

  Debug.Indent(Indent);
  Debug.Print("Driver     = ");
  if(Driver) Driver->Display();
  else       Debug.Print("{open}");
  Debug.Print("\n");

  Debug.Indent(Indent);
  Debug.Print("Enabled    = ");
  if(Enabled) Enabled->Display();
  else        Debug.Print("{open}");
  Debug.Print("\n");

  DisplayAttributes(Indent);
}
//------------------------------------------------------------------------------

void PIN::Validate(){
  assert(Type == TYPE::Pin);

  SYNTHESISABLE::Validate();

  if(Driver ) Driver ->Validate();
  if(Enabled) Enabled->Validate();
}
//------------------------------------------------------------------------------


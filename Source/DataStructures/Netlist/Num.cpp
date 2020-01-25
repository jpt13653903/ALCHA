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

#include "Num.h"
//------------------------------------------------------------------------------

using namespace NETLIST;
//------------------------------------------------------------------------------

NUM::NUM(int Line, const std::string& Filename, const char* Name) : BASE(Line, Filename, Name, TYPE::Number){
  Value = 0;
}
//------------------------------------------------------------------------------

NUM::~NUM(){
}
//------------------------------------------------------------------------------

void NUM::Display(){
  Debug.print("  Num: %s\n", Name.c_str());
  Debug.print("    Value = ");
  Debug.print(Value.Display());
  Debug.print("\n");
}
//------------------------------------------------------------------------------


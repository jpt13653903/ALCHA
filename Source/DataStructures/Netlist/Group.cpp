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

#include "Group.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

NETLIST::GROUP::GROUP(const char* Name): NAMESPACE(Name){
  Type = TYPE::Group;
}
//------------------------------------------------------------------------------

NETLIST::GROUP::~GROUP(){
}
//------------------------------------------------------------------------------

void NETLIST::GROUP::Display(){
  printf("\n  Group: ");

  if(Name.empty()) printf("{Anonymous}");
  else             DisplayLongName();
  printf("{\n");

  DisplayAttributes(4);
  printf("\n");

  for(auto s = Symbols.begin(); s != Symbols.end(); s++){
    if(s->second) s->second->Display();
    else          printf("    - %s: {null}\n", s->first.c_str());
    printf("\n");
  }
  printf("  }\n");
}
//------------------------------------------------------------------------------


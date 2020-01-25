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

#include "Synthesisable.h"
//------------------------------------------------------------------------------

using namespace NETLIST;
//------------------------------------------------------------------------------

SYNTHESISABLE::SYNTHESISABLE(int Line, const std::string& Filename, const char* Name, TYPE Type) : BASE(Line, Filename, Name, Type){
  Used      = false;
  Signed    = false;
  Width     = 1;
  FullScale = 2;
}
//------------------------------------------------------------------------------

SYNTHESISABLE::~SYNTHESISABLE(){
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::Display(){
  switch(Type){
    case TYPE::Pin: Debug.print("  Pin: "); break;
    case TYPE::Net: Debug.print("  Net: "); break;
    default: error ("Unknown synthesisable type"); break;
  }
  Debug.print("%s\n", Name.c_str());
  Debug.print("    Used       = %s\n", Used   ? "true" : "false");
  Debug.print("    Width      = %u\n", Width);
  Debug.print("    Full-scale = %s\n", FullScale.Display());
  Debug.print("    Signed     = %s\n", Signed ? "true" : "false");

  Debug.print("    Direction  = ");
  switch(Direction){
    case AST::DEFINITION::Inferred     : Debug.print("Inferred\n"     ); break;
    case AST::DEFINITION::Input        : Debug.print("Input\n"        ); break;
    case AST::DEFINITION::Output       : Debug.print("Output\n"       ); break;
    case AST::DEFINITION::Bidirectional: Debug.print("Bidirectional\n"); break;
    default                            : Debug.print("Invalid\n"      ); break;
  }
}
//------------------------------------------------------------------------------



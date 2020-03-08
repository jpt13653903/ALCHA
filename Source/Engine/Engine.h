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

/* Description

1. Processes the AST and produces the netlist tree (NETLIST::Global).
2. Processes the scripting statements so that the scripting variables become
   constants, thereby making the resulting netlist tree synthesisable.
------------------------------------------------------------------------------*/

#ifndef Engine_h
#define Engine_h
//------------------------------------------------------------------------------

#include <stack>
//------------------------------------------------------------------------------

#include "Parser.h"
#include "Utilities.h"
//------------------------------------------------------------------------------

class ENGINE{
  private: // Massages
    PARSER Parser;
  //----------------------------------------------------------------------------

  private: // Internal structures
    std::stack<AST::BASE*> AstStack; // Used for clean-up in the destructor
  //----------------------------------------------------------------------------

  public:
    ENGINE();
   ~ENGINE();

    bool Run(const char* Filename);
};
//------------------------------------------------------------------------------

extern ENGINE* Engine; // Created in ALCHA.cpp::main()
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


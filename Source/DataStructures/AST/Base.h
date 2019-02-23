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

#ifndef AST_Base_h
#define AST_Base_h
//------------------------------------------------------------------------------

#include <string>
//------------------------------------------------------------------------------

#include "General.h"
//------------------------------------------------------------------------------

namespace AST{
  struct BASE{ // The base type for AST nodes
    enum class TYPE{
      Base = 0,

      Fence, // Empty statement, but also "next-cycle" specifier in FSMs
      Import,
      Group,
      Alias,
      TargetDefinition,
      ClassDefinition,
      EnumDefinition,
      Definition, // pin, net, byte, char, num and
                  // class instance
      Parameter,  // Parameter definition, not function call
      Expression,
      Assignment,
      NamespacePush,
      IfStatement,
      ForLoop,
      LoopLoop,
      WhileLoop,
      Switch,
      Jump,
      RTL,
      FSM,
      HDL
    } Type;

    int         Line;
    std::string Filename;
    BASE*       Next; // Next sibling

             BASE(int Line, const char* Filename, TYPE Type);
    virtual ~BASE(); // Also deletes the rest of the linked list

            void DisplayInfo();
    virtual void Display() = 0;
  };
  //----------------------------------------------------------------------------

  extern BASE* Root; // The global AST root
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


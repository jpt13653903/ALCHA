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

#ifndef Objects_Base_h
#define Objects_Base_h
//------------------------------------------------------------------------------

#include <map>
#include <string>
//------------------------------------------------------------------------------

#include "General.h"
//------------------------------------------------------------------------------

namespace OBJECTS{
  struct BASE{ // Base class for the symbol table
    enum class TYPE{
      // Base types
      Pin, Net, // Goes via the "Synthesisable" class
      Number, Byte, Character,

      // Others
      Array, // An array of objects
      Namespace,
      ClassDefinition,
      ClassInstance,
      FunctionDefinition,
      FunctionInstance
    } Type;

    std::string                  Name;
    std::map<std::string, BASE*> Attributes;

             BASE(const char* Name, TYPE Type);
    virtual ~BASE();

    virtual void Display() = 0;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


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

#ifndef Netlist_Base_h
#define Netlist_Base_h
//------------------------------------------------------------------------------

#include <map>
#include <string>
//------------------------------------------------------------------------------

#include "Logger.h"
#include "General.h"
#include "AST/Assignment.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  class NAMESPACE;
  class EXPRESSION;

  class BASE{ // Base class for the symbol table
    protected:
      void DisplayAttributes(int Indent);

    public:
      // Indentation follows the inheritance tree
      enum class TYPE{
        Synthesisable,
          Pin,
          Net,
        Number,
        Byte,
        Character,

        Alias,
        Array, // An array of objects
        Namespace,
          Module,
          Group
      } Type;

      std::string                        Name;
      NAMESPACE*                         Namespace;
      std::map<std::string, EXPRESSION*> Attributes;

               BASE(const char* Name, TYPE Type);
      virtual ~BASE();

      // Generates a name based on the parent group(s), if it is not directly
      // part of the module.  This is the name used in the output HDL.
      std::string& HDL_Name();

      virtual void Display        () = 0;
              void DisplayLongName();

      // Access the attribute, but searches up to the root and
      // returns null when not found
      EXPRESSION* GetAttrib(const std::string& Key);
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


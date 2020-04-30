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

#ifndef AST_Assignment_h
#define AST_Assignment_h
//------------------------------------------------------------------------------

#include <list>
//------------------------------------------------------------------------------

#include "Expression.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  class BASE;
}
//------------------------------------------------------------------------------

namespace AST{
  class ASSIGNMENT: public BASE{
    public:
      // Left and Right operands
      EXPRESSION* Left;
      EXPRESSION* Right;

    protected:
      // Populates a list of existing expressions, except when the target is an
      // undefined attribute, in which case the attribute is created first.
      typedef std::list<NETLIST::BASE*> target_list;
      bool AddLHS_Object(NETLIST::BASE* Object, target_list& List);
      bool GetLHS(EXPRESSION* Node, target_list& List);

    protected:
      void DisplayAssignment(const char* Operator);

    public:
               ASSIGNMENT(int Line, const char* Filename, TYPE AssignmentType);
      virtual ~ASSIGNMENT();

      bool IsAssignment() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


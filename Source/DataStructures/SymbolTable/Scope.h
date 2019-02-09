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

#ifndef Scope_h
#define Scope_h
//------------------------------------------------------------------------------

#include <map>
//------------------------------------------------------------------------------

#include "Object.h"
//------------------------------------------------------------------------------

class SCOPE{
  private:
    typedef std::map<std::string, OBJECT*> SYMBOL_TREE;

    struct NODE{
      SYMBOL_TREE Symbols;   // In use when this is a scope
      OBJECT*     Namespace; // In use when this is a name-space (null otherwise)
      NODE*       Next;

      NODE();
     ~NODE();
    };
    NODE* Stack;

  public:
    SYMBOL_TREE* Current; // The current scope (do not change the pointer)

    void New ();                  // Creates new scope (statement blocks, etc.)
    void Push(OBJECT* Namespace); // Pushes name-space
    void Pop ();                  // Pops the top scope or name-space

    SCOPE();
   ~SCOPE();
};
//------------------------------------------------------------------------------

extern SCOPE Scope; // The global data structure
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


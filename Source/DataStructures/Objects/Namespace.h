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

#ifndef Objects_Namespace_h
#define Objects_Namespace_h
//------------------------------------------------------------------------------

#include <map>
#include <stack>
//------------------------------------------------------------------------------

#include "Base.h"
//------------------------------------------------------------------------------

namespace OBJECTS{
  class NAMESPACE{
    public:
      NAMESPACE*                   Parent;
      std::map<std::string, BASE*> Symbols;

      NAMESPACE(NAMESPACE* Parent = 0);
     ~NAMESPACE(); // Also cleans up the children
  };
  //----------------------------------------------------------------------------

  extern std::stack<NAMESPACE*> Stack;   // Used for "with" type statements
  extern NAMESPACE              Global;  // The root of the namespace tree
  extern NAMESPACE*             Current; // The current namespace
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


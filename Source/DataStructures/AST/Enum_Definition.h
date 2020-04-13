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

#ifndef AST_Enum_Definition_h
#define AST_Enum_Definition_h
//------------------------------------------------------------------------------

#include "Base.h"
//------------------------------------------------------------------------------

namespace AST{
  struct ENUM_DEFINITION: public BASE{
    struct VALUE{ // Link-list node for enumeration values
      std::string Identifier;

      VALUE* Next;

      VALUE();
      VALUE(const VALUE& Value);
     ~VALUE(); // Also deletes the rest of the list
    };

    std::string Identifier;
    VALUE*      Values;

    ENUM_DEFINITION(int Line, std::string& Filename);
    ENUM_DEFINITION(int Line, const char*  Filename);
   ~ENUM_DEFINITION();

    BASE* Copy() override;

    bool RunAST() override;
    bool GetVerilog(std::string& Body) override;

    void Display() override;

    void ValidateMembers() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


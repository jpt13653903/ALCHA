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

#ifndef AST_Import_h
#define AST_Import_h
//------------------------------------------------------------------------------

#include "Base.h"
//------------------------------------------------------------------------------

namespace AST{
  class IMPORT: public BASE{
    private:
      BASE* Ast; // Used to store in-line ASTs

    public:
      std::string File;
      std::string Namespace;

      IMPORT(int Line, std::string& Filename);
      IMPORT(int Line, const char*  Filename);
     ~IMPORT();

      // Returns a copy of this instance
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


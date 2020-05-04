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

#ifndef AST_Expression_Subtract_h
#define AST_Expression_Subtract_h
//------------------------------------------------------------------------------

#include "../Expression.h"
//------------------------------------------------------------------------------

namespace AST{
  class SUBTRACT: public EXPRESSION{
    private:
      // Result format, calculated in Evaluate()
      int    ResultWidth = 0;
      NUMBER ResultFullScale;
      bool   ResultSigned = false;

      // Helper function to subtract literals from objects, or vice versa
      EXPRESSION* SubtractLiteral(EXPRESSION* Object, EXPRESSION* Literal);

    public: // Public interface
      SUBTRACT(int Line, const std::string& Filename);
      SUBTRACT(int Line, const char*        Filename);
     ~SUBTRACT();

      BASE* Copy() override;

      bool GetVerilog(std::string& Body) override;
      EXPRESSION* Evaluate() override;

      int     GetWidth    () override;
      NUMBER& GetFullScale() override;
      bool    GetSigned   () override;

      bool HasCircularReference(NETLIST::BASE* Object) override;
      void PopulateUsed() override;

      EXPRESSION* RemoveTempNet(int Width, bool Signed) override;

      void Display() override;

      void ValidateMembers() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


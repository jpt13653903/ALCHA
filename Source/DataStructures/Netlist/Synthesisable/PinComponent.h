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

#ifndef Netlist_PinComponent_h
#define Netlist_PinComponent_h
//------------------------------------------------------------------------------

#include "../Base.h"
#include "Net.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  struct PIN;

  struct PIN_COMPONENT: public NET{
    PIN* Pin;

    PIN_COMPONENT(int Line, const std::string& Filename, const char* Name, PIN* Pin);
    virtual ~PIN_COMPONENT();

    bool RawAssign(AST::EXPRESSION* Expression) override;

    void PopulateUsed(bool SetUsed) override;

    BASE*            GetAttribute            (const std::string& Name) override;
    AST::EXPRESSION* GetAttribValue          (const std::string& Name) override;
    AST::EXPRESSION* GetBuiltInAttributeValue(const std::string& Name) override;

    int     Width    () override;
    NUMBER& FullScale() override;
    bool    Signed   () override;

    void Display(int Indent = 0) override;

    void Validate() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

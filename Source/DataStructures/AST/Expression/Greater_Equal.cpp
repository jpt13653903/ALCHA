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

#include "Greater_Equal.h"
#include "../Assignment.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

GREATER_EQUAL::GREATER_EQUAL(int Line, const string& Filename): GREATER_EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

GREATER_EQUAL::GREATER_EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Greater_Equal){
}
//------------------------------------------------------------------------------

GREATER_EQUAL::~GREATER_EQUAL(){
}
//------------------------------------------------------------------------------

BASE* GREATER_EQUAL::Copy(bool CopyNext){
  GREATER_EQUAL* Copy = new GREATER_EQUAL(Line, Filename.c_str());

  Copy->Name      = Name;
  Copy->Value     = Value;
  Copy->StrValue  = StrValue;
  Copy->ObjectRef = ObjectRef;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

void GREATER_EQUAL::Display(){
  DisplayStart();

  Debug.print(" >= ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

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

#include "Logical_AND.h"
#include "../Assignment.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LOGICAL_AND::LOGICAL_AND(int Line, const string& Filename): LOGICAL_AND(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LOGICAL_AND::LOGICAL_AND(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Logical_AND){
}
//------------------------------------------------------------------------------

LOGICAL_AND::~LOGICAL_AND(){
}
//------------------------------------------------------------------------------

BASE* LOGICAL_AND::Copy(bool CopyNext){
  LOGICAL_AND* Copy = new LOGICAL_AND(Line, Filename.c_str());

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

void LOGICAL_AND::Display(){
  DisplayStart();

  Debug.print(" && ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

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

#include "ArrayConcatenate.h"
#include "../Assignment.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ARRAYCONCATENATE::ARRAYCONCATENATE(int Line, const string& Filename): ARRAYCONCATENATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ARRAYCONCATENATE::ARRAYCONCATENATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::ArrayConcatenate){
}
//------------------------------------------------------------------------------

ARRAYCONCATENATE::~ARRAYCONCATENATE(){
}
//------------------------------------------------------------------------------

BASE* ARRAYCONCATENATE::Copy(bool CopyNext){
  ARRAYCONCATENATE* Copy = new ARRAYCONCATENATE(Line, Filename.c_str());

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

void ARRAYCONCATENATE::Display(){
  DisplayStart();

  Debug.print("{ArrayConcat}");

  DisplayEnd();
}
//------------------------------------------------------------------------------

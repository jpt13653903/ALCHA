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

#include "NOR_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NOR_REDUCE::NOR_REDUCE(int Line, const string& Filename): NOR_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NOR_REDUCE::NOR_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::NOR_Reduce){
}
//------------------------------------------------------------------------------

NOR_REDUCE::~NOR_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* NOR_REDUCE::Copy(bool CopyNext){
  NOR_REDUCE* Copy = new NOR_REDUCE(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool NOR_REDUCE::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* NOR_REDUCE::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* NOR_REDUCE::Simplify(){
  assert(Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify();

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void NOR_REDUCE::Display(){
  DisplayStart();

  Debug.print(" ~|");

  DisplayEnd();
}
//------------------------------------------------------------------------------

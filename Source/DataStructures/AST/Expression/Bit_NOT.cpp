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

#include "Bit_NOT.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_NOT::BIT_NOT(int Line, const string& Filename): BIT_NOT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_NOT::BIT_NOT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_NOT){
}
//------------------------------------------------------------------------------

BIT_NOT::~BIT_NOT(){
}
//------------------------------------------------------------------------------

BASE* BIT_NOT::Copy(bool CopyNext){
  BIT_NOT* Copy = new BIT_NOT(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_NOT::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOT::Evaluate(){
  EXPRESSION* Result = 0;

  Result = (EXPRESSION*)Copy(true);
  if(!Result->Right){
    delete Result;
    return 0;
  }
  assert(Result->Right->Type > TYPE::Expression,
    delete Result;
    return 0;
  );

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOT::Simplify(){
  assert(Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify();

  EXPRESSION* Result = this;

  if(((EXPRESSION*)Right)->Type == TYPE::Literal){
    error("Not yet implemented");
    // TODO: Literals require a length, so it must have been cast to a
    //       specific length for this to be valid
  }

  return Result;
}
//------------------------------------------------------------------------------

void BIT_NOT::Display(){
  DisplayStart();

  Debug.print(" ~");

  DisplayEnd();
}
//------------------------------------------------------------------------------

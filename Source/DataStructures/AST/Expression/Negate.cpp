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

#include "Negate.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NEGATE::NEGATE(int Line, const string& Filename): NEGATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NEGATE::NEGATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Negate){
}
//------------------------------------------------------------------------------

NEGATE::~NEGATE(){
}
//------------------------------------------------------------------------------

BASE* NEGATE::Copy(bool CopyNext){
  NEGATE* Copy = new NEGATE(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool NEGATE::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::Evaluate(){
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

EXPRESSION* NEGATE::Simplify(){
  assert(Right, return this);
  assert(Right->Type > AST::BASE::TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify();

  EXPRESSION* Result = this;

  auto Right = (EXPRESSION*)this->Right;
  if(Right->Type == TYPE::Literal){
    Result = Right;
    this->Right = 0;
    Result->Value.Mul(-1);
    delete this;
  }
  return Result;
}
//------------------------------------------------------------------------------

void NEGATE::Display(){
  DisplayStart();

  Debug.print(" -");

  DisplayEnd();
}
//------------------------------------------------------------------------------

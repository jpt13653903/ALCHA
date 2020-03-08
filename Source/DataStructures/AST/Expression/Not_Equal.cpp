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

#include "Not_Equal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NOT_EQUAL::NOT_EQUAL(int Line, const string& Filename): NOT_EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NOT_EQUAL::NOT_EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Not_Equal){
}
//------------------------------------------------------------------------------

NOT_EQUAL::~NOT_EQUAL(){
}
//------------------------------------------------------------------------------

BASE* NOT_EQUAL::Copy(bool CopyNext){
  NOT_EQUAL* Copy = new NOT_EQUAL(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool NOT_EQUAL::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* NOT_EQUAL::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* NOT_EQUAL::Simplify(){
  assert(Left && Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Left = Left->Simplify();
  Right = ((EXPRESSION*)Right)->Simplify();

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void NOT_EQUAL::Display(){
  DisplayStart();

  Debug.print(" != ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

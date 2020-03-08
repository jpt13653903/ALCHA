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

#include "Cast.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CAST::CAST(int Line, const string& Filename): CAST(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CAST::CAST(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Cast){
}
//------------------------------------------------------------------------------

CAST::~CAST(){
}
//------------------------------------------------------------------------------

BASE* CAST::Copy(bool CopyNext){
  CAST* Copy = new CAST(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool CAST::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* CAST::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* CAST::Simplify(){
  // TODO: Root has the target type; Left has the original expression
  //       Right is optional and carry the class name (when applicable)
  assert(Left, return this);

  Left = Left->Simplify();
  if(Right){
    assert(Right->Type > TYPE::Expression, return this);
    Right = ((EXPRESSION*)Right)->Simplify();
  }

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void CAST::Display(){
  DisplayStart();

  Debug.print(" {cast} ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

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

#include "FunctionCall.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FUNCTIONCALL::FUNCTIONCALL(int Line, const string& Filename): FUNCTIONCALL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

FUNCTIONCALL::FUNCTIONCALL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::FunctionCall){
}
//------------------------------------------------------------------------------

FUNCTIONCALL::~FUNCTIONCALL(){
}
//------------------------------------------------------------------------------

BASE* FUNCTIONCALL::Copy(bool CopyNext){
  FUNCTIONCALL* Copy = new FUNCTIONCALL(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool FUNCTIONCALL::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* FUNCTIONCALL::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* FUNCTIONCALL::Simplify(){
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

void FUNCTIONCALL::Display(){
  DisplayStart();

  Debug.print("{call}" );

  DisplayEnd();
}
//------------------------------------------------------------------------------

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

#include "Logical_NOT.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LOGICAL_NOT::LOGICAL_NOT(int Line, const string& Filename): LOGICAL_NOT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LOGICAL_NOT::LOGICAL_NOT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Logical_NOT){
}
//------------------------------------------------------------------------------

LOGICAL_NOT::~LOGICAL_NOT(){
}
//------------------------------------------------------------------------------

BASE* LOGICAL_NOT::Copy(bool CopyNext){
  LOGICAL_NOT* Copy = new LOGICAL_NOT(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool LOGICAL_NOT::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_NOT::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_NOT::Simplify(){
  assert(Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify();

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void LOGICAL_NOT::Display(){
  DisplayStart();

  Debug.print( " !");

  DisplayEnd();
}
//------------------------------------------------------------------------------

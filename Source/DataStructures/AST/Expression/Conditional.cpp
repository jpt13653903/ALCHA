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

#include "Conditional.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CONDITIONAL::CONDITIONAL(int Line, const string& Filename): CONDITIONAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CONDITIONAL::CONDITIONAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Conditional){
}
//------------------------------------------------------------------------------

CONDITIONAL::~CONDITIONAL(){
}
//------------------------------------------------------------------------------

BASE* CONDITIONAL::Copy(bool CopyNext){
  CONDITIONAL* Copy = new CONDITIONAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

BASE* CONDITIONAL::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool CONDITIONAL::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* CONDITIONAL::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* CONDITIONAL::Simplify(bool GenWire){
  // TODO: There should be a third component...  Left and Right of Right?
  //       Or break it down to an if-statement?
  assert(Left && Right, return this);
  assert(Right->IsExpression(), return this);

  Left = Left->Simplify(true);
  Right = ((EXPRESSION*)Right)->Simplify(true);

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void CONDITIONAL::Display(){
  DisplayStart();

  Debug.print(" ? ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

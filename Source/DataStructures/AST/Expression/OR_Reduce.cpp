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

#include "OR_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

OR_REDUCE::OR_REDUCE(int Line, const string& Filename): OR_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

OR_REDUCE::OR_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::OR_Reduce){
}
//------------------------------------------------------------------------------

OR_REDUCE::~OR_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* OR_REDUCE::Copy(bool CopyNext){
  OR_REDUCE* Copy = new OR_REDUCE(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool OR_REDUCE::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool OR_REDUCE::GetVerilog(string& Body){
  Body += "|(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* OR_REDUCE::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* OR_REDUCE::Simplify(bool GenWire){
  assert(Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify(true);

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void OR_REDUCE::Display(){
  DisplayStart();

  Debug.print(" |");

  DisplayEnd();
}
//------------------------------------------------------------------------------

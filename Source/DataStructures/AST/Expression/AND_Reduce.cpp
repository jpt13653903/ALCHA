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

#include "AND_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

AND_REDUCE::AND_REDUCE(int Line, const string& Filename): AND_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

AND_REDUCE::AND_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::AND_Reduce){
}
//------------------------------------------------------------------------------

AND_REDUCE::~AND_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* AND_REDUCE::Copy(bool CopyNext){
  AND_REDUCE* Copy = new AND_REDUCE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

BASE* AND_REDUCE::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool AND_REDUCE::GetVerilog(string& Body){
  Body += "&(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;

}
//------------------------------------------------------------------------------

EXPRESSION* AND_REDUCE::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* AND_REDUCE::Simplify(bool GenWire){
  assert(Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Right = ((EXPRESSION*)Right)->Simplify(true);

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void AND_REDUCE::Display(){
  DisplayStart();

  Debug.print( " &");

  DisplayEnd();
}
//------------------------------------------------------------------------------

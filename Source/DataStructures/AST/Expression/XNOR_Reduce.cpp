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

#include "XNOR_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

XNOR_REDUCE::XNOR_REDUCE(int Line, const string& Filename): XNOR_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

XNOR_REDUCE::XNOR_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::XNOR_Reduce){
}
//------------------------------------------------------------------------------

XNOR_REDUCE::~XNOR_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* XNOR_REDUCE::Copy(bool CopyNext){
  XNOR_REDUCE* Copy = new XNOR_REDUCE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

BASE* XNOR_REDUCE::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool XNOR_REDUCE::GetVerilog(string& Body){
  Body += "~^(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* XNOR_REDUCE::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* XNOR_REDUCE::Simplify(bool GenWire){
  assert(Right, return this);
  assert(Right->IsExpression(), return this);

  Right = ((EXPRESSION*)Right)->Simplify(true);

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void XNOR_REDUCE::Display(){
  DisplayStart();

  Debug.print(" ~#");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void XNOR_REDUCE::ValidateMembers(){
  assert(Type == TYPE::XNOR_Reduce);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------


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

#include "Equal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

EQUAL::EQUAL(int Line, const string& Filename): EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

EQUAL::EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Equal){
}
//------------------------------------------------------------------------------

EQUAL::~EQUAL(){
}
//------------------------------------------------------------------------------

BASE* EQUAL::Copy(bool CopyNext){
  EQUAL* Copy = new EQUAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool EQUAL::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool EQUAL::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") == (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* EQUAL::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* EQUAL::Simplify(bool GenWire){
  assert(Left && Right, return this);
  assert(Right->IsExpression(), return this);

  Left = Left->Simplify(true);
  Right = ((EXPRESSION*)Right)->Simplify(true);

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void EQUAL::Display(){
  DisplayStart();

  Debug.print(" == ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void EQUAL::ValidateMembers(){
  assert(Type == TYPE::Equal);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------


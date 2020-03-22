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

#include "Subtract.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const string& Filename): SUBTRACT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Subtract){
}
//------------------------------------------------------------------------------

SUBTRACT::~SUBTRACT(){
}
//------------------------------------------------------------------------------

BASE* SUBTRACT::Copy(bool CopyNext){
  SUBTRACT* Copy = new SUBTRACT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

BASE* SUBTRACT::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool SUBTRACT::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") - (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::Simplify(bool GenWire){
  assert(Left && Right, return this);
  assert(Right->IsExpression(), return this);

  Left = Left->Simplify(true);
  Right = ((EXPRESSION*)Right)->Simplify(true);

  EXPRESSION* Result = this;

  if(Left->Type == TYPE::Literal && Right->Type == TYPE::Literal){
    auto Literal = new LITERAL(Source.Line, Source.Filename);
    Literal->Value =   ((LITERAL*)Left )->Value;
    Literal->Value.Sub(((LITERAL*)Right)->Value);
    delete this;
    Result = Literal;
  }
  // TODO When subtracting an expression from a literal (or vice versa),
  //      follow the rules in the SIPS article

  error("Not yet implemented");
  return Result;
}
//------------------------------------------------------------------------------

void SUBTRACT::Display(){
  DisplayStart();

  Debug.print(" - " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

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

#include "VectorConcatenate.h"

#include "Literal.h"
#include "Multiply.h"
#include "Object.h"

#include "Netlist/Synthesisable/Net.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

VECTORCONCATENATE::VECTORCONCATENATE(int Line, const string& Filename): VECTORCONCATENATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

VECTORCONCATENATE::VECTORCONCATENATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::VectorConcatenate){
}
//------------------------------------------------------------------------------

VECTORCONCATENATE::~VECTORCONCATENATE(){
  foreach(Element, Elements) delete *Element;
}
//------------------------------------------------------------------------------

BASE* VECTORCONCATENATE::Copy(){
  VECTORCONCATENATE* Copy = new VECTORCONCATENATE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy());
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool VECTORCONCATENATE::GetVerilog(string& Body){
  bool isFirst = true;

  foreach(Element, Elements){
    if(isFirst) Body += "{";
    else        Body += ", ";
    (*Element)->GetVerilog(Body);
    isFirst = false;
  }
  Body += "}";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* VECTORCONCATENATE::Evaluate(){
  foreach(Element, Elements){
    (*Element) = (*Element)->Evaluate();
  }
  warning("Incomplete implementation");
  // TODO: If there are elements that can be combined, do so
  return this;
}
//------------------------------------------------------------------------------

int VECTORCONCATENATE::GetWidth(){
  int Result = 0;
  foreach(Element, Elements){
   *Element = (*Element)->Evaluate();
    Result += (*Element)->GetWidth();
  }
  return Result;
}
//------------------------------------------------------------------------------

EXPRESSION* VECTORCONCATENATE::FixedPointScale(int Width, NUMBER& FullScale){
  NUMBER Scale = 1;
  Scale.BinScale(Width);
  Scale.Div(FullScale);

  if(Scale == 1) return this;

  auto Object  = new OBJECT      (Source.Line, Source.Filename);
  auto Net     = new NETLIST::NET(Source.Line, Source.Filename, 0);
  auto Mul     = new MULTIPLY    (Source.Line, Source.Filename);
  auto Literal = new LITERAL     (Source.Line, Source.Filename);

  Net->SetFixedPoint(Width, FullScale);

  Literal->Value     = Scale;
  Mul    ->Left      = this;
  Mul    ->Right     = Literal;
  Net    ->Value     = Mul;
  Object ->ObjectRef = Net;

  NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;

  return Object;
}
//------------------------------------------------------------------------------

void VECTORCONCATENATE::Display(){
  Debug.Print("(VectorConcat: (");
  bool isFirst = true;
  foreach(Element, Elements){
    if(!isFirst) Debug.Print(", ");
    (*Element)->Display();
    isFirst = false;
  }
  Debug.Print("))");
}
//------------------------------------------------------------------------------

void VECTORCONCATENATE::ValidateMembers(){
  assert(Type == TYPE::VectorConcatenate);

  assert(!Next);
  assert(!Prev);

  assert(!Left);
  assert(!Right);

  foreach(Element, Elements){
    assert(*Element);
    (*Element)->Validate();
  }
}
//------------------------------------------------------------------------------


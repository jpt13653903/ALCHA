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

#include "ArrayConcatenate.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ARRAYCONCATENATE::ARRAYCONCATENATE(int Line, const string& Filename): ARRAYCONCATENATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ARRAYCONCATENATE::ARRAYCONCATENATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::ArrayConcatenate){
}
//------------------------------------------------------------------------------

ARRAYCONCATENATE::~ARRAYCONCATENATE(){
  foreach(Element, Elements) delete *Element;
}
//------------------------------------------------------------------------------

BASE* ARRAYCONCATENATE::Copy(){
  ARRAYCONCATENATE* Copy = new ARRAYCONCATENATE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy());
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool ARRAYCONCATENATE::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAYCONCATENATE::Evaluate(){
  error("Not yet implemented");
  return this;
//   auto Array = (AST::ARRAYCONCATENATE*)Copy(true);
//   foreach(Element, Array->Elements) (*Element) = (*Element)->Evaluate();
// 
//   return Array->Simplify(false);
}
//------------------------------------------------------------------------------

int ARRAYCONCATENATE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& ARRAYCONCATENATE::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool ARRAYCONCATENATE::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAYCONCATENATE::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool ARRAYCONCATENATE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ARRAYCONCATENATE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void ARRAYCONCATENATE::Display(){
  Debug.Print("(ArrayConcat: (");
  bool isFirst = true;
  foreach(Element, Elements){
    if(!isFirst) Debug.Print(", ");
    (*Element)->Display();
    isFirst = false;
  }
  Debug.Print("))");
}
//------------------------------------------------------------------------------

void ARRAYCONCATENATE::ValidateMembers(){
  assert(Type == TYPE::ArrayConcatenate);

  assert(!Next);
  assert(!Prev);

  assert(!Left );
  assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------


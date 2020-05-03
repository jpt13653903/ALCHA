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

#include "Array.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ARRAY::ARRAY(int Line, const string& Filename): ARRAY(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ARRAY::ARRAY(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Array){
}
//------------------------------------------------------------------------------

ARRAY::~ARRAY(){
  foreach(Element, Elements) delete *Element;
}
//------------------------------------------------------------------------------

BASE* ARRAY::Copy(){
  ARRAY* Copy = new ARRAY(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy());
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool ARRAY::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAY::Evaluate(){
  error("Not yet implemented");
  return this;
//   ARRAY* Array = (ARRAY*)Copy(true);
//   foreach(Element, Array->Elements) (*Element) = (*Element)->Evaluate();
// 
//   return Array->Simplify(false);
}
//------------------------------------------------------------------------------

int ARRAY::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& ARRAY::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool ARRAY::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAY::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool ARRAY::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ARRAY::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void ARRAY::Display(){
  Debug.Print("(Array: (");
  bool isFirst = true;
  foreach(Element, Elements){
    if(!isFirst) Debug.Print(", ");
    (*Element)->Display();
    isFirst = false;
  }
  Debug.Print("))");
}
//------------------------------------------------------------------------------

void ARRAY::ValidateMembers(){
  assert(Type == TYPE::Array);
  
  assert(!Next);
  assert(!Prev);

  assert(!Left);
  assert(!Right);

  foreach(Element, Elements) (*Element)->Validate();
}
//------------------------------------------------------------------------------


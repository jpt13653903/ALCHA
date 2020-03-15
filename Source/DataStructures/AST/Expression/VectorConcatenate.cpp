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

BASE* VECTORCONCATENATE::Copy(bool CopyNext){
  VECTORCONCATENATE* Copy = new VECTORCONCATENATE(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy(CopyNext));
  }

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool VECTORCONCATENATE::RunScripting(){
  error("Not yet implemented");
  return false;
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
  auto Array = (VECTORCONCATENATE*)Copy(true);
  foreach(Element, Array->Elements) (*Element) = (*Element)->Evaluate();

  return Array->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* VECTORCONCATENATE::Simplify(bool GenWire){
  foreach(Element, Elements){
    *Element = (*Element)->Simplify(true);
    assert((*Element)->Type == TYPE::Object, return this);
  }

  if(GenWire){
    error("Not yet implemented");
  }

  return this;
}
//------------------------------------------------------------------------------

void VECTORCONCATENATE::Display(){
  assert(!Left);

  Debug.print("(VectorConcat: (");
  bool isFirst = true;
  foreach(Element, Elements){
    if(!isFirst) Debug.print(", ");
    (*Element)->Display();
    isFirst = false;
  }
  Debug.print("))");

  assert(!Right);
  assert(!Next );
}
//------------------------------------------------------------------------------

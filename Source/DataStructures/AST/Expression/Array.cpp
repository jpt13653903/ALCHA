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

BASE* ARRAY::Copy(bool CopyNext){
  ARRAY* Copy = new ARRAY(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy(CopyNext));
  }

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool ARRAY::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAY::Evaluate(){
  ARRAY* Array = (ARRAY*)Copy(true);
  foreach(Element, Array->Elements) (*Element) = (*Element)->Evaluate();

  return Array->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAY::Simplify(){
  return this;
}
//------------------------------------------------------------------------------

void ARRAY::Display(){
  assert(!Left);

  Debug.print("(Array: (");
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

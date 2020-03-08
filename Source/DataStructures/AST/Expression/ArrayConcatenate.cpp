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

BASE* ARRAYCONCATENATE::Copy(bool CopyNext){
  ARRAYCONCATENATE* Copy = new ARRAYCONCATENATE(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  foreach(Element, Elements){
    Copy->Elements.push_back((EXPRESSION*)(*Element)->Copy(CopyNext));
  }

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool ARRAYCONCATENATE::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAYCONCATENATE::Evaluate(){
  auto Array = (AST::ARRAYCONCATENATE*)Copy(true);
  foreach(Element, Array->Elements) (*Element) = (*Element)->Evaluate();

  return Array->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* ARRAYCONCATENATE::Simplify(){
  // TODO: Simplify the elements
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void ARRAYCONCATENATE::Display(){
  assert(!Left);

  Debug.print("(ArrayConcat: (");
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

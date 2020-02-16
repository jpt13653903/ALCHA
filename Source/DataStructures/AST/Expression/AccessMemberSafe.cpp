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

#include "AccessMemberSafe.h"
#include "../Assignment.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ACCESSMEMBERSAFE::ACCESSMEMBERSAFE(int Line, const string& Filename): ACCESSMEMBERSAFE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ACCESSMEMBERSAFE::ACCESSMEMBERSAFE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::AccessMemberSafe){
}
//------------------------------------------------------------------------------

ACCESSMEMBERSAFE::~ACCESSMEMBERSAFE(){
}
//------------------------------------------------------------------------------

BASE* ACCESSMEMBERSAFE::Copy(bool CopyNext){
  ACCESSMEMBERSAFE* Copy = new ACCESSMEMBERSAFE(Line, Filename.c_str());

  Copy->Name      = Name;
  Copy->Value     = Value;
  Copy->StrValue  = StrValue;
  Copy->ObjectRef = ObjectRef;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

void ACCESSMEMBERSAFE::Display(){
  DisplayStart();

  Debug.print("?." );

  DisplayEnd();
}
//------------------------------------------------------------------------------

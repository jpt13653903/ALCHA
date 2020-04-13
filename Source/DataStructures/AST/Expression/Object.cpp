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

#include "Object.h"
#include "Literal.h"
#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

OBJECT::OBJECT(int Line, const string& Filename): OBJECT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

OBJECT::OBJECT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Object){
  ObjectRef = 0;
}
//------------------------------------------------------------------------------

OBJECT::~OBJECT(){
  // Don't delete the Object reference -- it's part of the namespace tree
}
//------------------------------------------------------------------------------

BASE* OBJECT::Copy(bool CopyNext){
  OBJECT* Copy = new OBJECT(Source.Line, Source.Filename.c_str());

  Copy->ObjectRef = ObjectRef;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool OBJECT::GetVerilog(string& Body){
  assert(ObjectRef, return false);
  Body += ObjectRef->EscapedName();

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* OBJECT::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = 0;
// 
//   error("Not yet implemented");
// 
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

// EXPRESSION* OBJECT::Simplify(bool GenWire){
//   EXPRESSION* Result = this;
// 
//   if(ObjectRef){
//     switch(ObjectRef->Type){
//       case NETLIST::BASE::TYPE::Byte:{
//         auto Byte = (NETLIST::BYTE*)ObjectRef;
//         Result = new LITERAL(Source.Line, Source.Filename);
//         ((LITERAL*)Result)->Value = Byte->Value;
//         delete this;
//       }
//       case NETLIST::BASE::TYPE::Character:{
//         auto Char = (NETLIST::CHARACTER*)ObjectRef;
//         Result = new LITERAL(Source.Line, Source.Filename);
//         ((LITERAL*)Result)->Value = Char->Value;
//         delete this;
//       }
//       case NETLIST::BASE::TYPE::Number:{
//         auto Num = (NETLIST::NUM*)ObjectRef;
//         Result = new LITERAL(Source.Line, Source.Filename);
//         ((LITERAL*)Result)->Value = Num->Value;
//         delete this;
//       }
//       default:
//         break;
//     }
//   }
//   return Result;
// }
//------------------------------------------------------------------------------

void OBJECT::Display(){
  DisplayStart();

  if(ObjectRef){
    // TODO: Display the width here, because it should be a property
    //       of the target object, not the expression itself
    ObjectRef->DisplayLongName();
  }else{
    error("Null object reference");
  }

  DisplayEnd();
}
//------------------------------------------------------------------------------

void OBJECT::ValidateMembers(){
  assert(Type == TYPE::Object);

  assert(!Next);
  assert(!Prev);

  assert(!Left );
  assert(!Right);

  assert(ObjectRef);
}
//------------------------------------------------------------------------------


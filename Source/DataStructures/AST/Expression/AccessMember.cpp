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

#include "AccessMember.h"
#include "Object.h"
#include "Identifier.h"
#include "Netlist/Alias.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Namespace.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ACCESSMEMBER::ACCESSMEMBER(int Line, const string& Filename): ACCESSMEMBER(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ACCESSMEMBER::ACCESSMEMBER(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::AccessMember){
}
//------------------------------------------------------------------------------

ACCESSMEMBER::~ACCESSMEMBER(){
}
//------------------------------------------------------------------------------

BASE* ACCESSMEMBER::Copy(){
  ACCESSMEMBER* Copy = new ACCESSMEMBER(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool ACCESSMEMBER::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSMEMBER::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = 0;
// 
//   if(this->Left) this->Left = this->Left->Evaluate();
// 
//   if(!this->Left || !this->Right){
//     // Typically caused by a syntax error
//     if(this->Left) delete this->Left;
//     this->Left = 0;
//     return 0;
//   }
//   auto Right = this->Right;
// 
//   if(this->Left->Type == TYPE::Object){
//     auto Left = (OBJECT*)this->Left;
//     if(Left->ObjectRef){
//       switch(Left->ObjectRef->Type){
//         case NETLIST::BASE::TYPE::Pin:{
//           // auto Pin = (PIN*)Left->ObjectRef;
//           // TODO Implement explicit "driver", "enabled" and "pin" access
//           error("Not yet implemented");
//           delete Left;
//           this->Left = 0;
//           return 0;
//         }
//         case NETLIST::BASE::TYPE::Module:
//         case NETLIST::BASE::TYPE::Group:{
//           assert(Right->Type == TYPE::Identifier, delete Left; this->Left = 0; return 0);
//           auto Right = (IDENTIFIER*)this->Right;
//           auto Namespace = (NETLIST::NAMESPACE*)Left->ObjectRef;
//           auto Found  = Namespace->Symbols.find(Right->Name);
//           if(Found == Namespace->Symbols.end()){
//             Error();
//             printf("Object %s not found in namespace %s\n",
//                    Right->Name.c_str(), Namespace->Name.c_str());
//             delete Left;
//             this->Left = 0;
//             return 0;
//           }
//           if(Found->second){
//             if(Found->second->Type == NETLIST::BASE::TYPE::Alias){
//               auto Alias = (NETLIST::ALIAS*)Found->second;
//               NETLIST::NamespaceStack.push_front(Alias->Namespace);
//                 Result = Alias->Expression->Evaluate();
//               NETLIST::NamespaceStack.pop_front();
//             }else{
//               OBJECT* Object = new OBJECT(Source.Line, Source.Filename);
//               Object->ObjectRef = Found->second;
//               if(Object->ObjectRef->IsSynthesisable()){
//                 auto ObjectRef = (NETLIST::SYNTHESISABLE*)Object->ObjectRef;
//                 ObjectRef->Used = true;
//               }
//               Result = Object;
//             }
//           }
//           break;
//         }
//         default:{
//           Error("Invalid member access");
//           delete Left;
//           this->Left = 0;
//           return 0;
//         }
//       }
//     }else{
//       error("Unexpected null expression");
//       delete Left;
//       this->Left = 0;
//       return 0;
//     }
//   }else{
//     // TODO Could be a slice expression, which is not supported yet
//     error("Unimplemented member access expression");
//     delete this->Left;
//     this->Left = 0;
//     return 0;
//   }
// 
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

// EXPRESSION* ACCESSMEMBER::Simplify(bool GenWire){
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void ACCESSMEMBER::Display(){
  DisplayStart();

  Debug.Print("." );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void ACCESSMEMBER::ValidateMembers(){
  assert(Type == TYPE::AccessMember);
  
  assert(!Next);
  assert(!Prev);

  // TODO: The "Left" assertion fails when the member cannot be found
  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------


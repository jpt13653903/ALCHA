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
#include "Netlist/Module.h"
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

BASE* ACCESSMEMBER::Copy(bool CopyNext){
  ACCESSMEMBER* Copy = new ACCESSMEMBER(Line, Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool ACCESSMEMBER::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSMEMBER::Evaluate(){
  EXPRESSION* Result = 0;

  EXPRESSION* Left = 0;
  if(this->Left) Left = this->Left->Evaluate();

  if(!Left || !this->Right || this->Right->Type <= TYPE::Expression){
    error("Invalid member access expression");
    delete Left;
    return 0;
  }
  auto Right = (EXPRESSION*)this->Right;

  if(Left->Type == TYPE::Object){
    auto Left = (OBJECT*)this->Left;
    if(Left->ObjectRef){
      switch(Left->ObjectRef->Type){
        case NETLIST::BASE::TYPE::Pin:{
          // auto Pin = (PIN*)Left->ObjectRef;
          // TODO Implement explicit "driver", "enabled" and "pin" access
          error("Not yet implemented");
          delete Left;
          return 0;
        }
        case NETLIST::BASE::TYPE::Module:
        case NETLIST::BASE::TYPE::Group:{
          assert(Right->Type == TYPE::Identifier, delete Left; return 0);
          auto Right = (IDENTIFIER*)this->Right;
          auto Namespace = (NETLIST::NAMESPACE*)Left->ObjectRef;
          auto Found  = Namespace->Symbols.find(Right->Name);
          if(Found == Namespace->Symbols.end()){
            Error();
            printf("Object %s not found in namespace %s\n",
                   Right->Name.c_str(), Namespace->Name.c_str());
            delete Left;
            return 0;
          }
          if(Found->second){
            if(Found->second->Type == NETLIST::BASE::TYPE::Alias){
              auto Alias = (NETLIST::ALIAS*)Found->second;
              NETLIST::NamespaceStack.push_front(Alias->Namespace);
                Result = Alias->Expression->Evaluate();
              NETLIST::NamespaceStack.pop_front();
            }else{
              OBJECT* Object = new OBJECT(this->Line, this->Filename);
              Object->ObjectRef = Found->second;
              if(Object->ObjectRef->Type == NETLIST::BASE::TYPE::Pin ||
                 Object->ObjectRef->Type == NETLIST::BASE::TYPE::Net ){
                auto ObjectRef = (NETLIST::SYNTHESISABLE*)Object->ObjectRef;
                ObjectRef->Used      = true;
              }
              Result = Object;
            }
          }
          delete Left;
          break;
        }
        default:{
          Error("Invalid member access");
          delete Left;
          return 0;
        }
      }
    }else{
      error("Unexpected null expression");
      delete Left;
      return 0;
    }
  }else{
    // TODO Could be a slice expression, which is not supported yet
    error("Unimplemented member access expression");
    delete Left;
    return 0;
  }

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSMEMBER::Simplify(bool GenWire){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void ACCESSMEMBER::Display(){
  DisplayStart();

  Debug.print("." );

  DisplayEnd();
}
//------------------------------------------------------------------------------

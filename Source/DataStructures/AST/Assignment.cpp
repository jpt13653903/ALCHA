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

#include "Assignment.h"

#include "Netlist/Alias.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"
#include "Netlist/Namespace/Module.h"

#include "Expression/Object.h"
#include "Expression/Identifier.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ASSIGNMENT::ASSIGNMENT(
  int             Line,
  const char*     Filename,
  TYPE            AssignmentType
): BASE(Line, Filename, AssignmentType){
  Fence = false;
  Left  = Right = 0;
}
//------------------------------------------------------------------------------

ASSIGNMENT::~ASSIGNMENT(){
  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::IsAssignment(){
  return true;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::GetLHS_Object(NETLIST::BASE* Object, target_list& List, BASE* Ast){
  TARGET_LIST ListNode;

  bool Result = false;

  if(Object){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Pin:{
        auto Pin = (NETLIST::PIN*)Object;
        if(Pin->Direction == DEFINITION::DIRECTION::Input){
          Ast->Error("Cannot assign to an input pin");
          return false;
        }
        Pin->Used = true;
        ListNode.Object     =  Pin;
        ListNode.Expression = &Pin->Driver;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Net:{
        auto Net = (NETLIST::NET*)Object;
        ListNode.Object     =  Net;
        ListNode.Expression = &Net->Value;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Number:
      case NETLIST::BASE::TYPE::Byte:
      case NETLIST::BASE::TYPE::Character:{
        ListNode.Object = Object;
        ListNode.Expression = 0;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Alias:{
        auto Alias = (NETLIST::ALIAS*)Object;
        NETLIST::NamespaceStack.push_front(Alias->Namespace);
          Result = GetLHS(Alias->Expression, List);
        NETLIST::NamespaceStack.pop_front();
        break;
      }
      case NETLIST::BASE::TYPE::Array:{
        error("Array assignment not yet implemented");
        break;
      }
      case NETLIST::BASE::TYPE::Module:
      case NETLIST::BASE::TYPE::Group:{
        ListNode.Object     = Object;
        ListNode.Expression = 0;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      default:
        error("Unknown object type %d", (int)Object->Type);
        break;
    }
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::GetLHS(EXPRESSION* Node, target_list& List){
  if(!Node) return 0;

  bool        Result = false;
  TARGET_LIST ListNode;

  switch(Node->Type){
    case TYPE::Array:{
      error("Array not yet implemented");
      // Idea: Simply call GetLHS recursively for each array element
      //
      // auto Element = (EXPRESSION*)Node->Right;
      // while(Element){
      //   Result->Elements.push_back(Evaluate(Element));
      //   Element = (EXPRESSION*)Element->Next;
      // }
      break;
    }

    case TYPE::Identifier:{
      auto Identifier = (IDENTIFIER*)Node;
      auto NamespaceIterator = NETLIST::NamespaceStack.begin();
      while(!Result && NamespaceIterator != NETLIST::NamespaceStack.end()){
        NETLIST::NAMESPACE* Namespace = *NamespaceIterator;
        while(!Result && Namespace){
          auto Object = Namespace->Symbols.find(Identifier->Name);
          if(Object != Namespace->Symbols.end()){
            Result = GetLHS_Object(Object->second, List, Identifier);
          }
          Namespace = Namespace->Namespace;
        }
        NamespaceIterator++;
      }
      if(!Result){
        Node->Error();
        printf("Undefined identifier: \"%s\"\n", Identifier->Name.c_str());
      }
      break;
    }

    case TYPE::VectorConcatenate:{
      error("VectorConcatenate not yet implemented");
      // Result = (EXPRESSION*)Node->Copy(true);
      break;
    }

    case TYPE::ArrayConcatenate:{
      error("ArrayConcatenate not yet implemented");
      // Result = (EXPRESSION*)Node->Copy(true);
      break;
    }

    case TYPE::Slice:
      error("Slice not yet implemented");
      break;

    case TYPE::AccessMember:{
      assert(Node->Left                 , return false);
      assert(Node->Right                , return false);
      assert(Node->Right->IsExpression(), return false);

      target_list LeftList;
      if(!GetLHS(Node->Left, LeftList)) return false;
      if(LeftList.empty()){
        error("Empty target list");
        return false;
      }
      if(LeftList.size() > 1){
        error("Multiple assignment targets not yet supported");
        return false;
      }
      auto Left  = LeftList.front();
      auto Right = (EXPRESSION*)Node->Right;

      if(Left.isAttribute){
        Node->Error("Attributes cannot be structures");
        return false;
      }

      switch(Left.Object->Type){
        case NETLIST::BASE::TYPE::Pin:{
          auto Pin = (NETLIST::PIN*)Left.Object;
          ListNode.Object = Pin;
          if(Right->Type != TYPE::Identifier){
            Node->Error("Invalid pin member");
            return false;
          }
          if(Pin->Direction == DEFINITION::DIRECTION::Input){
            Node->Error("Cannot assign to an input pin.");
            return false;
          }
          if(((IDENTIFIER*)Right)->Name == "driver"){
            ListNode.Expression = &Pin->Driver;
          }else if(((IDENTIFIER*)Right)->Name == "enabled"){
            ListNode.Expression = &Pin->Enabled;
          }else{
            Node->Error("Valid pin members are \"driver\" and \"enabled\" only");
            return false;
          }
          List.push_back(ListNode);
          Result = true;
          break;
        }
        case NETLIST::BASE::TYPE::Module:
        case NETLIST::BASE::TYPE::Group:{
          assert(Right->Type == TYPE::Identifier, return false);
          auto Namespace = (NETLIST::NAMESPACE*)Left.Object;
          auto Object    = Namespace->Symbols.find(((IDENTIFIER*)Right)->Name);
          if(Object == Namespace->Symbols.end()){
            Node->Error();
            printf("Object %s not found in namespace %s\n",
                   ((IDENTIFIER*)Right)->Name.c_str(), Namespace->Name.c_str());
            return false;
          }
          Result = GetLHS_Object(Object->second, List, Node);
          break;
        }
        case NETLIST::BASE::TYPE::Array:{
          error("Array not yet implemented");
          return false;
        }
        default:
          Node->Error();
          printf("Invalid type for member access: %d\n", (int)Left.Object->Type);
          return false;
      }
      break;
    }

    case TYPE::AccessMemberSafe:
      error("AccessMemberSafe not yet implemented");
      break;

    case TYPE::AccessAttribute:{
      if(Node->Left){
        target_list LeftList;
        if(!GetLHS(Node->Left, LeftList)) return false;
        if(LeftList.empty()){
          error("Empty target list");
          return false;
        }
        if(LeftList.size() > 1){
          error("Multiple assignment targets not yet supported");
          return false;
        }
        ListNode = LeftList.front();
      }else{ // An attribute of the current namespace
        ListNode.Object = NETLIST::NamespaceStack.front();
      }
      if(ListNode.isAttribute){
        Node->Error("Attributes are not hierarchical");
        return false;
      }

      if(Node->Right && Node->Right->IsExpression()){
        auto Right = (EXPRESSION*)Node->Right;
        if(Right->Type == TYPE::Identifier){
          // The process of adding an entry initialises the pointer to null.
          // The default constructor of the pointer type is called.
          ListNode.Expression = &ListNode.Object->Attributes[((IDENTIFIER*)Right)->Name];
          ListNode.isAttribute = true;
          List.push_back(ListNode);
          Result = true;
        }else{
          // TODO Could be a slice expression, which is not supported yet
          error("Unimplemented attribute access expression");
        }
      }
      break;
    }

    default:
      Node->Error("Invalid LHS expression");
      break;
  }
  return Result;
}
//------------------------------------------------------------------------------


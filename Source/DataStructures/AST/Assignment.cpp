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
#include "Netlist/Attribute.h"
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
): BASE(Line, Filename, AssignmentType)
{
    Left = Right = 0;
}
//------------------------------------------------------------------------------

ASSIGNMENT::~ASSIGNMENT()
{
    if(Left ) delete Left;
    if(Right) delete Right;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::IsAssignment()
{
    return true;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::AddLHS_Object(NETLIST::BASE* Object, target_list& List)
{
    bool Result = false;

    if(Object){
        switch(Object->Type){
            case NETLIST::BASE::TYPE::Pin:
            case NETLIST::BASE::TYPE::PinComponent:
            case NETLIST::BASE::TYPE::Net:
            case NETLIST::BASE::TYPE::Number:
            case NETLIST::BASE::TYPE::Byte:
            case NETLIST::BASE::TYPE::Character:
            case NETLIST::BASE::TYPE::Module:
            case NETLIST::BASE::TYPE::Group:{
                List.push_back(Object);
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
            default:
                error("Unknown object type %d", (int)Object->Type);
                break;
        }
    }
    return Result;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::GetLHS(EXPRESSION* Node, target_list& List)
{
    if(!Node) return 0;

    bool Result = false;

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
            foreach(NamespaceIterator, NETLIST::NamespaceStack){
                auto Namespace = *NamespaceIterator;
                while(!Result && Namespace){
                    auto Object = Namespace->GetMember(Identifier->Name);
                    if(Object){
                        Result = AddLHS_Object(Object, List);
                    }
                    Namespace = Namespace->Namespace;
                }
                if(Result) break;
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
            assert(Node->Left , return false);
            assert(Node->Right, return false);

            target_list LeftList;
            if(!GetLHS(Node->Left, LeftList)) return false;
            assert(!LeftList.empty(), return false);
            if(LeftList.size() > 1){
                error("Multiple assignment targets not yet supported");
                return false;
            }
            auto Left  = LeftList.front();
            auto Right = Node->Right;

            assert(Right->Type == TYPE::Identifier, return false);
            auto Object = Left->GetMember(((IDENTIFIER*)Right)->Name);
            if(!Object){
                Node->Error();
                printf("Object %s not a member of %s\n",
                              ((IDENTIFIER*)Right)->Name.c_str(), Left->Name.c_str());
                return false;
            }
            Result = AddLHS_Object(Object, List);

            break;
        }

        case TYPE::AccessMemberSafe:
            error("AccessMemberSafe not yet implemented");
            break;

        case TYPE::AccessAttribute:{
            assert(Node->Right, return false);

            NETLIST::BASE* Left = 0;
            if(Node->Left){
                target_list LeftList;
                if(!GetLHS(Node->Left, LeftList)) return false;
                assert(!LeftList.empty(), return false);
                if(LeftList.size() > 1){
                    error("Multiple assignment targets not yet supported");
                    return false;
                }
                Left = LeftList.front();
            }else{ // An attribute of the current namespace
                Left = NETLIST::NamespaceStack.front();
            }
            if(Left->Type == NETLIST::BASE::TYPE::Attribute){
                Node->Error("Attributes are not hierarchical");
                return false;
            }
            if(Left->Type == NETLIST::BASE::TYPE::PinComponent){
                Node->Error("Cannot assign attributes to pin components.  Assign to the pin directly");
                return false;
            }

            auto Right = Node->Right;
            if(Right->Type == TYPE::Identifier){
                // The process of adding an entry initialises the pointer to null.
                // The default constructor of the pointer type is called.
                auto Attribute = Left->GetAttribute(((IDENTIFIER*)Right)->Name);
                if(!Attribute){ // Create a new attribute
                    auto Attrib = new NETLIST::ATTRIBUTE(Right->Source.Line,
                                                                                          Right->Source.Filename,
                                                                                          ((IDENTIFIER*)Right)->Name.c_str());
                    Left->Attributes[((IDENTIFIER*)Right)->Name] = Attrib;
                    Attribute = Attrib;
                }
                List.push_back(Attribute);
                Result = true;
            }else{
                // TODO Could be a slice expression, which is not supported yet
                error("Unimplemented attribute access expression");
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

void ASSIGNMENT::DisplayAssignment(const char* Operator)
{
    DisplayInfo();
    Debug.Print("Assignment: ");

    if(Left){
        if(Left->Left || Left->Right) Debug.Print("(");
        Left->Display();
        if(Left->Left || Left->Right) Debug.Print(")");
    }

    Debug.Print(" %s ", Operator);

    if(Right){
        if(Right->Left || Right->Right) Debug.Print("(");
        Right->Display();
        if(Right->Left || Right->Right) Debug.Print(")");
    }else{
        Debug.Print("{Moved Expression}");
    }

    Debug.Print("\n");
    if(Next) Next->Display();
}
//------------------------------------------------------------------------------


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

#include "Add_Assign.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"

#include "../Expression/Add.h"
#include "../Expression/Object.h"
#include "../Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ADD_ASSIGN::ADD_ASSIGN(int Line, std::string Filename):
ADD_ASSIGN(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ADD_ASSIGN::ADD_ASSIGN(int Line, const char* Filename):
ASSIGNMENT(Line, Filename, TYPE::Add_Assign){}
//------------------------------------------------------------------------------

ADD_ASSIGN::~ADD_ASSIGN(){
}
//------------------------------------------------------------------------------

BASE* ADD_ASSIGN::Copy(bool CopyNext){
  ADD_ASSIGN* Copy = new ADD_ASSIGN(Source.Line, Source.Filename.c_str());

  Copy->Fence = Fence;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool ADD_ASSIGN::RunAST(){
  target_list TargetList;
  EXPRESSION* Expression;
  EXPRESSION* Temp;

  if(!GetLHS(Left, TargetList)) return false;
  if(TargetList.empty()){
    Error("Target object list is empty");
    return false;
  }

  if(TargetList.size() > 1){
    error("Multiple assignment targets not supported yet");
    return false;
  }

  Expression = Right->Evaluate();
  if(!Expression){
    // This is ok -- generally caused by a syntax or semantic error, but should
    // halt further compilation anyway
    return false;
  }

  NETLIST::BASE* Object = TargetList.front().Object;
  if(!Object){
    error("Unexpected null reference");
    return false;
  }

  EXPRESSION*  ScriptTarget = 0;
  EXPRESSION** Target = TargetList.front().Expression;
  if(!Target){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Byte:
      case NETLIST::BASE::TYPE::Character:
      case NETLIST::BASE::TYPE::Number:{
        ScriptTarget = new OBJECT(Source.Line, Source.Filename);
        ((OBJECT*)ScriptTarget)->ObjectRef = Object;
        Target = &ScriptTarget;
        break;
      }

      default:
        error("Unexpected null reference");
        return false;
    }
  }

  bool RawAssign = true;

  if(Expression->Type == TYPE::Literal){
    if(Object->IsSynthesisable()){
      auto Synth   = (NETLIST::SYNTHESISABLE*)Object;
      auto Literal = (LITERAL*)Expression;
      Literal->Signed = Synth->Signed;
      Literal->Width  = Synth->Width;
    }
  }

  switch(Type){
    case TYPE::Add_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new ADD(Source.Line, Source.Filename);
        Temp->Left  = *Target;
        Temp->Right = Expression;
        *Target     = Temp;
        break;
      }else{
        Error("Add-Assign with null target");
        delete Expression;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    default:
      error("Unknown assignment type: %d", (int)Type);
      delete Expression;
      if(ScriptTarget) delete ScriptTarget;
      return false;
  }
  *Target = (*Target)->Simplify(false);

  if(*Target){
    if(!RawAssign){
      if(Object->IsSynthesisable()){
        NETLIST::SYNTHESISABLE* Synth = (NETLIST::SYNTHESISABLE*)Object;
        if((*Target)->Type == TYPE::Literal){
          auto Literal = (LITERAL*)(*Target);
          Literal->Value.Div     (Synth->FullScale);
          Literal->Value.BinScale(Synth->Width);
        }
      }
    }
  }

  if(ScriptTarget){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Byte:{
        auto Byte = (NETLIST::BYTE*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Byte->Value = ((LITERAL*)ScriptTarget)->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case NETLIST::BASE::TYPE::Character:{
        auto Char = (NETLIST::CHARACTER*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Char->Value = ((LITERAL*)ScriptTarget)->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case NETLIST::BASE::TYPE::Number:{
        auto Num = (NETLIST::NUM*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Num->Value = ((LITERAL*)ScriptTarget)->Value;
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
          delete ScriptTarget;
          return false;
        }
        break;
      }
      default:
        error("Unexpected default case");
        break;
    }
    delete ScriptTarget;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ADD_ASSIGN::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ADD_ASSIGN::Display(){
  DisplayInfo();
  Debug.Print("Assignment: ");

  if(Left){
    if(Left->Left || Left->Right) Debug.Print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.Print(")");
  }

  switch(Type){
    case TYPE::Add_Assign        : Debug.Print( " += "); break;
    default: Debug.Print("(Unknown assignment type)");
  }

  if(Right){
    if(Right->Left || Right->Right) Debug.Print("(");
    Right->Display();
    if(Right->Left || Right->Right) Debug.Print(")");
  }

  if(Fence) Debug.Print("{Fence}");

  Debug.Print("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void ADD_ASSIGN::ValidateMembers(){
  assert(Type == TYPE::Add_Assign);

  assert(Left , return);
  assert(Right, return);

  Left ->Validate();
  Right->Validate();
}
//------------------------------------------------------------------------------


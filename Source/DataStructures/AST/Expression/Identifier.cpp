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

#include "Identifier.h"
#include "Object.h"
#include "Literal.h"

#include "Netlist/Alias.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

IDENTIFIER::IDENTIFIER(int Line, const string& Filename): IDENTIFIER(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

IDENTIFIER::IDENTIFIER(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Identifier){
  // TODO: Maybe pull constants into a class of their own?
  Constants["e" ].Set_e ();
  Constants["π" ].Set_pi();
  Constants["pi"].Set_pi();
  Constants["i" ].Set_i ();
  Constants["j" ].Set_i ();

  time_t RawTime;
  struct tm* Time;

  time(&RawTime);
  Time = localtime(&RawTime);

  Constants["__YEAR__"   ] = Time->tm_year+1900;
  Constants["__MONTH__"  ] = Time->tm_mon+1;
  Constants["__DAY__"    ] = Time->tm_mday;
  Constants["__HOUR__"   ] = Time->tm_hour;
  Constants["__MINUTE__" ] = Time->tm_min;
  Constants["__SECOND__" ] = Time->tm_sec;

  Constants["__WEEKDAY__"] = ((Time->tm_wday+6)%7)+1;
  Constants["__YEARDAY__"] = Time->tm_yday+1;
}
//------------------------------------------------------------------------------

IDENTIFIER::~IDENTIFIER(){
}
//------------------------------------------------------------------------------

BASE* IDENTIFIER::Copy(bool CopyNext){
  IDENTIFIER* Copy = new IDENTIFIER(Source.Line, Source.Filename.c_str());

  Copy->Name = Name;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool IDENTIFIER::GetConstant(const string& Name, NUMBER* Constant){
  auto Result = Constants.find(Name);
  if(Result == Constants.end()) return false;

  *Constant = Result->second;

  return true;
}
//------------------------------------------------------------------------------

BASE* IDENTIFIER::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool IDENTIFIER::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* IDENTIFIER::Evaluate(){
  EXPRESSION* Result = 0;

  auto NamespaceIterator = NETLIST::NamespaceStack.begin();
  while(!Result && NamespaceIterator != NETLIST::NamespaceStack.end()){
    NETLIST::NAMESPACE* Namespace = *NamespaceIterator;
    while(!Result && Namespace){
      auto Object = Namespace->Symbols.find(this->Name);
      if(Object != Namespace->Symbols.end()){
        if(Object->second){
          switch(Object->second->Type){
            case NETLIST::BASE::TYPE::Alias:{
              auto Alias = (NETLIST::ALIAS*)Object->second;
              NETLIST::NamespaceStack.push_front(Alias->Namespace);
                Result = Alias->Expression->Evaluate();
              NETLIST::NamespaceStack.pop_front();
              break;
            }
            case NETLIST::BASE::TYPE::Pin:
            case NETLIST::BASE::TYPE::Net:{
              auto Synthesisable = (NETLIST::SYNTHESISABLE*)Object->second;
              Result = new OBJECT(Source.Line, Source.Filename);
              ((OBJECT*)Result)->ObjectRef = Synthesisable;
              Synthesisable->Used = true;
              break;
            }
            default:{
              Result = new OBJECT(Source.Line, Source.Filename);
              ((OBJECT*)Result)->ObjectRef = Object->second;
              break;
            }
          }
        }
      }
      Namespace = Namespace->Namespace;
    }
    NamespaceIterator++;
  }
  if(!Result){
    NUMBER Constant;
    if(GetConstant(this->Name.c_str(), &Constant)){
      Result = new LITERAL(Source.Line, Source.Filename);
      ((LITERAL*)Result)->Value = Constant;
    }else{
      Error();
      printf("Identifier \"%s\" not defined\n", this->Name.c_str());
    }
  }
  if(this->Next){
    assert(this->Next->Type > TYPE::Expression);
    Result->Next = ((EXPRESSION*)this->Next)->Evaluate();
  }

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* IDENTIFIER::Simplify(bool GenWire){
  EXPRESSION* Result = Evaluate();

  if(Result != this) delete this;
  return Result;
}
//------------------------------------------------------------------------------

void IDENTIFIER::Display(){
  DisplayStart();

  if(Name.empty()) error ("(Identifier node has no name)");
  else             Debug.print("%s", Name.c_str());

  DisplayEnd();
}
//------------------------------------------------------------------------------

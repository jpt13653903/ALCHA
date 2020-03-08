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

#include "Definition.h"
#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Pin.h"
#include "Netlist/Module.h"
#include "Netlist/Net.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

DEFINITION::ARRAY::ARRAY(){
  Size = 0;
  Next = 0;
}
//------------------------------------------------------------------------------

DEFINITION::ARRAY::ARRAY(const ARRAY& Array){
  Size = 0;
  Next = 0;

  if(Array.Size) Size = (decltype(Array.Size))Array.Size->Copy(true);
  if(Array.Next) Next = new ARRAY(*Array.Next);
}
//------------------------------------------------------------------------------

DEFINITION::ARRAY::~ARRAY(){
  if(Size) delete Size;
  if(Next) delete Next;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::IDENTIFIER(){
  Next        = 0;
  Array       = 0;
  Initialiser = 0;

  Function     = false;
  Parameters   = 0;
  FunctionBody = 0;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::IDENTIFIER(const IDENTIFIER& Identifier){
  Next        = 0;
  Array       = 0;
  Initialiser = 0;

  Function     = Identifier.Function;
  Parameters   = 0;
  FunctionBody = 0;

  if(Identifier.Next        ) Next         = new IDENTIFIER(*Identifier.Next );
  if(Identifier.Array       ) Array        = new ARRAY     (*Identifier.Array);
  if(Identifier.Parameters  ) Parameters   = (decltype(Identifier.Parameters  ))Identifier.Parameters  ->Copy(true);
  if(Identifier.Initialiser ) Initialiser  = (decltype(Identifier.Initialiser ))Identifier.Initialiser ->Copy(true);
  if(Identifier.FunctionBody) FunctionBody = (decltype(Identifier.FunctionBody))Identifier.FunctionBody->Copy(true);
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::~IDENTIFIER(){
  if(Next        ) delete Next;
  if(Array       ) delete Array;
  if(Parameters  ) delete Parameters;
  if(Initialiser ) delete Initialiser;
  if(FunctionBody) delete FunctionBody;
}
//------------------------------------------------------------------------------

DEFINITION::DEFINITION(
  int             Line,
  string&         Filename,
  DEFINITION_TYPE DefinitionType
): DEFINITION(Line, Filename.c_str(), DefinitionType){}
//------------------------------------------------------------------------------

DEFINITION::DEFINITION(
  int             Line,
  const char*     Filename,
  DEFINITION_TYPE DefinitionType
): BASE(Line, Filename, TYPE::Definition){
  this->DefinitionType = DefinitionType;

  Direction = DIRECTION::Inferred;

  ClassName   = 0;
  Parameters  = 0;
  Attributes  = 0;
  Identifiers = 0;
}
//------------------------------------------------------------------------------

DEFINITION::~DEFINITION(){
  if(ClassName  ) delete ClassName;
  if(Parameters ) delete Parameters;
  if(Attributes ) delete Attributes;
  if(Identifiers) delete Identifiers;
}
//------------------------------------------------------------------------------

BASE* DEFINITION::Copy(bool CopyNext){
  DEFINITION* Copy = new DEFINITION(Line, Filename.c_str(), DefinitionType);

  Copy->Direction = Direction;

  if(ClassName  ) Copy->ClassName   = (decltype(ClassName ))ClassName ->Copy(CopyNext);
  if(Parameters ) Copy->Parameters  = (decltype(Parameters))Parameters->Copy(CopyNext);
  if(Attributes ) Copy->Attributes  = (decltype(Attributes))Attributes->Copy(CopyNext);
  if(Identifiers) Copy->Identifiers = new IDENTIFIER(*Identifiers);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool DEFINITION::RunScripting(){
  auto Identifier = Identifiers;

  while(Identifier){
    auto Symbol = NETLIST::NamespaceStack.front()->Symbols.find(Identifier->Identifier);
    if(Symbol != NETLIST::NamespaceStack.front()->Symbols.end()){
      Error();
      printf("Symbol \"%s\" already defined in the current namespace\n",
             Identifier->Identifier.c_str());
      return false;
    }

    if(Identifier->Function){
      error("Not yet implemented");
      Identifier = Identifier->Next;
      continue;
    }
    if(Identifier->Parameters) error("Not yet implemented");

    switch(DefinitionType){
      case DEFINITION_TYPE::Void:
        error("Not yet implemented");
        break;

      case DEFINITION_TYPE::Auto:
        error("Not yet implemented");
        break;

      case DEFINITION_TYPE::Pin:{
        auto Pin = new NETLIST::PIN(Line, Filename, Identifier->Identifier.c_str());
        Pin->Direction = Direction;
        if(!Pin->ApplyParameters(Parameters)) Error("Invalid parameters");
        if(!Pin->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Pin->Name] = Pin;
        if(Identifier->Initialiser){
          if(!Identifier->Initialiser->RunScripting()) return false;
        }
        break;
      }

      case DEFINITION_TYPE::Net:{
        auto Net = new NETLIST::NET(Line, Filename, Identifier->Identifier.c_str());
        Net->Direction = Direction;
        if(!Net->ApplyParameters(Parameters)) Error("Invalid parameters");
        if(!Net->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
        if(Identifier->Initialiser){
          if(!Identifier->Initialiser->RunScripting()) return false;
        }
        break;
      }

      case DEFINITION_TYPE::Byte:{
        auto Byte = new NETLIST::BYTE(Line, Filename, Identifier->Identifier.c_str());
        if(!Byte->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Byte->Name] = Byte;
        if(Identifier->Initialiser){
          if(!Identifier->Initialiser->RunScripting()) return false;
        }
        break;
      }

      case DEFINITION_TYPE::Char:{
        auto Char = new NETLIST::CHARACTER(Line, Filename, Identifier->Identifier.c_str());
        if(!Char->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Char->Name] = Char;
        if(Identifier->Initialiser){
          if(!Identifier->Initialiser->RunScripting()) return false;
        }
        break;
      }

      case DEFINITION_TYPE::Num:{
        auto Number = new NETLIST::NUM(Line, Filename, Identifier->Identifier.c_str());
        if(!Number->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Number->Name] = Number;
        if(Identifier->Initialiser){
          if(!Identifier->Initialiser->RunScripting()) return false;
        }
        break;
      }

      case DEFINITION_TYPE::Func:
        error("Not yet implemented");
        break;

      case DEFINITION_TYPE::ClassInstance:
        error("Not yet implemented");
        break;

      default:
        error("Unknown definition type: %d", (int)DefinitionType);
        return false;
    }
    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

void DEFINITION::Display(){
  DisplayInfo();
  Debug.print("Definition (");

  switch(DefinitionType){
    case DEFINITION_TYPE::Pin : Debug.print("Pin):\n"     ); break;
    case DEFINITION_TYPE::Net : Debug.print("Net):\n"     ); break;
    case DEFINITION_TYPE::Void: Debug.print("Void):\n"    ); break;
    case DEFINITION_TYPE::Auto: Debug.print("Auto):\n"    ); break;
    case DEFINITION_TYPE::Byte: Debug.print("Byte):"      ); break;
    case DEFINITION_TYPE::Char: Debug.print("Character):" ); break;
    case DEFINITION_TYPE::Num : Debug.print("Number):\n"  ); break;
    case DEFINITION_TYPE::Func: Debug.print("Function):\n"); break;

    case DEFINITION_TYPE::ClassInstance:
      Debug.print("Class instance definition (");
      if(ClassName) ClassName->Display();
      else          Debug.print("Class instance with no class name");
      Debug.print("):\n");
      break;

    default: Debug.print("Invalid definition type:\n");
  }

  Debug.print(" Direction = ");
  switch(Direction){
    case DIRECTION::Input : Debug.print("Input\n"   ); break;
    case DIRECTION::Output: Debug.print("Output\n"  ); break;
    default               : Debug.print("Inferred\n"); break;
  }

  Debug.print(" Parameters: ");
  if(Parameters){
    Parameters->Display();
    Debug.print("\n");
  }else{
    Debug.print("none / default\n");
  }

  Debug.print(" Attributes: ");
  if(Attributes){
    Attributes->Display();
    Debug.print("\n");
  }

  Debug.print(" Identifiers:\n");
  IDENTIFIER* Identifier = Identifiers;
  ARRAY     * Array;
  while(Identifier){
    Debug.print(" - %s", Identifier->Identifier.c_str());
    Array = Identifier->Array;
    while(Array){
      Debug.print("[");
      if(Array->Size) Array->Size->Display();
      Debug.print("]");
      Array = Array->Next;
    }

    if(Identifier->Function){
      Debug.print(" -- function:\n  Parameters: (\n");
      if(Identifier->Parameters) Identifier->Parameters->Display();
      Debug.print(" )\n  Body:{\n");
      if(Identifier->FunctionBody) Identifier->FunctionBody->Display();
      Debug.print("  }\n");
    }
    if(Identifier->Initialiser){
      Debug.print(" -- initialiser:");
      Identifier->Initialiser->Display();
    }

    Debug.print("\n");
    Identifier = Identifier->Next;
  }

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------


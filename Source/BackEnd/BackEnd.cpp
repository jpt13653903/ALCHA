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

#include "BackEnd.h"
#include "Ast/Expression/Object.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

BACK_END::BACK_END(){
}
//------------------------------------------------------------------------------

BACK_END::~BACK_END(){
}
//------------------------------------------------------------------------------

void BACK_END::Error(AST::EXPRESSION* Expression, const char* Message){
  ::Error(Expression->Source.Line, Expression->Source.Filename, Message);
}
//------------------------------------------------------------------------------

void BACK_END::Warning(AST::EXPRESSION* Expression, const char* Message){
  ::Warning(Expression->Source.Line, Expression->Source.Filename.c_str(), Message);
}
//------------------------------------------------------------------------------

void BACK_END::PopulateUsed(NAMESPACE* Namespace){
  Debug.Print("Populate used...\n");

  foreach(SymbolIterator, Namespace->Symbols){
    SymbolIterator->second->PopulateUsed(false);

    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Module:
      case BASE::TYPE::Group:
        PopulateUsed((NAMESPACE*)(SymbolIterator->second));
        break;

      default:
        break;
    }
  }
}
//------------------------------------------------------------------------------

bool BACK_END::DeleteUnused(NAMESPACE* Namespace){
  Debug.Print("Delete unused...\n");

  auto SymbolIterator = Namespace->Symbols.begin();

  while(SymbolIterator != Namespace->Symbols.end()){

    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Byte:
      case BASE::TYPE::Character:
      case BASE::TYPE::Number:
      case BASE::TYPE::Alias:{
        auto Object = (SYNTHESISABLE*)(SymbolIterator->second);
        SymbolIterator++;
        Namespace->Symbols.erase(Object->Name);
        delete Object;
        break;
      }

      case BASE::TYPE::Pin:
      case BASE::TYPE::Net:{
        auto Object = (SYNTHESISABLE*)(SymbolIterator->second);
        SymbolIterator++;
        if(!Object->Used){
          Object->Warning(0);
          printf("Deleting unused object %s\n", Object->HDL_Name().c_str());
          Namespace->Symbols.erase(Object->Name);
          delete Object;
        }
        break;
      }

      case BASE::TYPE::Module:
      case BASE::TYPE::Group:{
        auto Object = (NAMESPACE*)(SymbolIterator->second);
        DeleteUnused(Object);
        SymbolIterator++;
        if(Object->Symbols.empty()){
          Object->Warning(0);
          printf("Deleting unused object %s\n", Object->HDL_Name().c_str());
          Namespace->Symbols.erase(Object->Name);
          delete Object;
        }
        break;
      }

      default:
        error("Type %d not handled", (int)SymbolIterator->second->Type);
        SymbolIterator++;
        break;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::AssignPinDirections(NAMESPACE* Namespace){
  Debug.Print("Assign pin directions...\n");

  foreach(SymbolIterator, Namespace->Symbols){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)(SymbolIterator->second);
        if(Pin->Direction == AST::DEFINITION::DIRECTION::Inferred){
          if(Pin->Enabled->Value){ // Possible bidirectional
            if(Pin->Enabled->Value->Type == AST::BASE::TYPE::Literal){
              if(((AST::LITERAL*)Pin->Enabled->Value)->Value == 0){
                Pin->Direction = AST::DEFINITION::DIRECTION::Input;
              }else{
                Pin->Direction = AST::DEFINITION::DIRECTION::Output;
              }
            }else{
              Pin->Direction = AST::DEFINITION::DIRECTION::Bidirectional;
            }
          }else{ // Enabled is undefined
            if(Pin->Driver->Value){
              Pin->Direction = AST::DEFINITION::DIRECTION::Output;
            }else{
              Pin->Direction = AST::DEFINITION::DIRECTION::Input;
            }
          }
        }
        break;
      }
      case BASE::TYPE::Module:
      case BASE::TYPE::Group:
        AssignPinDirections((NAMESPACE*)(SymbolIterator->second));
        break;

      default:
        break;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::RoutePorts(NAMESPACE* Namespace){
  Debug.Print("Route ports...\n");

  // At this point, the expressions use pointers, not names.  Any inter-module
  // usage needs to be broken into temporary signals throughout the hierarchy
  // and assigned to either module ports or internal signals.  Pins need to be
  // routed to be moved to the top-level entity, and the original replaced
  // with HDL module ports.

  // Do the children first
  foreach(SymbolIterator, Namespace->Symbols){
    if(SymbolIterator->second->IsNamespace()){
      RoutePorts((NAMESPACE*)(SymbolIterator->second));
    }
  }

  // If this is the global module, don't go further
  if(!Namespace->Namespace) return true;

  // Route inter-module connections to the parent
  error("Not yet implemented");
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::WriteFile(string& Filename, const char* Ext, string& Body){
  FILE_WRAPPER Files;
  string Fullname = Path + "/" + Filename + "." + Ext;
  return Files.WriteAll(Fullname.c_str(), (const byte*)Body.c_str());
}
//------------------------------------------------------------------------------

bool BACK_END::BuildAssignments(string& Body, NAMESPACE* Namespace){
  foreach(SymbolIterator, Namespace->Symbols){
    auto Object = SymbolIterator->second;
    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(Pin->Driver->Value){
          string Driver;
          Body += "// " + Pin->Driver->Value->Source.Filename +" +"+ to_string(Pin->Driver->Value->Source.Line) + "\n";
          if(!Pin->Driver->Value->GetVerilog(Driver)) return false;
          if(Pin->Enabled->Value){
            string Enabled;
            Body += "// " + Pin->Enabled->Value->Source.Filename +" +"+ to_string(Pin->Enabled->Value->Source.Line) + "\n";
            if(!Pin->Enabled->Value->GetVerilog(Enabled)) return false;
            Body += "assign "+ Pin->EscapedName() +
                    " = |("+ Enabled + ")"
                    " ? ("+ Driver + ")"
                    " : " + to_string(Pin->Width()) + "'bZ;\n\n";
          }else{
            Body += "assign "+ Pin->EscapedName() +" = "+ Driver + ";\n\n";
          }
        }
        break;
      }
      case BASE::TYPE::Net:{
        auto Net = (NET*)Object;
        if(Net->Value){
          string Value;
          Body += "// " + Net->Value->Source.Filename +" +"+ to_string(Net->Value->Source.Line) + "\n";
          if(!Net->Value->GetVerilog(Value)) return false;
          Body += "assign "+ Net->EscapedName() +" = "+ Value +";\n";
        }
        break;
      }
      case BASE::TYPE::Group:{
        if(!BuildAssignments(Body, (NAMESPACE*)Object)) return false;
        break;
      }
      default:
        break;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

void BACK_END::BuildSizeDef(string& Body, int Width, bool Signed){
  int Top;
  if(Signed) Top = Width;
  else       Top = Width-1;

  if(Top > 0){
    Body += "[";
    if(Top < 100) Body += ' ';
    if(Top <  10) Body += ' ';
    Body += to_string(Top) +":0]";
  }else{
    Body += "       ";
  }
}
//------------------------------------------------------------------------------

void BACK_END::BuildPorts(string& Body, NAMESPACE* Namespace, bool& isFirst){
  foreach(SymbolIterator, Namespace->Symbols){
    auto Object = SymbolIterator->second;
    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(!isFirst) Body += ",\n";
        isFirst = false;

        switch(Pin->Direction){
          case AST::DEFINITION::DIRECTION::Input : Body += "  input  logic "; break;
          case AST::DEFINITION::DIRECTION::Output: Body += "  output logic "; break;
          default                                : Body += "  inout  logic "; break;
        }
        BuildSizeDef(Body, Pin->Width(), Pin->Signed());
        Body += Pin->EscapedName();
        break;
      }
      case BASE::TYPE::Group:{
        BuildPorts(Body, (NAMESPACE*)Object, isFirst);
        break;
      }
      default:
        break;
    }
  }
}
//------------------------------------------------------------------------------

void BACK_END::BuildNets(string& Body, NAMESPACE* Namespace){
  foreach(SymbolIterator, Namespace->Symbols){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Net:{
        auto Net = (NET*)SymbolIterator->second;
        Body += "logic ";
        BuildSizeDef(Body, Net->Width(), Net->Signed());
        Body += Net->EscapedName() + ";\n";
        break;
      }
      case BASE::TYPE::Group:{
        BuildNets(Body, (NAMESPACE*)SymbolIterator->second);
        break;
      }
      default:
        break;
    }
  }
}
//------------------------------------------------------------------------------

bool BACK_END::BuildHDL(MODULE* Module, string Path){
  bool isGlobal = (Module == &Global);

  // Recursively generate the modules (each namespace is a module)
  foreach(SymbolIterator, Module->Symbols){
    auto Symbol = SymbolIterator->second;
    if(Symbol->Type == BASE::TYPE::Module){
      auto Child = (MODULE*)Symbol;
      if(isGlobal) BuildHDL(Child, "source");
      else         BuildHDL(Child, Path + "/" + Module->HDL_Name());
    }
  }
  // Generate this module's name
  string Name;
  if(isGlobal) Name = Filename;
  else         Name = Module->EscapedName();

  // Header
  string Body;
  Body = "// Auto-generated by ALCHA "
         "Version "+ to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +" ("
         "Built on " __DATE__ " at " __TIME__ ")\n"
         "//--------------------------------------"
         "----------------------------------------\n\n";

  // Module Definition
  Body += "module "+ Name +"(\n";

  // Ports
  bool isFirst = true;
  BuildPorts(Body, Module, isFirst);
  if(!isFirst) Body += "\n";
  Body += ");\n";
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  // Nets
  BuildNets(Body, Module);
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  // Assignments
  if(!BuildAssignments(Body, Module)) return false;

  Body += "//--------------------------------------"
          "----------------------------------------\n\n";
  Body += "endmodule\n";
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  if(isGlobal) Name = Filename;
  else         Name = Path + "/" + Module->Name;
  WriteFile(Name, "v", Body);

  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::BuildAltera(const char* Path, const char* Filename){
  this->Path     = Path;
  this->Filename = Filename;

  Debug.Print(
    ANSI_FG_GREEN "\nStarting BackEnd -----------------------"
                  "----------------------------------------\n\n"
    ANSI_RESET
  );

  PopulateUsed(&Global);
  Debug.Print("\n");

  if(!DeleteUnused(&Global)) return false;
  Debug.Print("\n");

  if(!AssignPinDirections(&Global)) return false;
  Debug.Print("\n");

  if(!RoutePorts(&Global)) return false;
  Debug.Print("\n");

  Global.Display();

  Debug.Print(
    ANSI_FG_GREEN "\nBuilding Project -----------------------"
                  "----------------------------------------\n\n"
    ANSI_RESET
  );

  ALTERA::PROJECT Project;
  Project.Build(Path, Filename);

  if(!BuildHDL(&Global, "")) return false;

  return true;
}
//------------------------------------------------------------------------------


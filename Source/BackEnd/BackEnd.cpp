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
//------------------------------------------------------------------------------

using namespace std;
using namespace OBJECTS;
//------------------------------------------------------------------------------

BACK_END::BACK_END(){
}
//------------------------------------------------------------------------------

BACK_END::~BACK_END(){
}
//------------------------------------------------------------------------------

bool BACK_END::DeleteUnused(NAMESPACE* Namespace){
  info("Delete unused...");

  auto SymbolIterator = Namespace->Symbols.begin();

  while(SymbolIterator != Namespace->Symbols.end()){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:
      case BASE::TYPE::Net:
      case BASE::TYPE::Synthesisable:{
        auto Object = (SYNTHESISABLE*)(SymbolIterator->second);
        SymbolIterator++;
        if(!Object->Used){
          Namespace->Symbols.erase(Object->Name);
          delete Object;
        }
        break;
      }

      case BASE::TYPE::Module:
      case BASE::TYPE::Group:
      case BASE::TYPE::Namespace:{
        auto Object = (NAMESPACE*)(SymbolIterator->second);
        DeleteUnused(Object);
        SymbolIterator++;
        if(Object->Symbols.empty()){
          Namespace->Symbols.erase(Object->Name);
          delete Object;
        }
        break;
      }

      default:
        SymbolIterator++;
        break;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::AssignPinDirections(NAMESPACE* Namespace){
  info("Assign pin directions...");

  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)(SymbolIterator->second);
        if(Pin->Direction == AST::DEFINITION::Inferred){
          if(Pin->Enabled){ // Possible bidirectional
            if(Pin->Enabled->ExpressionType == EXPRESSION::Literal){
              if(Pin->Enabled->Value == 0){
                Pin->Direction = AST::DEFINITION::Input;
              }else{
                Pin->Direction = AST::DEFINITION::Output;
              }
            }else{
              Pin->Direction = AST::DEFINITION::Bidirectional;
            }
          }else{ // Enabled is undefined
            if(Pin->Driver){
              Pin->Direction = AST::DEFINITION::Output;
            }else{
              Pin->Direction = AST::DEFINITION::Input;
            }
          }
        }
        break;
      }
      case BASE::TYPE::Module:
      case BASE::TYPE::Group:
      case BASE::TYPE::Namespace:
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
  info("Route ports...");

  // At this point, the expressions use pointers, not names.  Any inter-module 
  // usage needs to be broken into temporary signals throughout the hierarchy 
  // and assigned to either module ports or internal signals.  Pins need to be 
  // routed to be moved to the top-level entity, and the original replaced 
  // with HDL module ports.

  // Do the children first
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Module:
      case BASE::TYPE::Group:
      case BASE::TYPE::Namespace:
        RoutePorts((NAMESPACE*)(SymbolIterator->second));
        break;

      default:
        break;
    }
  }

  // If this is the global module, don't go further
  if(!Namespace->Namespace) return true;

  // After the children routed upwards, route further
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)(SymbolIterator->second);
        warning("Pin hierarchical routing not yet implemented");
        break;
      }
      case BASE::TYPE::Net:{
        auto Net = (NET*)(SymbolIterator->second);
        warning("Net hierarchical routing not yet implemented");
        break;
      }
      default:
        break;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::WriteFile(string& Filename, const char* Ext, string& Body){
  FILE_WRAPPER Files;
  string Fullname = Path + "/" + Filename + "." + Ext;
  return Files.WriteAll(Fullname.c_str(), (const byte*)Body.c_str());
}
//------------------------------------------------------------------------------

bool BACK_END::BuildExpression(string& Body, EXPRESSION* Expression){
  if(!Expression) return false;

  switch(Expression->ExpressionType){
    case EXPRESSION::Literal:
      if(!Expression->Value.IsReal()){
        error("non-real literal");
        return false;
      }
      Body += to_string((unsigned)Expression->Value.IsReal());
      break;

    case EXPRESSION::Object:
      if(!Expression->ObjectRef){
        error("Null object reference");
        return false;
      }
      Body += Expression->ObjectRef->Name;
      break;

    case EXPRESSION::VectorConcatenate:
      Body += "{";
      for(size_t n = 0; n < Expression->Elements.size(); n++){
        Body += "(";
        if(!BuildExpression(Body, Expression->Elements[n])) return false;
        Body += ")";
        if(n < Expression->Elements.size()-1) Body += ", ";
      }
      Body += "}";
      break;

    case EXPRESSION::Slice:
      error("Not yet implemented");
      break;

    case EXPRESSION::Negate:
      Body += "-(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Bit_NOT:
      Body += "~(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::AND_Reduce:
      Body += "&(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::NAND_Reduce:
      Body += "~&(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::OR_Reduce:
      Body += "|(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::NOR_Reduce:
      Body += "~|(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::XOR_Reduce:
      Body += "^(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::XNOR_Reduce:
      Body += "~^(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Logical_NOT:
      Body += "!(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Replicate:
      Body += "{(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += "){";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += "}}";
      break;

    case EXPRESSION::Multiply:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")*(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Add:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")+(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Subtract:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")-(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Shift_Left:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")<<(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Shift_Right:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")>>(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Less:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")<(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Greater:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")>(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Less_Equal:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")<=(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Greater_Equal:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")>=(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Equal:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")==(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Not_Equal:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")!=(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Bit_AND:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")&(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Bit_NAND:
      Body += "~((";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")&(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += "))";
      break;

    case EXPRESSION::Bit_OR:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")|(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Bit_NOR:
      Body += "~((";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")|(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += "))";
      break;

    case EXPRESSION::Bit_XOR:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")^(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Bit_XNOR:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")~^(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Logical_AND:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")&&(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Logical_OR:
      Body += "(";
      if(!BuildExpression(Body, Expression->Left)) return false;
      Body += ")||(";
      if(!BuildExpression(Body, Expression->Right)) return false;
      Body += ")";
      break;

    case EXPRESSION::Conditional:
      error("Not yet implemented");
      break;

    default:
      error("Unexpected expression type");
      break;
  }
  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::BuildHDL(MODULE* Module, string Path){
  bool isGlobal = (Module == &Global);

  // Recursively generate the modules (each namespace is a module)
  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
    auto Symbol = SymbolIterator->second;
    if(Symbol->Type == BASE::TYPE::Module){
      auto Child = (MODULE*)Symbol;
      if(isGlobal) BuildHDL(Child, "source");
      else         BuildHDL(Child, Path + "/" + Module->Name);
    }
  }
  // Generate this module's name
  string Name;
  if(isGlobal) Name = Filename;
  else         Name = Path + "/" + Module->Name;

  string Body;
  Body += "module "+ Name +"(\n";
  // Ports
  bool isFirst = true;
  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
    if(SymbolIterator->second->Type == BASE::TYPE::Pin){
      auto Pin = (PIN*)SymbolIterator->second;
      if(!isFirst) Body += ",\n";
      isFirst = false;

      switch(Pin->Direction){
        case AST::DEFINITION::Input : Body += "  input  "; break;
        case AST::DEFINITION::Output: Body += "  output "; break;
        default                     : Body += "  inout  "; break;
      }
      if(Pin->Width > 1) Body += "["+ to_string(Pin->Width-1) +":0]";
      Body += Pin->Name;
    }
  }
  if(!isFirst) Body += "\n";
  Body += ");\n";
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  // Body
  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
    if(SymbolIterator->second->Type == BASE::TYPE::Pin){
      auto Pin = (PIN*)SymbolIterator->second;
      if(Pin->Driver){
        Body += "assign "+ Pin->Name +" = ";
        if(!BuildExpression(Body, Pin->Driver)) return false;
        Body += ";\n";
      }
    }
  }

  Body += "//--------------------------------------"
          "----------------------------------------\n\n";
  Body += "endmodule\n";
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  WriteFile(Name, "v", Body);

  return true;
}
//------------------------------------------------------------------------------

bool BACK_END::BuildAltera(const char* Path, const char* Filename){
  this->Path     = Path;
  this->Filename = Filename;

  printf(
    ANSI_FG_GREEN "\nStarting BackEnd -----------------------"
                  "----------------------------------------\n\n"
    ANSI_RESET
  );
  Global.Display();

  if(!DeleteUnused(&Global)) return false;

  if(!AssignPinDirections(&Global)) return false;

  if(!RoutePorts(&Global)) return false;

  printf(
    ANSI_FG_GREEN "\nBuilding Project -----------------------"
                  "----------------------------------------\n\n"
    ANSI_RESET
  );
  Global.Display();

  ALTERA::PROJECT Project;
  Project.Build(Path, Filename);
  
  if(!BuildHDL(&Global, "")) return false;

  return true;
}
//------------------------------------------------------------------------------


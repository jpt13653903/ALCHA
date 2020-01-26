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
using namespace NETLIST;
//------------------------------------------------------------------------------

BACK_END::BACK_END(){
  error = false;
}
//------------------------------------------------------------------------------

BACK_END::~BACK_END(){
}
//------------------------------------------------------------------------------

void BACK_END::Error(EXPRESSION* Expression, const char* Message){
  if(error) return;
  error = true;
  printf(
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_BRIGHT_RED   "  Error: "
    ANSI_RESET,
    Expression->Line,
    Expression->Filename.c_str()
  );
  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

void BACK_END::Warning(EXPRESSION* Expression, const char* Message){
  printf(
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_MAGENTA      "  Warning: "
    ANSI_RESET,
    Expression->Line,
    Expression->Filename.c_str()
  );
  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

bool BACK_END::DeleteUnused(NAMESPACE* Namespace){
  info("Delete unused...");

  auto SymbolIterator = Namespace->Symbols.begin();

  while(SymbolIterator != Namespace->Symbols.end()){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Byte:
      case BASE::TYPE::Character:
      case BASE::TYPE::Number:{
        auto Object = (SYNTHESISABLE*)(SymbolIterator->second);
        SymbolIterator++;
        Namespace->Symbols.erase(Object->Name);
        delete Object;
        break;
      }

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
    if(SymbolIterator->second->Type == BASE::TYPE::Module ||
       SymbolIterator->second->Type == BASE::TYPE::Group  ){
      RoutePorts((MODULE*)(SymbolIterator->second));
    }
  }

  // If this is the global module, don't go further
  if(!Namespace->Namespace) return true;

  // Route inter-module connections to the parent
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
    case EXPRESSION::Literal:{
      if(!Expression->Value.IsReal()){
        Error(Expression, "non-real literal");
        return false;
      }
      NUMBER Result(Expression->Value);
      if(!Result.IsPositive()){
        if(!Expression->Signed){
          Error(Expression, "Cannot store a negative literal to an unsigned target");
          return false;
        }
        Body += "-";
        Result.Mul(-1);
      }
      if(Expression->Signed) Body += to_string(Expression->Width+1) + "'h";
      else                   Body += to_string(Expression->Width  ) + "'h";
      Result.Round();
      Body += Result.GetString(16);
      Result.BinScale(-Expression->Width);
      if(Result > 1){
        Error(Expression, "The literal does not fit in its full-scale range");
        return false;
      }
      break;
    }

    case EXPRESSION::Object:
      if(!Expression->ObjectRef){
        error("Null object reference");
        return false;
      }
      Body += Expression->ObjectRef->EscapedName();
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

    case EXPRESSION::Cast:{
      if(!Expression->Left){
        error("Unexpected null reference");
        return false;
      }
      if(Expression->Right){
        error("Unexpected cast to class");
        return false;
      }
      EXPRESSION* From = Expression->Left;
      EXPRESSION* To   = Expression;
      NUMBER Factor = From->FullScale;
      Factor.Div(To->FullScale);
      Factor.BinScale(To->Width - From->Width);
      if(Factor == 0){
        error("Unexpected 0 full-scale");
        return false;
      }

      NUMBER Limit(1);
      Limit.BinScale(To->Width);
      int Shift = 0;
      while(Factor.IsInt()){
        Factor.BinScale(-1);
        Shift--;
      }
      while(!Factor.IsInt() && (Factor < Limit)){
        Factor.BinScale(1);
        Shift++;
      }
      while(Factor >= Limit){
        Factor.BinScale(-1);
        Shift--;
      }
      Factor.Round();
      int Width = 0;
      NUMBER Num(Factor);
      while(Num >= 1){
        Num.BinScale(-1);
        Width++;
      }

      if(Factor == 1){
        if(Shift) Body += "(";
        if(!BuildExpression(Body, Expression->Left)) return false;
        if     (Shift > 0) Body += ") >> " + to_string( Shift);
        else if(Shift < 0) Body += ") << " + to_string(-Shift);
      }else{
        Warning(Expression, "Non power-of-two scaling factor: synthesising a multiplier");
        if(Shift == 0) Body += "(";
        else           Body += "((";
        if(!BuildExpression(Body, Expression->Left)) return false;
        Body += ")*" + to_string(Width) + "'h";
        Body += Factor.GetString(16);
        if     (Shift > 0) Body += ") >> " + to_string( Shift);
        else if(Shift < 0) Body += ") << " + to_string(-Shift);
      }
      break;
    }

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

bool BACK_END::AddAssignment(string& Body, BASE* Object){
  switch(Object->Type){
    case BASE::TYPE::Pin:{
      auto Pin = (PIN*)Object;
      if(Pin->Driver){
        Body += "assign "+ Pin->EscapedName() +" = ";
        if(Pin->Enabled){
          Body += "(";
          if(!BuildExpression(Body, Pin->Enabled)) return false;
          Body += ") ? (";
          if(!BuildExpression(Body, Pin->Driver)) return false;
          Body += ") : " + to_string(Pin->Width) + "'bZ";
        }else{
          if(!BuildExpression(Body, Pin->Driver)) return false;
        }
        Body += ";\n";
      }
      break;
    }
    case BASE::TYPE::Net:{
      auto Net = (NET*)Object;
      if(Net->Value){
        Body += "assign "+ Net->EscapedName() +" = ";
        if(!BuildExpression(Body, Net->Value)) return false;
        Body += ";\n";
      }
      break;
    }
    case BASE::TYPE::Group:{
      auto Namespace = (NAMESPACE*)Object;
      for(auto SymbolIterator  = Namespace->Symbols.begin();
               SymbolIterator != Namespace->Symbols.end  ();
               SymbolIterator++){
        if(!AddAssignment(Body, SymbolIterator->second)) return false;
      }
      break;
    }
    default:
      break;
  }
  return true;
}
//------------------------------------------------------------------------------

void BACK_END::BuildPorts(string& Body, NAMESPACE* Namespace, bool& isFirst){
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)SymbolIterator->second;
        if(!isFirst) Body += ",\n";
        isFirst = false;

        switch(Pin->Direction){
          case AST::DEFINITION::Input : Body += "  input  "; break;
          case AST::DEFINITION::Output: Body += "  output "; break;
          default                     : Body += "  inout  "; break;
        }
        if(Pin->Width > 1){
          if(Pin->Signed) Body += "["+ to_string(Pin->Width  ) +":0]";
          else            Body += "["+ to_string(Pin->Width-1) +":0]";
        }
        Body += Pin->EscapedName();
        break;
      }
      case BASE::TYPE::Group:{
        BuildPorts(Body, (NAMESPACE*)SymbolIterator->second, isFirst);
        break;
      }
      default:
        break;
    }
  }
}
//------------------------------------------------------------------------------

void BACK_END::BuildNets(string& Body, NAMESPACE* Namespace){
  bool isFirst = true;

  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Net:{
        if(!isFirst) Body += "\n";
        isFirst = false;
        auto Net = (NET*)SymbolIterator->second;
        Body += "wire ";
        if(Net->Width > 1){
          if(Net->Signed) Body += "["+ to_string(Net->Width  ) +":0]";
          else            Body += "["+ to_string(Net->Width-1) +":0]";
        }
        Body += Net->EscapedName() + ";";
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
  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
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

  string Body;
  Body += "module "+ Name +"(\n";

  // Ports
  bool isFirst = true;
  BuildPorts(Body, Module, isFirst);
  if(!isFirst) Body += "\n";
  Body += ");\n";
  Body += "//--------------------------------------"
          "----------------------------------------\n\n";

  // Nets
  BuildNets (Body, Module);
  Body += "\n//--------------------------------------"
          "----------------------------------------\n\n";

  // Body
  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
    if(!AddAssignment(Body, SymbolIterator->second)) return false;
  }

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

  Debug.print(
    ANSI_FG_GREEN "\nStarting BackEnd -----------------------"
                  "----------------------------------------\n\n"
    ANSI_RESET
  );
  Global.Display();

  if(!DeleteUnused(&Global)) return false;

  if(!AssignPinDirections(&Global)) return false;

  if(!RoutePorts(&Global)) return false;

  Debug.print(
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


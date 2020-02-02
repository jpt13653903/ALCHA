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
}
//------------------------------------------------------------------------------

BACK_END::~BACK_END(){
}
//------------------------------------------------------------------------------

void BACK_END::Error(EXPRESSION* Expression, const char* Message){
  ::Error(Expression->Line, Expression->Filename, Message);
}
//------------------------------------------------------------------------------

void BACK_END::Warning(EXPRESSION* Expression, const char* Message){
  ::Warning(Expression->Line, Expression->Filename.c_str(), Message);
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

const char* BACK_END::GetTemporaryName(){
  static unsigned Count = 0;
  static char     Name[0x10];
  sprintf(Name, "\\t..%d ", Count++);
  return Name;
}
//------------------------------------------------------------------------------

// TODO: Rename "Temporary" -- it can also return the literal, for instance
bool BACK_END::BuildExpression(string& Body, EXPRESSION* Expression, string& Temporary){
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
        Temporary += "-";
        Result.Mul(-1);
      }
      if(Expression->Signed) Temporary += to_string(Expression->Width+1) + "'h";
      else                   Temporary += to_string(Expression->Width  ) + "'h";
      Result.Round();
      Temporary += Result.GetString(16);
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
      Temporary += Expression->ObjectRef->EscapedName();
      break;

    case EXPRESSION::VectorConcatenate:{
      vector<string> Elements;
      Elements.resize(Expression->Elements.size());

      for(size_t n = 0; n < Expression->Elements.size(); n++){
        if(!BuildExpression(Body, Expression->Elements[n], Elements[n])) return false;
      }
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= {";
      for(size_t n = 0; n < Expression->Elements.size(); n++){
        Body += Elements[n];
        if(n < Expression->Elements.size()-1) Body += ", ";
      }
      Body += "};\n";
      break;
    }

    case EXPRESSION::Slice:
      error("Not yet implemented");
      break;

    case EXPRESSION::Negate:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= -("+ Right +");\n";
      break;
    }

    case EXPRESSION::Bit_NOT:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= ~("+ Right +");\n";
      break;
    }

    case EXPRESSION::AND_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= &("+ Right +");\n";
      break;
    }

    case EXPRESSION::NAND_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= ~&("+ Right +");\n";
      break;
    }

    case EXPRESSION::OR_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= |("+ Right +");\n";
      break;
    }

    case EXPRESSION::NOR_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= ~|("+ Right +");\n";
      break;
    }

    case EXPRESSION::XOR_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= ^("+ Right +");\n";
      break;
    }

    case EXPRESSION::XNOR_Reduce:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= ~^("+ Right +");\n";
      break;
    }

    case EXPRESSION::Logical_NOT:{
      string Right;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire ";
      Body += Temporary +"= !("+ Right +");\n";
      break;
    }

    case EXPRESSION::Replicate:{ // TODO: Test
      string Left, Right;

      if(Expression->Right->ExpressionType != EXPRESSION::Literal){
        Error(Expression, "Replication count must break down to a run-time constant");
        return false;
      }
      if(!Expression->Right->Value.IsInt()){
        Error(Expression, "Replication count must be an integer");
        return false;
      }
      if(!Expression->Right->Value.IsPositive()){
        Error(Expression, "Replication count must be real and positive");
        return false;
      }
      if(!BuildExpression(Body, Expression->Left, Left)) return false;
      Right = Expression->Right->Value.GetString(10);

      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= {"+ Right +"{"+ Left +"}};\n";
      break;
    }

    case EXPRESSION::Multiply:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" * "+ Right +";\n";
      break;
    }

    case EXPRESSION::Add:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" + "+ Right +";\n";
      break;
    }

    case EXPRESSION::Subtract:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" - "+ Right +";\n";
      break;
    }

    case EXPRESSION::Shift_Left:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" << "+ Right +";\n";
      break;
    }

    case EXPRESSION::Shift_Right:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" >> "+ Right +";\n";
      break;
    }

    case EXPRESSION::Less:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" < "+ Right +";\n";
      break;
    }

    case EXPRESSION::Greater:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" > "+ Right +";\n";
      break;
    }

    case EXPRESSION::Less_Equal:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" <= "+ Right +";\n";
      break;
    }

    case EXPRESSION::Greater_Equal:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" >= "+ Right +";\n";
      break;
    }

    case EXPRESSION::Equal:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" == "+ Right +";\n";
      break;
    }

    case EXPRESSION::Not_Equal:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" != "+ Right +";\n";
      break;
    }

    case EXPRESSION::Bit_AND:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" & "+ Right +";\n";
      break;
    }

    case EXPRESSION::Bit_NAND:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= ~("+ Left +" & "+ Right +");\n";
      break;
    }

    case EXPRESSION::Bit_OR:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" | "+ Right +";\n";
      break;
    }

    case EXPRESSION::Bit_NOR:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= ~("+ Left +" | "+ Right +");\n";
      break;
    }

    case EXPRESSION::Bit_XOR:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" ^ "+ Right +";\n";
      break;
    }

    case EXPRESSION::Bit_XNOR:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      if(Expression->Width > 1) Body += "wire ["+ to_string(Expression->Width - 1) +":0] ";
      else                      Body += "wire ";
      Body += Temporary +"= "+ Left +" ~^ "+ Right +";\n";
      break;
    }

    case EXPRESSION::Logical_AND:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" && "+ Right +";\n";
      break;
    }

    case EXPRESSION::Logical_OR:{
      string Left, Right;
      if(!BuildExpression(Body, Expression->Left , Left )) return false;
      if(!BuildExpression(Body, Expression->Right, Right)) return false;
      Temporary = GetTemporaryName();
      Body += "wire "+ Temporary +"= "+ Left +" || "+ Right +";\n";
      break;
    }

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

      // Calculate the limit of the inferred multiplier size.  Most FPGAs have 
      // 18-bit multipliers, so make that the minimum limit, otherwise use the 
      // target width as the limit so that no to little resolution is lost.
      NUMBER Limit(1);
      if(To->Width < 18) Limit.BinScale(18);
      else               Limit.BinScale(To->Width);
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
      NUMBER FullFactor(Factor);
      Factor.Round();
      if(Factor != FullFactor){
        Warning(Expression, "Rounding the scaling factor - this can be fixed "
                            "with an explicit scaling multiplication.");
        while(Factor.IsInt()){ // Make sure it's still minimised after rounding
          Factor.BinScale(-1);
          Shift--;
        }
        while(!Factor.IsInt()){
          Factor.BinScale(1);
          Shift++;
        }
      }

      int Width = 0;
      NUMBER Num(Factor);
      while(Num >= 1){
        Num.BinScale(-1);
        Width++;
      }

      string FromString;
      if(!BuildExpression(Body, From, FromString)) return false;

      if(Factor == 1){
        Body += "wire ";
        if(To->Width > 1){
          if(To->Signed) Body += "["+ to_string(To->Width  ) +":0] ";
          else           Body += "["+ to_string(To->Width-1) +":0] ";
        }
        Temporary = GetTemporaryName();
        Body += Temporary +"= ";
        if     (Shift > 0) Body += FromString +" >> "+ to_string( Shift);
        else if(Shift < 0) Body += FromString +" << "+ to_string(-Shift);
        Body += ";\n";

      }else{
        Warning(Expression, "Non power-of-two scaling factor: synthesising a multiplier");
        string MulTemporaryName = GetTemporaryName();

        // TODO: Signed
        Body += "wire ["+ to_string(From->Width + Width - 1) +":0] ";
        Body += MulTemporaryName +"= "+ FromString + " * ";

        Body += to_string(Width) + "'h";
        Body += Factor.GetString(16);
        Body += ";\n";

        Body += "wire ";
        if(To->Width > 1){
          if(To->Signed) Body += "["+ to_string(To->Width  ) +":0] ";
          else           Body += "["+ to_string(To->Width-1) +":0] ";
        }
        Temporary = GetTemporaryName();
        Body += Temporary +"= ";
        if     (Shift > 0) Body += MulTemporaryName +" >> "+ to_string( Shift);
        else if(Shift < 0) Body += MulTemporaryName +" << "+ to_string(-Shift);
        Body += ";\n";
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

bool BACK_END::BuildAssignments(string& Body, NAMESPACE* Namespace){
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    auto Object = SymbolIterator->second;
    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(Pin->Driver){
          string Driver;
          Body += "// " + Pin->Driver->Filename +" +"+ to_string(Pin->Driver->Line) + "\n";
          if(!BuildExpression(Body, Pin->Driver, Driver)) return false;
          if(Pin->Enabled){
            string Enabled;
            Body += "// " + Pin->Enabled->Filename +" +"+ to_string(Pin->Enabled->Line) + "\n";
            if(!BuildExpression(Body, Pin->Enabled, Enabled)) return false;
            Body += "assign "+ Pin->EscapedName() +
                    " = |("+ Enabled + ")"
                    " ? ("+ Driver + ")"
                    " : " + to_string(Pin->Width) + "'bZ;\n\n";
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
          Body += "// " + Net->Value->Filename +" +"+ to_string(Net->Value->Line) + "\n";
          if(!BuildExpression(Body, Net->Value, Value)) return false;
          Body += "assign "+ Net->EscapedName() +" = "+ Value +";\n\n";
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

void BACK_END::BuildPorts(string& Body, NAMESPACE* Namespace, bool& isFirst){
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    auto Object = SymbolIterator->second;
    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(!isFirst) Body += ",\n";
        isFirst = false;

        switch(Pin->Direction){
          case AST::DEFINITION::Input : Body += "  input  logic "; break;
          case AST::DEFINITION::Output: Body += "  output logic "; break;
          default                     : Body += "  inout  logic "; break;
        }
        if(Pin->Width > 1){
          if(Pin->Signed) Body += "["+ to_string(Pin->Width  ) +":0]";
          else            Body += "["+ to_string(Pin->Width-1) +":0]";
        }
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
  for(auto SymbolIterator  = Namespace->Symbols.begin();
           SymbolIterator != Namespace->Symbols.end  ();
           SymbolIterator++){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Net:{
        auto Net = (NET*)SymbolIterator->second;
        Body += "logic ";
        if(Net->Width > 1){
          if(Net->Signed) Body += "["+ to_string(Net->Width  ) +":0]";
          else            Body += "["+ to_string(Net->Width-1) +":0]";
        }
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


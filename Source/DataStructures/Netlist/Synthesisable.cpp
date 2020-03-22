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

#include "Synthesisable.h"
#include "Ast/Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

SYNTHESISABLE::SYNTHESISABLE(int Line, const std::string& Filename, const char* Name, TYPE Type) : BASE(Line, Filename, Name, Type){
  Used      = false;
  Signed    = false;
  Width     = 1;
  FullScale = 2;
}
//------------------------------------------------------------------------------

SYNTHESISABLE::~SYNTHESISABLE(){
}
//------------------------------------------------------------------------------

bool SYNTHESISABLE::IsSynthesisable(){
  return true;
}
//------------------------------------------------------------------------------

bool SYNTHESISABLE::ApplyParameters(list<AST::BASE*>& Parameters){
  int  Position          = 0; // Negative => named parameters
  bool ExplicitFullScale = false;

  foreach(Parameter, Parameters){
    if((*Parameter)->IsExpression()){
      if(Position < 0) return false; // Mixing named and positional parameters

      AST::EXPRESSION* Param = ((AST::EXPRESSION*)(*Parameter))->Evaluate();
      if(!Param){
        (*Parameter)->Error("Invalid parameter expression");
        return false;
      }

      switch(Param->Type){
        case AST::BASE::TYPE::Literal:{
          auto Literal = (AST::LITERAL*)Param;
          switch(Position){
            case 0:
              if(!Literal->Value.IsInt()) return false;
              Width = round(Literal->Value.GetReal());
              break;

            case 1:
              ExplicitFullScale = true;
              FullScale = Literal->Value;
              break;

            default: // Too many parameters
              delete Literal;
              return false;
          }
          break;
        }

        default:
          (*Parameter)->Error("Parameters must be pure scripting expressions");
          delete Param;
          return false;
      }
      delete Param;

    }else if((*Parameter)->Type == AST::BASE::TYPE::Assignment){
      // auto Param = (AST::EXPRESSION*)(*Parameter);
      Position = -1;
      error("Not yet implemented");

    }else{
      return false;
    }
    if(Position >= 0) Position++;
  }
  if(Width < 0){
    Width *= -1;
    Signed = true;
  }
  if(!ExplicitFullScale){
    FullScale = pow(2.0, Width);
  }
  if(FullScale.IsReal() && (FullScale.GetReal() < 0)){
    FullScale.Mul(-1);
    Signed = true;
  }
  return true;
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::DisplayParameters(){
  Debug.print("%s\n", Name.c_str());
  Debug.print("    Used       = %s\n", Used   ? "true" : "false");
  Debug.print("    Width      = %u\n", Width);
  Debug.print("    Full-scale = %s\n", FullScale.Display());
  Debug.print("    Signed     = %s\n", Signed ? "true" : "false");

  Debug.print("    Direction  = ");
  switch(Direction){
    case AST::DEFINITION::DIRECTION::Inferred     : Debug.print("Inferred\n"     ); break;
    case AST::DEFINITION::DIRECTION::Input        : Debug.print("Input\n"        ); break;
    case AST::DEFINITION::DIRECTION::Output       : Debug.print("Output\n"       ); break;
    case AST::DEFINITION::DIRECTION::Bidirectional: Debug.print("Bidirectional\n"); break;
    default                                       : Debug.print("Invalid\n"      ); break;
  }
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::Validate(){
  BASE::Validate();
}
//------------------------------------------------------------------------------


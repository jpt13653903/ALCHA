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
  Used = false;

  Format.Width     = new NUM(Line, Filename, "Width"    );
  Format.FullScale = new NUM(Line, Filename, "FullScale");
  Format.Signed    = new NUM(Line, Filename, "Signed"   );

  Format.Width    ->Value = 1;
  Format.FullScale->Value = 2;
  Format.Signed   ->Value = 0;
}
//------------------------------------------------------------------------------

SYNTHESISABLE::~SYNTHESISABLE(){
  delete Format.Width;
  delete Format.FullScale;
  delete Format.Signed;
}
//------------------------------------------------------------------------------

bool SYNTHESISABLE::IsSynthesisable(){
  return true;
}
//------------------------------------------------------------------------------

int SYNTHESISABLE::Width(){
  return Format.Width->Value.GetReal();
}
//------------------------------------------------------------------------------

NUMBER& SYNTHESISABLE::FullScale(){
  return Format.FullScale->Value;
}
//------------------------------------------------------------------------------

bool SYNTHESISABLE::Signed(){
  return Format.Signed->Value != 0;
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::SetFixedPoint(int Width, const NUMBER& FullScale){
  Format.Signed->Value = 0;

  if(Width < 0){
    Format.Width ->Value = -Width;
    Format.Signed->Value =  1;
  }else{
    Format.Width ->Value =  Width;
  }

  Format.FullScale->Value = FullScale;
  if(FullScale < 0){
    Format.FullScale->Value.Mul(-1);
    Format.Signed   ->Value = 1;
  }
}
//------------------------------------------------------------------------------

bool SYNTHESISABLE::ApplyParameters(list<AST::BASE*>& Parameters){
  int  Position          = 0; // Negative => named parameters
  bool ExplicitFullScale = false;

  int    Width = 0;
  NUMBER FullScale;

  foreach(Parameter, Parameters){
    if((*Parameter)->IsExpression()){
      if(Position < 0) return false; // Mixing named and positional parameters

      AST::EXPRESSION* Param = ((AST::EXPRESSION*)(*Parameter))->Evaluate();
      *Parameter = Param;
      if(!Param) return false;

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
          Param->Error("Parameters must be pure scripting expressions");
          return false;
      }

    }else if((*Parameter)->IsAssignment()){
      Position = -1;
      error("Not yet implemented");

    }else{
      return false;
    }
    if(Position >= 0) Position++;
  }
  if(Width == 0) Width = 1;

  if(!ExplicitFullScale){
    FullScale = 1;
    FullScale.BinScale(Width);
  }
  SetFixedPoint(Width, FullScale);

  return true;
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::DisplayParameters(int Indent){
  Debug.Print("%s\n", Name.c_str());

  Debug.Indent(Indent);
  Debug.Print("Used       = %s\n", Used ? "true" : "false");
  Debug.Indent(Indent);
  Debug.Print("Width      = %u\n", Width());
  Debug.Indent(Indent);
  Debug.Print("Full-scale = %s\n", FullScale().Display());
  Debug.Indent(Indent);
  Debug.Print("Signed     = %s\n", Signed() ? "true" : "false");

  Debug.Indent(Indent);
  Debug.Print("Direction  = ");
  switch(Direction){
    case AST::DEFINITION::DIRECTION::Inferred     : Debug.Print("Inferred\n"     ); break;
    case AST::DEFINITION::DIRECTION::Input        : Debug.Print("Input\n"        ); break;
    case AST::DEFINITION::DIRECTION::Output       : Debug.Print("Output\n"       ); break;
    case AST::DEFINITION::DIRECTION::Bidirectional: Debug.Print("Bidirectional\n"); break;
    default                                       : Debug.Print("Invalid\n"      ); break;
  }
}
//------------------------------------------------------------------------------

BASE* SYNTHESISABLE::GetAttribute(const std::string& Name){
  if(Name == "width"    ) return Format.Width;
  if(Name == "fullscale") return Format.FullScale;
  if(Name == "signed"   ) return Format.Signed;

  return BASE::GetAttribute(Name);
}
//------------------------------------------------------------------------------

AST::EXPRESSION* SYNTHESISABLE::GetBuiltInAttributeValue(const std::string& Name){
  if(Name == "width"){
    auto Result = new AST::LITERAL(0, "");
    Result->Value = Width();
    return Result;
  }
  if(Name == "fullscale"){
    auto Result = new AST::LITERAL(0, "");
    Result->Value = FullScale();
    return Result;
  }
  if(Name == "signed"){
    auto Result = new AST::LITERAL(0, "");
    Result->Value = Signed() ? 1 : 0;
    return Result;
  }
  return BASE::GetBuiltInAttributeValue(Name);
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::Validate(){
  assert(Format.Width    ->Value  > 0);
  assert(Format.FullScale->Value  > 0);
  assert(Format.Signed   ->Value == 0 || Format.Signed->Value == 1);

  BASE::Validate();
}
//------------------------------------------------------------------------------


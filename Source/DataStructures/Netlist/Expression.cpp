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

#include "Expression.h"
//------------------------------------------------------------------------------

using namespace NETLIST;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(int Line, const std::string& Filename, EXPRESSION_TYPE ExpressionType){
  this->Line           = Line;
  this->Filename       = Filename;
  this->ExpressionType = ExpressionType;

  ObjectRef = 0;
  Left      = 0;
  Right     = 0;

  Signed    = false;
  Width     = 0;
  FullScale = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(EXPRESSION* Expression){
  ExpressionType = EXPRESSION_TYPE::Literal;
  ObjectRef      = 0;
  Left           = 0;
  Right          = 0;

  if(!Expression) return;

  Line           = Expression->Line;
  Filename       = Expression->Filename;
  ExpressionType = Expression->ExpressionType;
  Signed         = Expression->Signed;
  Width          = Expression->Width;
  FullScale      = Expression->FullScale;

  ObjectRef      = Expression->ObjectRef;
  Value          = Expression->Value;
  StrValue       = Expression->StrValue;

  if(Expression->Left ) Left  = new EXPRESSION(Expression->Left );
  if(Expression->Right) Right = new EXPRESSION(Expression->Right);

  for(size_t n = 0; n < Expression->Elements.size(); n++){
    if(Expression->Elements[n]) Elements.push_back(new EXPRESSION(Expression->Elements[n]));
    else                        Elements.push_back(0);
  }
}
//------------------------------------------------------------------------------

EXPRESSION::~EXPRESSION(){
  // Don't delete the Object reference -- it's part of the namespace tree

  if(Left ) delete Left;
  if(Right) delete Right;

  for(size_t n = 0; n < Elements.size(); n++){
    if(Elements[n]) delete Elements[n];
  }
}
//------------------------------------------------------------------------------

void EXPRESSION::Display(){
  if(Width) Debug.print(ANSI_FG_BRIGHT_GREEN "(" ANSI_RESET);

  if(Left){
    if(Left->Left || Left->Right) Debug.print(ANSI_FG_BRIGHT_GREEN "(" ANSI_RESET);
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(ANSI_FG_BRIGHT_GREEN ")" ANSI_RESET);
  }

  switch(ExpressionType){
    case EXPRESSION_TYPE::String:
      Debug.print(ANSI_FG_CYAN "\"%s\"" ANSI_RESET, StrValue.c_str());
      break;

    case EXPRESSION_TYPE::Literal:
      Debug.print(Value.Display());
      break;

    case EXPRESSION_TYPE::Array:
      Debug.print(ANSI_FG_BRIGHT_BLUE "[" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE "]" ANSI_RESET);
      break;

    case EXPRESSION_TYPE::Object:
      if(ObjectRef){
        ObjectRef->DisplayLongName();
      }else{
        error("Null object reference");
      }
      break;

    case EXPRESSION_TYPE::VectorConcatenate:
      Debug.print(ANSI_FG_BRIGHT_BLUE ":(" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE ")" ANSI_RESET);
      break;

    case EXPRESSION_TYPE::ArrayConcatenate:
      Debug.print(ANSI_FG_BRIGHT_BLUE ":[" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE "]" ANSI_RESET);
      break;

    case EXPRESSION_TYPE::FunctionCall : Debug.print(ANSI_FG_BRIGHT_BLUE "{call}" ANSI_RESET);   break;

    case EXPRESSION_TYPE::Slice        : Debug.print(ANSI_FG_BRIGHT_BLUE "{slice}" ANSI_RESET);  break;

    case EXPRESSION_TYPE::Increment    : Debug.print(ANSI_FG_BRIGHT_BLUE "++" ANSI_RESET);       break;
    case EXPRESSION_TYPE::Decrement    : Debug.print(ANSI_FG_BRIGHT_BLUE "--" ANSI_RESET);       break;
    case EXPRESSION_TYPE::Factorial    : Debug.print(ANSI_FG_BRIGHT_BLUE "!"  ANSI_RESET);       break;

    case EXPRESSION_TYPE::Negate       : Debug.print(ANSI_FG_BRIGHT_BLUE " -" ANSI_RESET);       break;
    case EXPRESSION_TYPE::Bit_NOT      : Debug.print(ANSI_FG_BRIGHT_BLUE " ~" ANSI_RESET);       break;

    case EXPRESSION_TYPE::AND_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE  " &" ANSI_RESET);      break;
    case EXPRESSION_TYPE::NAND_Reduce  : Debug.print(ANSI_FG_BRIGHT_BLUE " ~&" ANSI_RESET);      break;
    case EXPRESSION_TYPE::OR_Reduce    : Debug.print(ANSI_FG_BRIGHT_BLUE  " |" ANSI_RESET);      break;
    case EXPRESSION_TYPE::NOR_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE " ~|" ANSI_RESET);      break;
    case EXPRESSION_TYPE::XOR_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE  " #" ANSI_RESET);      break;
    case EXPRESSION_TYPE::XNOR_Reduce  : Debug.print(ANSI_FG_BRIGHT_BLUE " ~#" ANSI_RESET);      break;
    case EXPRESSION_TYPE::Logical_NOT  : Debug.print(ANSI_FG_BRIGHT_BLUE  " !" ANSI_RESET);      break;

    case EXPRESSION_TYPE::Cast         : Debug.print(ANSI_FG_BRIGHT_BLUE " {cast} " ANSI_RESET); break;

    case EXPRESSION_TYPE::Replicate    : Debug.print(ANSI_FG_BRIGHT_BLUE "{rep}" ANSI_RESET);    break;

    case EXPRESSION_TYPE::Exponential  : Debug.print(ANSI_FG_BRIGHT_BLUE " ^ "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Multiply     : Debug.print(ANSI_FG_BRIGHT_BLUE " * "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Divide       : Debug.print(ANSI_FG_BRIGHT_BLUE " / "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Modulus      : Debug.print(ANSI_FG_BRIGHT_BLUE " %% " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Add          : Debug.print(ANSI_FG_BRIGHT_BLUE " + "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Subtract     : Debug.print(ANSI_FG_BRIGHT_BLUE " - "  ANSI_RESET);     break;

    case EXPRESSION_TYPE::Shift_Left   : Debug.print(ANSI_FG_BRIGHT_BLUE " << " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Shift_Right  : Debug.print(ANSI_FG_BRIGHT_BLUE " >> " ANSI_RESET);     break;

    case EXPRESSION_TYPE::Less         : Debug.print(ANSI_FG_BRIGHT_BLUE " < "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Greater      : Debug.print(ANSI_FG_BRIGHT_BLUE " > "  ANSI_RESET);     break;
    case EXPRESSION_TYPE::Less_Equal   : Debug.print(ANSI_FG_BRIGHT_BLUE " <= " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Greater_Equal: Debug.print(ANSI_FG_BRIGHT_BLUE " >= " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Equal        : Debug.print(ANSI_FG_BRIGHT_BLUE " == " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Not_Equal    : Debug.print(ANSI_FG_BRIGHT_BLUE " != " ANSI_RESET);     break;

    case EXPRESSION_TYPE::Bit_AND      : Debug.print(ANSI_FG_BRIGHT_BLUE  " & " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Bit_NAND     : Debug.print(ANSI_FG_BRIGHT_BLUE " ~& " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Bit_OR       : Debug.print(ANSI_FG_BRIGHT_BLUE  " | " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Bit_NOR      : Debug.print(ANSI_FG_BRIGHT_BLUE " ~| " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Bit_XOR      : Debug.print(ANSI_FG_BRIGHT_BLUE  " # " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Bit_XNOR     : Debug.print(ANSI_FG_BRIGHT_BLUE " ~# " ANSI_RESET);     break;

    case EXPRESSION_TYPE::Logical_AND  : Debug.print(ANSI_FG_BRIGHT_BLUE " && " ANSI_RESET);     break;
    case EXPRESSION_TYPE::Logical_OR   : Debug.print(ANSI_FG_BRIGHT_BLUE " || " ANSI_RESET);     break;

    case EXPRESSION_TYPE::Conditional  : Debug.print(ANSI_FG_BRIGHT_BLUE " ? " ANSI_RESET);      break;

    default: Debug.print(ANSI_FG_BRIGHT_RED "(Unknown expression type: %d)" ANSI_RESET, (int)ExpressionType);
  }

  if(Right){
    if(Right->Left || Right->Right) Debug.print(ANSI_FG_BRIGHT_GREEN "(" ANSI_RESET);
    Right->Display();
    if(Right->Left || Right->Right) Debug.print(ANSI_FG_BRIGHT_GREEN ")" ANSI_RESET);
  }

  if(Width){
    Debug.print(ANSI_FG_BRIGHT_GREEN ")");
    Debug.print(ANSI_FG_BRIGHT_BLACK "@(");
    Debug.print("%d", Width);
    Debug.print(", %s", Signed ? "-" : "");
    Debug.print("%s)" ANSI_RESET, FullScale.Display());
  }
}
//------------------------------------------------------------------------------

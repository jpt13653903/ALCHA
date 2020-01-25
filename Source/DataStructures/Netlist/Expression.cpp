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

EXPRESSION::EXPRESSION(EXPRESSION_TYPE ExpressionType){
  this->ExpressionType = ExpressionType;

  ObjectRef = 0;
  Left      = 0;
  Right     = 0;

  RawAssign = false;
  Signed    = false;
  Width     = 0;
  FullScale = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(EXPRESSION* Expression){
  ExpressionType = Literal;
  ObjectRef      = 0;
  Left           = 0;
  Right          = 0;

  if(!Expression) return;

  ExpressionType = Expression->ExpressionType;
  RawAssign      = Expression->RawAssign;
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
  if(RawAssign) Debug.print(ANSI_FG_BRIGHT_BLACK "{raw}" ANSI_RESET);
  if(Width    ) Debug.print(ANSI_FG_BRIGHT_GREEN "(" ANSI_RESET);

  if(Left){
    if(Left->Left || Left->Right) Debug.print(ANSI_FG_BRIGHT_GREEN "(" ANSI_RESET);
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(ANSI_FG_BRIGHT_GREEN ")" ANSI_RESET);
  }

  switch(ExpressionType){
    case String:
      Debug.print(ANSI_FG_CYAN "\"%s\"" ANSI_RESET, StrValue.c_str());
      break;

    case Literal:
      Debug.print(Value.Display());
      break;

    case Array:
      Debug.print(ANSI_FG_BRIGHT_BLUE "[" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE "]" ANSI_RESET);
      break;

    case Object:
      if(ObjectRef){
        ObjectRef->DisplayLongName();
      }else{
        error("Null object reference");
      }
      break;

    case VectorConcatenate:
      Debug.print(ANSI_FG_BRIGHT_BLUE ":(" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE ")" ANSI_RESET);
      break;

    case ArrayConcatenate:
      Debug.print(ANSI_FG_BRIGHT_BLUE ":[" ANSI_RESET);
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(ANSI_FG_BRIGHT_BLUE "]" ANSI_RESET);
      break;

    case FunctionCall : Debug.print(ANSI_FG_BRIGHT_BLUE "{call}" ANSI_RESET);   break;

    case Slice        : Debug.print(ANSI_FG_BRIGHT_BLUE "{slice}" ANSI_RESET);  break;

    case Increment    : Debug.print(ANSI_FG_BRIGHT_BLUE "++" ANSI_RESET);       break;
    case Decrement    : Debug.print(ANSI_FG_BRIGHT_BLUE "--" ANSI_RESET);       break;
    case Factorial    : Debug.print(ANSI_FG_BRIGHT_BLUE "!"  ANSI_RESET);       break;

    case Negate       : Debug.print(ANSI_FG_BRIGHT_BLUE " -" ANSI_RESET);       break;
    case Bit_NOT      : Debug.print(ANSI_FG_BRIGHT_BLUE " ~" ANSI_RESET);       break;

    case AND_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE  " &" ANSI_RESET);      break;
    case NAND_Reduce  : Debug.print(ANSI_FG_BRIGHT_BLUE " ~&" ANSI_RESET);      break;
    case OR_Reduce    : Debug.print(ANSI_FG_BRIGHT_BLUE  " |" ANSI_RESET);      break;
    case NOR_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE " ~|" ANSI_RESET);      break;
    case XOR_Reduce   : Debug.print(ANSI_FG_BRIGHT_BLUE  " #" ANSI_RESET);      break;
    case XNOR_Reduce  : Debug.print(ANSI_FG_BRIGHT_BLUE " ~#" ANSI_RESET);      break;
    case Logical_NOT  : Debug.print(ANSI_FG_BRIGHT_BLUE  " !" ANSI_RESET);      break;

    case Cast         : Debug.print(ANSI_FG_BRIGHT_BLUE " {cast} " ANSI_RESET); break;

    case Replicate    : Debug.print(ANSI_FG_BRIGHT_BLUE "{rep}" ANSI_RESET);    break;

    case Exponential  : Debug.print(ANSI_FG_BRIGHT_BLUE " ^ "  ANSI_RESET);     break;
    case Multiply     : Debug.print(ANSI_FG_BRIGHT_BLUE " * "  ANSI_RESET);     break;
    case Divide       : Debug.print(ANSI_FG_BRIGHT_BLUE " / "  ANSI_RESET);     break;
    case Modulus      : Debug.print(ANSI_FG_BRIGHT_BLUE " %% " ANSI_RESET);     break;
    case Add          : Debug.print(ANSI_FG_BRIGHT_BLUE " + "  ANSI_RESET);     break;
    case Subtract     : Debug.print(ANSI_FG_BRIGHT_BLUE " - "  ANSI_RESET);     break;

    case Shift_Left   : Debug.print(ANSI_FG_BRIGHT_BLUE " << " ANSI_RESET);     break;
    case Shift_Right  : Debug.print(ANSI_FG_BRIGHT_BLUE " >> " ANSI_RESET);     break;

    case Less         : Debug.print(ANSI_FG_BRIGHT_BLUE " < "  ANSI_RESET);     break;
    case Greater      : Debug.print(ANSI_FG_BRIGHT_BLUE " > "  ANSI_RESET);     break;
    case Less_Equal   : Debug.print(ANSI_FG_BRIGHT_BLUE " <= " ANSI_RESET);     break;
    case Greater_Equal: Debug.print(ANSI_FG_BRIGHT_BLUE " >= " ANSI_RESET);     break;
    case Equal        : Debug.print(ANSI_FG_BRIGHT_BLUE " == " ANSI_RESET);     break;
    case Not_Equal    : Debug.print(ANSI_FG_BRIGHT_BLUE " != " ANSI_RESET);     break;

    case Bit_AND      : Debug.print(ANSI_FG_BRIGHT_BLUE  " & " ANSI_RESET);     break;
    case Bit_NAND     : Debug.print(ANSI_FG_BRIGHT_BLUE " ~& " ANSI_RESET);     break;
    case Bit_OR       : Debug.print(ANSI_FG_BRIGHT_BLUE  " | " ANSI_RESET);     break;
    case Bit_NOR      : Debug.print(ANSI_FG_BRIGHT_BLUE " ~| " ANSI_RESET);     break;
    case Bit_XOR      : Debug.print(ANSI_FG_BRIGHT_BLUE  " # " ANSI_RESET);     break;
    case Bit_XNOR     : Debug.print(ANSI_FG_BRIGHT_BLUE " ~# " ANSI_RESET);     break;

    case Logical_AND  : Debug.print(ANSI_FG_BRIGHT_BLUE " && " ANSI_RESET);     break;
    case Logical_OR   : Debug.print(ANSI_FG_BRIGHT_BLUE " || " ANSI_RESET);     break;

    case Conditional  : Debug.print(ANSI_FG_BRIGHT_BLUE " ? " ANSI_RESET);      break;

    default: Debug.print(ANSI_FG_BRIGHT_RED "(Unknown expression type: %d)" ANSI_RESET, ExpressionType);
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

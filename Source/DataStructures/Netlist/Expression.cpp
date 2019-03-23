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

  Raw = false;
}
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(EXPRESSION* Expression){
  ExpressionType = Literal;
  ObjectRef      = 0;
  Left           = 0;
  Right          = 0;
  Raw            = false;

  if(!Expression) return;

  ExpressionType = Expression->ExpressionType;
  Raw            = Expression->Raw;
                                             
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
  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }

  if(Raw) Debug.print("{raw}");

  switch(ExpressionType){
    case String:
      Debug.print("\"%s\"", StrValue.c_str());
      break;

    case Literal:
      Debug.print(Value.GetString());
      break;

    case Array:
      Debug.print("[");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print("]");
      break;

    case Object:
      if(ObjectRef){
        ObjectRef->DisplayLongName();
      }else{
        error("Null object reference");
      }
      break;

    case VectorConcatenate:
      Debug.print(":(");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print(")");
      break;

    case ArrayConcatenate:
      Debug.print(":[");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) Debug.print(", ");
        if(Elements[n]) Elements[n]->Display();
        else            Debug.print("{null}");
      }
      Debug.print("]");
      break;

    case FunctionCall     : Debug.print("{call}" );        break;

    case Slice            : Debug.print("{slice}");        break;

    case Increment        : Debug.print("++");             break;
    case Decrement        : Debug.print("--");             break;
    case Factorial        : Debug.print("!" );             break;

    case Negate           : Debug.print(" -");             break;
    case Bit_NOT          : Debug.print(" ~");             break;

    case AND_Reduce       : Debug.print( " &");            break;
    case NAND_Reduce      : Debug.print(" ~&");            break;
    case OR_Reduce        : Debug.print( " |");            break;
    case NOR_Reduce       : Debug.print(" ~|");            break;
    case XOR_Reduce       : Debug.print( " #");            break;
    case XNOR_Reduce      : Debug.print(" ~#");            break;
    case Logical_NOT      : Debug.print( " !");            break;

    case Cast             : Debug.print(" {cast} ");       break;

    case Replicate        : Debug.print("{rep}");          break;

    case Exponential      : Debug.print(" ^ " );           break;
    case Multiply         : Debug.print(" * " );           break;
    case Divide           : Debug.print(" / " );           break;
    case Modulus          : Debug.print(" %% ");           break;
    case Add              : Debug.print(" + " );           break;
    case Subtract         : Debug.print(" - " );           break;

    case Shift_Left       : Debug.print(" << ");           break;
    case Shift_Right      : Debug.print(" >> ");           break;

    case Less             : Debug.print(" < " );           break;
    case Greater          : Debug.print(" > " );           break;
    case Less_Equal       : Debug.print(" <= ");           break;
    case Greater_Equal    : Debug.print(" >= ");           break;
    case Equal            : Debug.print(" == ");           break;
    case Not_Equal        : Debug.print(" != ");           break;

    case Bit_AND          : Debug.print( " & ");           break;
    case Bit_NAND         : Debug.print(" ~& ");           break;
    case Bit_OR           : Debug.print( " | ");           break;
    case Bit_NOR          : Debug.print(" ~| ");           break;
    case Bit_XOR          : Debug.print( " # ");           break;
    case Bit_XNOR         : Debug.print(" ~# ");           break;

    case Logical_AND      : Debug.print(" && ");           break;
    case Logical_OR       : Debug.print(" || ");           break;

    case Conditional      : Debug.print(" ? ");            break;

    default: Debug.print("(Unknown expression type: %d)", ExpressionType);
  }

  if(Right){
    if(Right->Left || Right->Right) Debug.print("(");
    Right->Display();
    if(Right->Left || Right->Right) Debug.print(")");
  }
}
//------------------------------------------------------------------------------

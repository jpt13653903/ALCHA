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
    if(Left->Left || Left->Right) printf("(");
    Left->Display();
    if(Left->Left || Left->Right) printf(")");
  }

  if(Raw) printf("{raw}");

  switch(ExpressionType){
    case String:
      printf("\"%s\"", StrValue.c_str());
      break;

    case Literal:
      Value.Display();
      break;

    case Array:
      printf("[");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) printf(", ");
        if(Elements[n]) Elements[n]->Display();
        else            printf("{null}");
      }
      printf("]");
      break;

    case Object:
      if(ObjectRef){
        ObjectRef->DisplayLongName();
      }else{
        error("Null object reference");
      }
      break;

    case VectorConcatenate:
      printf(":(");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) printf(", ");
        if(Elements[n]) Elements[n]->Display();
        else            printf("{null}");
      }
      printf(")");
      break;

    case ArrayConcatenate:
      printf(":[");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) printf(", ");
        if(Elements[n]) Elements[n]->Display();
        else            printf("{null}");
      }
      printf("]");
      break;

    case FunctionCall     : printf("{call}" );        break;

    case Slice            : printf("{slice}");        break;

    case Increment        : printf("++");             break;
    case Decrement        : printf("--");             break;
    case Factorial        : printf("!" );             break;

    case Negate           : printf(" -");             break;
    case Bit_NOT          : printf(" ~");             break;

    case AND_Reduce       : printf( " &");            break;
    case NAND_Reduce      : printf(" ~&");            break;
    case OR_Reduce        : printf( " |");            break;
    case NOR_Reduce       : printf(" ~|");            break;
    case XOR_Reduce       : printf( " #");            break;
    case XNOR_Reduce      : printf(" ~#");            break;
    case Logical_NOT      : printf( " !");            break;

    case Cast             : printf(" {cast} ");       break;

    case Replicate        : printf("{rep}");          break;

    case Exponential      : printf(" ^ " );           break;
    case Multiply         : printf(" * " );           break;
    case Divide           : printf(" / " );           break;
    case Modulus          : printf(" %% ");           break;
    case Add              : printf(" + " );           break;
    case Subtract         : printf(" - " );           break;

    case Shift_Left       : printf(" << ");           break;
    case Shift_Right      : printf(" >> ");           break;

    case Less             : printf(" < " );           break;
    case Greater          : printf(" > " );           break;
    case Less_Equal       : printf(" <= ");           break;
    case Greater_Equal    : printf(" >= ");           break;
    case Equal            : printf(" == ");           break;
    case Not_Equal        : printf(" != ");           break;

    case Bit_AND          : printf( " & ");           break;
    case Bit_NAND         : printf(" ~& ");           break;
    case Bit_OR           : printf( " | ");           break;
    case Bit_NOR          : printf(" ~| ");           break;
    case Bit_XOR          : printf( " # ");           break;
    case Bit_XNOR         : printf(" ~# ");           break;

    case Logical_AND      : printf(" && ");           break;
    case Logical_OR       : printf(" || ");           break;

    case Conditional      : printf(" ? ");            break;

    default: printf("(Unknown expression type: %d)", ExpressionType);
  }

  if(Right){
    if(Right->Left || Right->Right) printf("(");
    Right->Display();
    if(Right->Left || Right->Right) printf(")");
  }
}
//------------------------------------------------------------------------------

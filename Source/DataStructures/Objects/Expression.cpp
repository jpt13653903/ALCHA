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

using namespace OBJECTS;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(AST::EXPRESSION::EXPRESSION_TYPE ExpressionType):
BASE("", TYPE::Expression){
  this->ExpressionType = ExpressionType;

  Object   = 0;
  Left     = 0;
  Right    = 0;
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

  switch(ExpressionType){
    case AST::EXPRESSION::String:
      printf("\"%s\"", StrValue.c_str());
      break;

    case AST::EXPRESSION::Literal:
      Value.Display();
      break;

    case AST::EXPRESSION::Identifier:
      error("Not yet implemented");
      // if(Name.empty()) printf("(Identifier node has no name)");
      // else             printf("%s", Name.c_str());
      break;

    case AST::EXPRESSION::Array:
      printf("[");
      for(size_t n = 0; n < Elements.size(); n++){
        if(n > 0) printf(", ");
        if(Elements[n]) Elements[n]->Display();
        else            printf("{null}");
      }
      printf("]");
      break;

    case AST::EXPRESSION::VectorConcatenate: printf("{VectorConcat}"); break;
    case AST::EXPRESSION::ArrayConcatenate : printf("{ArrayConcat}" ); break;

    case AST::EXPRESSION::FunctionCall     : printf("{call}" );        break;

    case AST::EXPRESSION::Slice            : printf("{slice}");        break;

    case AST::EXPRESSION::AccessMember     : printf("." );             break;
    case AST::EXPRESSION::AccessMemberSafe : printf("?." );            break;
    case AST::EXPRESSION::AccessAttribute  : printf("'" );             break;

    case AST::EXPRESSION::Increment        : printf("++");             break;
    case AST::EXPRESSION::Decrement        : printf("--");             break;
    case AST::EXPRESSION::Factorial        : printf("!" );             break;

    case AST::EXPRESSION::Range            : printf("..");             break;

    case AST::EXPRESSION::Negate           : printf(" -");             break;
    case AST::EXPRESSION::Bit_NOT          : printf(" ~");             break;
    case AST::EXPRESSION::Raw              : printf(" :");             break;

    case AST::EXPRESSION::AND_Reduce       : printf( " &");            break;
    case AST::EXPRESSION::NAND_Reduce      : printf(" ~&");            break;
    case AST::EXPRESSION::OR_Reduce        : printf( " |");            break;
    case AST::EXPRESSION::NOR_Reduce       : printf(" ~|");            break;
    case AST::EXPRESSION::XOR_Reduce       : printf( " #");            break;
    case AST::EXPRESSION::XNOR_Reduce      : printf(" ~#");            break;
    case AST::EXPRESSION::Logical_NOT      : printf( " !");            break;

    case AST::EXPRESSION::Cast             : printf(" {cast} ");       break;

    case AST::EXPRESSION::Replicate        : printf("{rep}");          break;

    case AST::EXPRESSION::Exponential      : printf(" ^ " );           break;
    case AST::EXPRESSION::Multiply         : printf(" * " );           break;
    case AST::EXPRESSION::Divide           : printf(" / " );           break;
    case AST::EXPRESSION::Modulus          : printf(" %% ");           break;
    case AST::EXPRESSION::Add              : printf(" + " );           break;
    case AST::EXPRESSION::Subtract         : printf(" - " );           break;

    case AST::EXPRESSION::Shift_Left       : printf(" << ");           break;
    case AST::EXPRESSION::Shift_Right      : printf(" >> ");           break;

    case AST::EXPRESSION::Less             : printf(" < " );           break;
    case AST::EXPRESSION::Greater          : printf(" > " );           break;
    case AST::EXPRESSION::Less_Equal       : printf(" <= ");           break;
    case AST::EXPRESSION::Greater_Equal    : printf(" >= ");           break;
    case AST::EXPRESSION::Equal            : printf(" == ");           break;
    case AST::EXPRESSION::Not_Equal        : printf(" != ");           break;

    case AST::EXPRESSION::Bit_AND          : printf( " & ");           break;
    case AST::EXPRESSION::Bit_NAND         : printf(" ~& ");           break;
    case AST::EXPRESSION::Bit_OR           : printf( " | ");           break;
    case AST::EXPRESSION::Bit_NOR          : printf(" ~| ");           break;
    case AST::EXPRESSION::Bit_XOR          : printf( " # ");           break;
    case AST::EXPRESSION::Bit_XNOR         : printf(" ~# ");           break;

    case AST::EXPRESSION::Logical_AND      : printf(" && ");           break;
    case AST::EXPRESSION::Logical_OR       : printf(" || ");           break;

    case AST::EXPRESSION::Conditional      : printf(" ? ");            break;

    default: printf("(Unknown expression type: %d)", ExpressionType);
  }

  if(Right){
    if(Right->Left || Right->Right) printf("(");
    Right->Display();
    if(Right->Left || Right->Right) printf(")");
  }
}
//------------------------------------------------------------------------------

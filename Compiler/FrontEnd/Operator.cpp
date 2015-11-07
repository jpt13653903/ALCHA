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

#include "Operator.h"
//------------------------------------------------------------------------------

OPERATOR Operator;
//------------------------------------------------------------------------------

OPERATOR_NODE::OPERATOR_NODE(){
 Value = oUnknown;
}
//------------------------------------------------------------------------------

int OPERATOR_NODE::Compare(TREE_NODE* Right){
 return Operator.Compare(((OPERATOR_NODE*)Right)->Operator);
}
//------------------------------------------------------------------------------

// When adding operators, make sure that all operators starts with valid
// operators.  For instance, when adding !@#$% as an operator, make sure that
// !, !@, !@# and !@#$ are added as well.  The scanner depends on this.

OPERATOR::OPERATOR(){
 Add(oDirective       , "#"  );
 Add(oTokenConcatenate, "##" );
 Add(oBegin           , "{"  );
 Add(oEnd             , "}"  );
 Add(oStartSlice      , "["  );
 Add(oEndSlice        , "]"  );
 Add(oStartBitSlice   , "<[" );
 Add(oEndBitSlice     , "]>" );
 Add(oStartGroup      , "("  );
 Add(oEndGroup        , ")"  );
 Add(oEndState        , ";"  );
 Add(oEndStatement    , ","  );
 Add(oQuestion        , "?"  );
 Add(oElse            , ":"  );
 Add(oNamespace       , "::" );
 Add(oMember          , "."  );
 Add(oPlus            , "+"  );
 Add(oMinus           , "-"  );
 Add(oMultiply        , "*"  );
 Add(oDivide          , "/"  );
 Add(oModulus         , "%"  );
 Add(oBitXOr          , "^"  );
 Add(oBitAnd          , "&"  );
 Add(oBitOr           , "|"  );
 Add(oBitNot          , "~"  );
 Add(oNot             , "!"  );
 Add(oAssign          , "="  );
 Add(oLess            , "<"  );
 Add(oGreater         , ">"  );
 Add(oPlusAssign      , "+=" );
 Add(oMinusAssign     , "-=" );
 Add(oMultiplyAssign  , "*=" );
 Add(oDivideAssign    , "/=" );
 Add(oModulusAssign   , "%=" );
 Add(oBitXOrAssign    , "^=" );
 Add(oBitAndAssign    , "&=" );
 Add(oBitOrAssign     , "|=" );
 Add(oShiftLeft       , "<<" );
 Add(oShiftRight      , ">>" );
 Add(oShiftLeftAssign , "<<=");
 Add(oShiftRightAssign, ">>=");
 Add(oEqual           , "==" );
 Add(oNotEqual        , "!=" );
 Add(oLessEqual       , "<=" );
 Add(oGreaterEqual    , ">=" );
 Add(oAnd             , "&&" );
 Add(oOr              , "||" );
 Add(oIncrement       , "++" );
 Add(oDecrement       , "--" );
 Add(oGrave           , "`"  );
 Add(oArrow           , "->" );
}
//------------------------------------------------------------------------------

void OPERATOR::Add(OPERATOR_VALUE Value, const char* Operator){
 OPERATOR_NODE* N = new OPERATOR_NODE;
 N->Value = Value;
 N->Operator << Operator;
 Insert(N);
}
//------------------------------------------------------------------------------

int OPERATOR::GetCode(const char* Operator){
 OPERATOR_NODE Key;
 Key.Operator << Operator;

 OPERATOR_NODE* N = (OPERATOR_NODE*)Find(&Key);

 if(N) return N->Value;
 else  return 0;
}
//------------------------------------------------------------------------------

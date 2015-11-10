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

#ifndef Operators_h
#define Operators_h
//------------------------------------------------------------------------------

#include "Tree.h"
#include "MyString.h"
//------------------------------------------------------------------------------

enum OPERATOR_CODE{
 oUnknown = 0,

 oDirective       , // #
 oTokenConcatenate, // ##

 oBegin           , // {
 oEnd             , // }
 oStartSlice      , // [
 oEndSlice        , // ]
 oStartBitSlice   , // <[
 oEndBitSlice     , // ]>
 oStartGroup      , // (
 oEndGroup        , // )
 oEndState        , // ;
 oEndStatement    , // ,
 oQuestion        , // ?
 oElse            , // :
 oNamespace       , // ::
 oMember          , // .
 oPlus            , // +
 oMinus           , // -
 oMultiply        , // *
 oDivide          , // /
 oModulus         , // %
 oBitXOr          , // ^
 oBitAnd          , // &
 oBitOr           , // |
 oBitNot          , // ~
 oNot             , // !
 oAssign          , // =
 oLess            , // <
 oGreater         , // >
 oPlusAssign      , // +=
 oMinusAssign     , // -=
 oMultiplyAssign  , // *=
 oDivideAssign    , // /=
 oModulusAssign   , // %=
 oBitXOrAssign    , // ^=
 oBitAndAssign    , // &=
 oBitOrAssign     , // |=
 oShiftLeft       , // <<
 oShiftRight      , // >>
 oShiftLeftAssign , // <<=
 oShiftRightAssign, // >>=
 oEqual           , // ==
 oNotEqual        , // !=
 oLessEqual       , // <=
 oGreaterEqual    , // >=
 oAnd             , // &&
 oOr              , // ||
 oIncrement       , // ++
 oDecrement       , // --

 // Unused?
 oArrow, // ->

 // Aliases
 oStartParameters = oStartGroup  , // (
 oEndParameters   = oEndGroup    , // )
 oTo              = oElse        , // :
 oStar            = oMultiply    , // *
 oBeginAttributes = oLess        , // <
 oEndAttributes   = oGreater     , // >
 oComma           = oEndStatement  // ,
};
//------------------------------------------------------------------------------

struct OPERATOR_NODE: public TREE_NODE{
  OPERATOR_CODE Code;
  STRING        Operator; // Key

  OPERATOR_NODE* Next; // Linked list used for reverse lookup

  OPERATOR_NODE();
 ~OPERATOR_NODE(){}

  int Compare(TREE_NODE* Right);
};
//------------------------------------------------------------------------------

class OPERATORS: protected TREE{
 private:
  OPERATOR_NODE* List; // Linked list used for reverse lookup

  void Add(OPERATOR_CODE Code, const char* Operator);

 public:
  OPERATORS();
 ~OPERATORS(){}

  OPERATOR_CODE GetCode(const char* Operator); // O(log(N))

  // Reverse look-up: used to display errors; runs in O(N)
  bool GetName(OPERATOR_CODE Code, STRING& Operator);
};
//------------------------------------------------------------------------------

extern OPERATORS Operators;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

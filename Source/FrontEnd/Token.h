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

#ifndef Token_h
#define Token_h
//------------------------------------------------------------------------------

#include <string>
//------------------------------------------------------------------------------

#include "Logger.h"
#include "General.h"
#include "Number.h"
//------------------------------------------------------------------------------

struct TOKEN{
    enum class TYPE{
        Unknown = 0, // also used to designate errors, etc.

        // White-space .............................................................
        Space, // Spaces are only used in the scanner
        Newline,

        // Special keywords ........................................................
        FILE, LINE, DATE, TIME,
        // __CLASS__, __FUNCTION__, etc. are implemented as special variables

        // Keywords ................................................................
        Void    , Auto   ,
        Pin     , Net    ,
        Byte    , Char   , Num,
        True    , False  ,
        Class   , Enum   , Struct   , Group , Alias,
        Input   , Output ,
        Public  , Private, Protected,
        If      , Else   , For, In  , While , Loop ,
        Switch  , Case   , Default  ,
        Import  , As     ,
        Return  , Break  , Continue , Goto  ,
        Func    , Inline , Operator ,
        RTL     , FSM    , HDL,
        Stimulus, Emulate, Assert, Wait ,
        PosEdge , NegEdge,

        // Operators ...............................................................
        TernaryIf,
        Elvis,

        Bit_OR,
        Bit_NOR,
        Bit_XOR,
        Bit_XNOR,
        Bit_AND,
        Bit_NAND,

        Equal,
        Not_Equal,
        Less,
        Greater,
        Less_Equal,
        Greater_Equal,

        Shift_Left,
        Shift_Right,

        Add,
        Subtract,
        Multiply,
        Divide,
        Modulus,
        Exponential,
        Factorial,

        Replicate,
        Concatenate,
        ArrayConcatenate,

        Stringify,
        StringifyExpression,

        To,

        Bit_NOT,
        Increment,
        Decrement,

        AccessMember,
        AccessMemberSafe,
        AccessMemberPush,
        AccessAttribute,

        CastOp,

        Assign,
        Raw_Assign,
        Append_Assign,
        Add_Assign,
        Subtract_Assign,
        Multiply_Assign,
        Divide_Assign,
        Modulus_Assign,
        XOR_Assign,
        AND_Assign,
        OR_Assign,
        Exponential_Assign,
        Shift_Left_Assign,
        Shift_Right_Assign,

        OpenRound,
        CloseRound,
        OpenSquare,
        CloseSquare,
        OpenCurly,
        CloseCurly,
        Comma,
        Colon,
        Semicolon,

        WaitFor,
        WaitCycles,
        SequenceConsecutive,
        SequenceGoto,
        SequenceNonConsecutive,
        AssertImplies,
        AssertImpliesNext,
        Or,
        And,
        Intersect,

        // Other types .............................................................
        Identifier,
        Literal,
        String,

        // Aliases .................................................................
        AND_Reduce  = Bit_AND,
        NAND_Reduce = Bit_NAND,
        OR_Reduce   = Bit_OR,
        NOR_Reduce  = Bit_NOR,
        XOR_Reduce  = Bit_XOR,
        XNOR_Reduce = Bit_XNOR,
        Logical_NOT = Factorial,
        TernaryElse = Colon,
        Step        = Colon,
        RawBits     = Colon,
        Negate      = Subtract,
        OpenAngle   = Less,
        CloseAngle  = Greater,

        WaitUntil   = CastOp
    } Type;

    int         Line;  // The line number
    std::string Data;  // The string contents; otherwise the original token characters
    NUMBER      Value; // Literal value

    TOKEN();
    void Display(); // Used for debugging
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

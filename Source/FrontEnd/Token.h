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
#include "Number.h"
//------------------------------------------------------------------------------

struct Token{
    enum class Type{
            Unknown = 0, // also used to designate errors, etc.
            EndOfFile,

        // White-space .........................................................
            Space, // Spaces are only used in the scanner
            Newline,

        // Special keywords ....................................................
            File, Line, Date, Time,
            // __CLASS__, __FUNCTION__, etc. are implemented as special variables

        // Keywords ............................................................
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
            Return  , Break  , Continue , GoTo  ,
            Func    , Inline , Operator ,
            RTL     , FSM    , HDL,
            Stimulus, Emulate, Assert, Wait ,
            PosEdge , NegEdge,

        // Operators ...........................................................
            TernaryIf,
            Elvis,

            BitOr,
            BitNor,
            BitXor,
            BitXnor,
            BitAnd,
            BitNand,

            Equal,
            NotEqual,
            Less,
            Greater,
            LessEqual,
            GreaterEqual,

            ShiftLeft,
            ShiftRight,

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

            BitNot,
            Increment,
            Decrement,

            AccessMember,
            AccessMemberSafe,
            AccessMemberPush,
            AccessAttribute,

            CastOp,

            Assign,
            RawAssign,
            AppendAssign,
            AddAssign,
            SubtractAssign,
            MultiplyAssign,
            DivideAssign,
            ModulusAssign,
            XorAssign,
            AndAssign,
            OrAssign,
            ExponentialAssign,
            ShiftLeftAssign,
            ShiftRightAssign,

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
            SequenceGoTo,
            SequenceNonConsecutive,
            AssertImplies,
            AssertImpliesNext,
            Or,
            And,
            Intersect,

        // Other types .........................................................
            Identifier,
            Literal,
            String,

        // Aliases .............................................................
            AndReduce   = BitAnd,
            NandReduce  = BitNand,
            OrReduce    = BitOr,
            NorReduce   = BitNor,
            XorReduce   = BitXor,
            XnorReduce  = BitXnor,
            LogicalNot  = Factorial,
            TernaryElse = Colon,
            Step        = Colon,
            RawBits     = Colon,
            Negate      = Subtract,
            OpenAngle   = Less,
            CloseAngle  = Greater,

            WaitUntil   = Wait,
            WaitOn      = CastOp
    } type = Type::Unknown;

    int         line  = 0;
    std::string data; // The string contents; otherwise the original token characters
    Number      value;

    const char*  decodeType() const;
    std::string& print() const;
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

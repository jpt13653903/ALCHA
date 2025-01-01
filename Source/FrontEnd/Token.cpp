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

#include "Token.h"
#include "Logger.h"
#include "General.h"
//------------------------------------------------------------------------------

const char* Token::decodeType() const
{
    switch(type){
            case Type::Unknown:                  return "Unknown";
            case Type::EndOfFile:                return "EndOfFile";

        // White-space .........................................................
            case Type::Space:                    return "Space";
            case Type::Newline:                  return "Newline";

        // Special keywords ....................................................
            case Type::File:                     return "File";
            case Type::Line:                     return "Line";
            case Type::Date:                     return "Date";
            case Type::Time:                     return "Time";

        // Keywords ............................................................
            case Type::Void:                     return "Void";
            case Type::Auto:                     return "Auto";

            case Type::Pin:                      return "Pin";
            case Type::Net:                      return "Net";

            case Type::Byte:                     return "Byte";
            case Type::Char:                     return "Char";
            case Type::Num:                      return "Num";

            case Type::True:                     return "True";
            case Type::False:                    return "False";

            case Type::Class:                    return "Class";
            case Type::Enum:                     return "Enum";
            case Type::Struct:                   return "Struct";
            case Type::Group:                    return "Group";
            case Type::Alias:                    return "Alias";

            case Type::Input:                    return "Input";
            case Type::Output:                   return "Output";

            case Type::Public:                   return "Public";
            case Type::Private:                  return "Private";
            case Type::Protected:                return "Protected";

            case Type::If:                       return "If";
            case Type::Else:                     return "Else";
            case Type::For:                      return "For";
            case Type::In:                       return "In";
            case Type::While:                    return "While";
            case Type::Loop:                     return "Loop";

            case Type::Switch:                   return "Switch";
            case Type::Case:                     return "Case";
            case Type::Default:                  return "Default";

            case Type::Import:                   return "Import";
            case Type::As:                       return "As";

            case Type::Return:                   return "Return";
            case Type::Break:                    return "Break";
            case Type::Continue:                 return "Continue";

            case Type::Func:                     return "Func";
            case Type::Inline:                   return "Inline";
            case Type::Operator:                 return "Operator";

            case Type::RTL:                      return "RTL";
            case Type::FSM:                      return "FSM";
            case Type::HDL:                      return "HDL";

            case Type::Stimulus:                 return "Stimulus";
            case Type::Emulate:                  return "Emulate";
            case Type::Sequence:                 return "Sequence";
            case Type::Assert:                   return "Assert";
            case Type::Wait:                     return "Wait";

            case Type::PosEdge:                  return "PosEdge";
            case Type::NegEdge:                  return "NegEdge";

        // Operators ...........................................................
            case Type::TernaryIf:                return "TernaryIf";
            case Type::Elvis:                    return "Elvis";

            case Type::BitOr:                    return "BitOr";
            case Type::BitNor:                   return "BitNor";
            case Type::BitXor:                   return "BitXor";
            case Type::BitXnor:                  return "BitXnor";
            case Type::BitAnd:                   return "BitAnd";
            case Type::BitNand:                  return "BitNand";

            case Type::Equal:                    return "Equal";
            case Type::NotEqual:                 return "NotEqual";
            case Type::Less:                     return "Less";
            case Type::Greater:                  return "Greater";
            case Type::LessEqual:                return "LessEqual";
            case Type::GreaterEqual:             return "GreaterEqual";

            case Type::ShiftLeft:                return "ShiftLeft";
            case Type::ShiftRight:               return "ShiftRight";

            case Type::Add:                      return "Add";
            case Type::Subtract:                 return "Subtract";
            case Type::Multiply:                 return "Multiply";
            case Type::Divide:                   return "Divide";
            case Type::Modulus:                  return "Modulus";
            case Type::Exponential:              return "Exponential";
            case Type::Factorial:                return "Factorial";

            case Type::Replicate:                return "Replicate";
            case Type::Concatenate:              return "Concatenate";
            case Type::ArrayConcatenate:         return "ArrayConcatenate";

            case Type::Stringify:                return "Stringify";
            case Type::StringifyExpression:      return "StringifyExpression";

            case Type::To:                       return "To";

            case Type::BitNot:                   return "BitNot";
            case Type::Increment:                return "Increment";
            case Type::Decrement:                return "Decrement";

            case Type::AccessMember:             return "AccessMember";
            case Type::AccessMemberSafe:         return "AccessMemberSafe";
            case Type::AccessMemberPush:         return "AccessMemberPush";
            case Type::AccessAttribute:          return "AccessAttribute";

            case Type::CastOp:                   return "CastOp";

            case Type::Assign:                   return "Assign";
            case Type::RawAssign:                return "RawAssign";
            case Type::AppendAssign:             return "AppendAssign";
            case Type::AddAssign:                return "AddAssign";
            case Type::SubtractAssign:           return "SubtractAssign";
            case Type::MultiplyAssign:           return "MultiplyAssign";
            case Type::DivideAssign:             return "DivideAssign";
            case Type::ModulusAssign:            return "ModulusAssign";
            case Type::XorAssign:                return "XorAssign";
            case Type::AndAssign:                return "AndAssign";
            case Type::OrAssign:                 return "OrAssign";
            case Type::ExponentialAssign:        return "ExponentialAssign";
            case Type::ShiftLeftAssign:          return "ShiftLeftAssign";
            case Type::ShiftRightAssign:         return "ShiftRightAssign";

            case Type::OpenRound:                return "OpenRound";
            case Type::CloseRound:               return "CloseRound";
            case Type::OpenSquare:               return "OpenSquare";
            case Type::CloseSquare:              return "CloseSquare";
            case Type::OpenCurly:                return "OpenCurly";
            case Type::CloseCurly:               return "CloseCurly";
            case Type::Comma:                    return "Comma";
            case Type::Colon:                    return "Colon";
            case Type::Semicolon:                return "Semicolon";

            case Type::WaitFor:                  return "WaitFor";
            case Type::WaitCycles:               return "WaitCycles";
            case Type::RepetitionConsecutive:    return "RepetitionConsecutive";
            case Type::RepetitionGoTo:           return "RepetitionGoTo";
            case Type::RepetitionNonConsecutive: return "RepetitionNonConsecutive";
            case Type::AssertImplies:            return "AssertImplies";
            case Type::AssertImpliesNext:        return "AssertImpliesNext";
            case Type::Or:                       return "Or";
            case Type::And:                      return "And";
            case Type::Intersect:                return "Intersect";

        // Other types .........................................................
            case Type::Identifier:               return "Identifier";
            case Type::Literal:                  return "Literal";
            case Type::String:                   return "String";

            case Type::InterpolatedStringPart:   return "InterpolatedStringPart";
            case Type::InterpolatedStringEnd:    return "InterpolatedStringEnd";

        // Invalid .............................................................
            default:                             return "Invalid Type Index";
    }
}
//------------------------------------------------------------------------------

std::string& Token::print() const
{
    static std::string result;
    char _string[0x100];

    snprintf(_string, 0x100,
             ANSI_FG_CYAN "Token: " ANSI_RESET
             ANSI_FG_BRIGHT_BLACK "Type = "  ANSI_RESET "%-24s "
             ANSI_FG_BRIGHT_BLACK "Line = "  ANSI_RESET "%5d "
             ANSI_FG_BRIGHT_BLACK "Data = "  ANSI_RESET "%-40s "
             ANSI_FG_BRIGHT_BLACK "Value = " ANSI_RESET "%s",
             decodeType(), line, data.c_str(), value.print().c_str());

    result = _string;
    return result;
}
//------------------------------------------------------------------------------


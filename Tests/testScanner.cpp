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

#include "Scanner.h"

#include <string>
//------------------------------------------------------------------------------

using std::string;

Scanner* scanner;
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s... ", name); \

    string filenmae;
    filenmae  = "testScanner/";
    filenmae += name;
    filenmae += ".alc";

    if(!scanner->open(filenmae.c_str())){
        error("Cannot open file %s", filenmae.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

void printError(Token& token, int line, Token::Type type, Number value, const char* data)
{
    string got = token.print();
    token.line  = line;
    token.type  = type;
    token.value = value;
    token.data  = data;
    string expected = token.print();

    printf(ANSI_FG_BRIGHT_RED "FAILED:\n"
           ANSI_FG_GREEN      "   Expected: %s\n"
           ANSI_FG_BRIGHT_RED "   Got:      %s\n",
           expected.c_str(),
           got.c_str());
}
//------------------------------------------------------------------------------

bool test(int line, Token::Type type, Number value, const char* data)
{
    static Token token;
    bool result = scanner->getToken(&token);

    if(type == Token::Type::EndOfFile){
        if(result){
            printError(token, line, type, value, data);
            return false;
        }
    }else{
        if(!result){
            printError(token, line, type, value, data);
            return false;
        }
    }

    if(token.line != line){
        printError(token, line, type, value, data);
        return false;
    }
    if(token.type != type){
        printError(token, line, type, value, data);
        return false;
    }
    if(token.value != value){
        printError(token, line, type, value, data);
        return false;
    }
    if(token.data != data){
        printError(token, line, type, value, data);
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool testKeywords()
{
    if(!startTest("Keywords")) return false;

    if(!test( 1, Token::Type::Void,      0, "void"))      return false;
    if(!test( 2, Token::Type::Auto,      0, "auto"))      return false;
    if(!test( 4, Token::Type::Pin,       0, "pin"))       return false;
    if(!test( 5, Token::Type::Net,       0, "net"))       return false;
    if(!test( 7, Token::Type::Byte,      0, "byte"))      return false;
    if(!test( 8, Token::Type::Char,      0, "char"))      return false;
    if(!test( 9, Token::Type::Num,       0, "num"))       return false;
    if(!test(11, Token::Type::True,      0, "true"))      return false;
    if(!test(12, Token::Type::False,     0, "false"))     return false;
    if(!test(14, Token::Type::Class,     0, "class"))     return false;
    if(!test(15, Token::Type::Enum,      0, "enum"))      return false;
    if(!test(16, Token::Type::Struct,    0, "struct"))    return false;
    if(!test(17, Token::Type::Group,     0, "group"))     return false;
    if(!test(18, Token::Type::Alias,     0, "alias"))     return false;
    if(!test(20, Token::Type::Input,     0, "input"))     return false;
    if(!test(21, Token::Type::Output,    0, "output"))    return false;
    if(!test(23, Token::Type::Public,    0, "public"))    return false;
    if(!test(24, Token::Type::Private,   0, "private"))   return false;
    if(!test(25, Token::Type::Protected, 0, "protected")) return false;
    if(!test(27, Token::Type::If,        0, "if"))        return false;
    if(!test(28, Token::Type::Else,      0, "else"))      return false;
    if(!test(29, Token::Type::For,       0, "for"))       return false;
    if(!test(30, Token::Type::In,        0, "in"))        return false;
    if(!test(31, Token::Type::While,     0, "while"))     return false;
    if(!test(32, Token::Type::Loop,      0, "loop"))      return false;
    if(!test(34, Token::Type::Switch,    0, "switch"))    return false;
    if(!test(35, Token::Type::Case,      0, "case"))      return false;
    if(!test(36, Token::Type::Default,   0, "default"))   return false;
    if(!test(38, Token::Type::Import,    0, "import"))    return false;
    if(!test(39, Token::Type::As,        0, "as"))        return false;
    if(!test(41, Token::Type::Return,    0, "return"))    return false;
    if(!test(42, Token::Type::Break,     0, "break"))     return false;
    if(!test(43, Token::Type::Continue,  0, "continue"))  return false;
    if(!test(44, Token::Type::GoTo,      0, "goto"))      return false;
    if(!test(46, Token::Type::Func,      0, "func"))      return false;
    if(!test(47, Token::Type::Inline,    0, "inline"))    return false;
    if(!test(48, Token::Type::Operator,  0, "operator"))  return false;
    if(!test(50, Token::Type::RTL,       0, "rtl"))       return false;
    if(!test(51, Token::Type::FSM,       0, "fsm"))       return false;
    if(!test(52, Token::Type::HDL,       0, "hdl"))       return false;
    if(!test(54, Token::Type::Stimulus,  0, "stimulus"))  return false;
    if(!test(55, Token::Type::Emulate,   0, "emulate"))   return false;
    if(!test(56, Token::Type::Assert,    0, "assert"))    return false;
    if(!test(57, Token::Type::Wait,      0, "wait"))      return false;
    if(!test(59, Token::Type::PosEdge,   0, "posedge"))   return false;
    if(!test(60, Token::Type::NegEdge,   0, "negedge"))   return false;
    if(!test(60, Token::Type::EndOfFile, 0, ""))          return false;

    printf(ANSI_FG_GREEN "PASS\n" ANSI_RESET);
    return true;
}
//------------------------------------------------------------------------------

bool testOperators()
{
    if(!startTest("Operators")) return false;

    if(!test(  2, Token::Type::TernaryIf,              0, "?"  )) return false;
    if(!test(  3, Token::Type::TernaryElse,            0, ":"  )) return false;
    if(!test(  4, Token::Type::Elvis,                  0, "?:" )) return false;
    if(!test(  7, Token::Type::BitOr,                  0, "|"  )) return false;
    if(!test(  8, Token::Type::BitNor,                 0, "~|" )) return false;
    if(!test(  9, Token::Type::BitXor,                 0, "^"  )) return false;
    if(!test( 10, Token::Type::BitXnor,                0, "~^" )) return false;
    if(!test( 11, Token::Type::BitAnd,                 0, "&"  )) return false;
    if(!test( 12, Token::Type::BitNand,                0, "~&" )) return false;
    if(!test( 15, Token::Type::Equal,                  0, "==" )) return false;
    if(!test( 16, Token::Type::NotEqual,               0, "!=" )) return false;
    if(!test( 17, Token::Type::Less,                   0, "<"  )) return false;
    if(!test( 18, Token::Type::Greater,                0, ">"  )) return false;
    if(!test( 19, Token::Type::LessEqual,              0, "<=" )) return false;
    if(!test( 20, Token::Type::GreaterEqual,           0, ">=" )) return false;
    if(!test( 23, Token::Type::ShiftLeft,              0, "<<" )) return false;
    if(!test( 24, Token::Type::ShiftRight,             0, ">>" )) return false;
    if(!test( 27, Token::Type::Add,                    0, "+"  )) return false;
    if(!test( 28, Token::Type::Subtract,               0, "-"  )) return false;
    if(!test( 29, Token::Type::Multiply,               0, "*"  )) return false;
    if(!test( 30, Token::Type::Divide,                 0, "/"  )) return false;
    if(!test( 31, Token::Type::Modulus,                0, "%"  )) return false;
    if(!test( 32, Token::Type::Exponential,            0, "**" )) return false;
    if(!test( 33, Token::Type::Factorial,              0, "!"  )) return false;
    if(!test( 36, Token::Type::Replicate,              0, "`"  )) return false;
    if(!test( 37, Token::Type::Concatenate,            0, ":(" )) return false;
    if(!test( 38, Token::Type::ArrayConcatenate,       0, ":[" )) return false;
    if(!test( 41, Token::Type::Stringify,              0, "$"  )) return false;
    if(!test( 42, Token::Type::StringifyExpression,    0, "$(" )) return false;
    if(!test( 45, Token::Type::AndReduce,              0, "&"  )) return false;
    if(!test( 46, Token::Type::NandReduce,             0, "~&" )) return false;
    if(!test( 47, Token::Type::OrReduce,               0, "|"  )) return false;
    if(!test( 48, Token::Type::NorReduce,              0, "~|" )) return false;
    if(!test( 49, Token::Type::XorReduce,              0, "^"  )) return false;
    if(!test( 50, Token::Type::XnorReduce,             0, "~^" )) return false;
    if(!test( 51, Token::Type::LogicalNot,             0, "!"  )) return false;
    if(!test( 54, Token::Type::To,                     0, ".." )) return false;
    if(!test( 55, Token::Type::Step,                   0, ":"  )) return false;
    if(!test( 58, Token::Type::Negate,                 0, "-"  )) return false;
    if(!test( 59, Token::Type::BitNot,                 0, "~"  )) return false;
    if(!test( 60, Token::Type::RawBits,                0, ":"  )) return false;
    if(!test( 61, Token::Type::Increment,              0, "++" )) return false;
    if(!test( 62, Token::Type::Decrement,              0, "--" )) return false;
    if(!test( 65, Token::Type::AccessMember,           0, "."  )) return false;
    if(!test( 66, Token::Type::AccessMemberSafe,       0, "?." )) return false;
    if(!test( 67, Token::Type::AccessMemberPush,       0, ".{" )) return false;
    if(!test( 68, Token::Type::AccessAttribute,        0, "'"  )) return false;
    if(!test( 71, Token::Type::CastOp,                 0, "@"  )) return false;
    if(!test( 74, Token::Type::Assign,                 0, "="  )) return false;
    if(!test( 75, Token::Type::RawAssign,              0, ":=" )) return false;
    if(!test( 76, Token::Type::AppendAssign,           0, "~=" )) return false;
    if(!test( 77, Token::Type::AddAssign,              0, "+=" )) return false;
    if(!test( 78, Token::Type::SubtractAssign,         0, "-=" )) return false;
    if(!test( 79, Token::Type::MultiplyAssign,         0, "*=" )) return false;
    if(!test( 80, Token::Type::DivideAssign,           0, "/=" )) return false;
    if(!test( 81, Token::Type::ModulusAssign,          0, "%=" )) return false;
    if(!test( 82, Token::Type::XorAssign,              0, "^=" )) return false;
    if(!test( 83, Token::Type::AndAssign,              0, "&=" )) return false;
    if(!test( 84, Token::Type::OrAssign,               0, "|=" )) return false;
    if(!test( 85, Token::Type::ExponentialAssign,      0, "**=")) return false;
    if(!test( 86, Token::Type::ShiftLeftAssign,        0, "<<=")) return false;
    if(!test( 87, Token::Type::ShiftRightAssign,       0, ">>=")) return false;
    if(!test( 90, Token::Type::OpenRound,              0, "("  )) return false;
    if(!test( 91, Token::Type::CloseRound,             0, ")"  )) return false;
    if(!test( 92, Token::Type::OpenSquare,             0, "["  )) return false;
    if(!test( 93, Token::Type::CloseSquare,            0, "]"  )) return false;
    if(!test( 94, Token::Type::OpenCurly,              0, "{"  )) return false;
    if(!test( 95, Token::Type::CloseCurly,             0, "}"  )) return false;
    if(!test( 96, Token::Type::OpenAngle,              0, "<"  )) return false;
    if(!test( 97, Token::Type::CloseAngle,             0, ">"  )) return false;
    if(!test( 98, Token::Type::Comma,                  0, ","  )) return false;
    if(!test( 99, Token::Type::Colon,                  0, ":"  )) return false;
    if(!test(100, Token::Type::Semicolon,              0, ";"  )) return false;
    if(!test(103, Token::Type::WaitFor,                0, "#"  )) return false;
    if(!test(104, Token::Type::WaitOn,                 0, "@"  )) return false;
    if(!test(105, Token::Type::WaitCycles,             0, "##" )) return false;
    if(!test(106, Token::Type::SequenceConsecutive,    0, "[*" )) return false;
    if(!test(107, Token::Type::SequenceGoTo,           0, "[->")) return false;
    if(!test(108, Token::Type::SequenceNonConsecutive, 0, "[=" )) return false;
    if(!test(109, Token::Type::AssertImplies,          0, "|->")) return false;
    if(!test(110, Token::Type::AssertImpliesNext,      0, "|=>")) return false;
    if(!test(111, Token::Type::Or,                     0, "||" )) return false;
    if(!test(112, Token::Type::And,                    0, "&&" )) return false;
    if(!test(113, Token::Type::Intersect,              0, "&&&")) return false;
    if(!test(113, Token::Type::EndOfFile,              0, ""   )) return false;

    printf(ANSI_FG_GREEN "PASS\n" ANSI_RESET);
    return true;
}
//------------------------------------------------------------------------------

bool testOthers()
{
    if(!startTest("Others")) return false;

    if(!test(1, Token::Type::Identifier,                  0, "Hello"      )) return false;
    if(!test(2, Token::Type::Literal,                   123, "123"        )) return false;
    if(!test(3, Token::Type::String,                      0, "Hello There")) return false;

    if(!test(21, Token::Type::Identifier, 0, "Some")) return false;
    if(!test(21, Token::Type::Identifier, 0, "text")) return false;
    if(!test(21, Token::Type::Identifier, 0, "and")) return false;

    if(!test(23, Token::Type::Identifier,   0, "tar"       )) return false;
    if(!test(23, Token::Type::Identifier,   0, "Testing"   )) return false;
    if(!test(23, Token::Type::Identifier,   0, "the"       )) return false;
    if(!test(23, Token::Type::Identifier,   0, "scanner"   )) return false;
    if(!test(23, Token::Type::For,          0, "for"       )) return false;
    if(!test(23, Token::Type::While,        0, "while"     )) return false;
    if(!test(23, Token::Type::If,           0, "if"        )) return false;
    if(!test(24, Token::Type::Identifier,   0, "ςιτη"      )) return false;
    if(!test(24, Token::Type::Identifier,   0, "σομε"      )) return false;
    if(!test(24, Token::Type::Identifier,   0, "γρεεκ"     )) return false;
    if(!test(24, Token::Type::Identifier,   0, "ψηαραψτερσ")) return false;
    if(!test(24, Token::Type::Identifier,   0, "τηροςν"    )) return false;
    if(!test(24, Token::Type::Identifier,   0, "ιν"        )) return false;
    if(!test(24, Token::Type::AccessMember, 0, "."         )) return false;
    if(!test(26, Token::Type::Identifier,   0, "And"       )) return false;
    if(!test(26, Token::Type::Identifier,   0, "some"      )) return false;
    if(!test(26, Token::Type::Identifier,   0, "numbers"   )) return false;
    if(!test(26, Token::Type::Colon,        0, ":"         )) return false;
    if(!test(27, Token::Type::Identifier,   0, "ABC"       )) return false;

    if(!test(29, Token::Type::Literal, Number(158422625, 1000), "0b1001_1010_1011_0101_1010_1000p-6")) return false;
    if(!test(30, Token::Type::Literal, Number(158422625, 1000), "0x___9____A____B____5____A____8p-6")) return false;

    if(!test(32, Token::Type::Literal, (double)0x09a935fe50, "0x09A935Fe50")) return false;
    if(!test(33, Token::Type::Literal, 27264, "0b011010101p7")) return false;

    if(!test(35, Token::Type::Literal, Number(123456, 1000), "123.456")) return false;
    if(!test(35, Token::Type::CastOp,     0, "@" )) return false;
    if(!test(35, Token::Type::OpenRound,  0, "(" )) return false;
    if(!test(35, Token::Type::Literal,   16, "16")) return false;
    if(!test(35, Token::Type::Comma,      0, "," )) return false;
    if(!test(35, Token::Type::Literal,    8, "8" )) return false;
    if(!test(35, Token::Type::CloseRound, 0, ")" )) return false;

    if(!test(36, Token::Type::Negate,        0, "-"        )) return false;
    if(!test(36, Token::Type::Literal, 1269632, "0x9AFCp+5")) return false;
    if(!test(36, Token::Type::CastOp,        0, "@"        )) return false;
    if(!test(36, Token::Type::Literal,      24, "24"       )) return false;

    if(!test(37, Token::Type::Literal, Number(0x3774F, 128), "0o673517p-7")) return false;

    if(!test(39, Token::Type::Literal, Number(0x09A935F, 0x10000),    "0x09A.935F"     )) return false;
    if(!test(40, Token::Type::Literal, Number(0xD5, 0x20),            "0b0110.10101"   )) return false;
    if(!test(42, Token::Type::Literal, Number(0x09A935F, 0x10000000), "0x09A.935Fp-12" )) return false;
    if(!test(43, Token::Type::Literal, Number(0x354),                 "0b0110.10101p+7")) return false;

    if(!test(45, Token::Type::Identifier, 0, "Or"       )) return false;
    if(!test(45, Token::Type::Identifier, 0, "different")) return false;
    if(!test(45, Token::Type::Identifier, 0, "style"    )) return false;
    if(!test(45, Token::Type::Identifier, 0, "numbers"  )) return false;
    if(!test(45, Token::Type::Colon,      0, ":"        )) return false;
    if(!test(46, Token::Type::Negate,     0, "-"        )) return false;
    if(!test(46, Token::Type::Literal,    Number(0x1BA827CE, 0x10000), "0x1B_A8.27_CE")) return false;
    if(!test(46, Token::Type::CastOp,     0, "@"        )) return false;
    if(!test(46, Token::Type::OpenRound,  0, "("        )) return false;
    if(!test(46, Token::Type::Literal,   16, "16"       )) return false;
    if(!test(46, Token::Type::Comma,      0, ","        )) return false;
    if(!test(46, Token::Type::Literal,   16, "16"       )) return false;
    if(!test(46, Token::Type::CloseRound, 0, ")"        )) return false;

    if(!test(48, Token::Type::String,    0, "A string \" with escaped \" stuff""Ω""\n")) return false;
    if(!test(49, Token::Type::Semicolon, 0, ";")) return false;
    if(!test(51, Token::Type::String,    0, "\x37""Fa"                )) return false;
    if(!test(51, Token::Type::Semicolon, 0, ";")) return false;
    if(!test(52, Token::Type::String,    0, "\xE1\x88\xB4"            )) return false;
    if(!test(52, Token::Type::Semicolon, 0, ";")) return false;
    if(!test(53, Token::Type::String,    0, "\xFC\x92\xB7\xAA\xBC\xB4")) return false;
    if(!test(53, Token::Type::Semicolon, 0, ";")) return false;
    if(!test(54, Token::Type::String,    0, "\x7A"                    )) return false;
    if(!test(54, Token::Type::Semicolon, 0, ";")) return false;
    if(!test(56, Token::Type::String,    0, "A multi-line\nstring"    )) return false;

    if(!test(59, Token::Type::Identifier,   0, "Base"            )) return false;
    if(!test(59, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(59, Token::Type::Identifier,   0, "Member"          )) return false;
    if(!test(59, Token::Type::OpenRound,    0, "("               )) return false;
    if(!test(59, Token::Type::CloseRound,   0, ")"               )) return false;
    if(!test(59, Token::Type::Semicolon,    0, ";"               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "Identifier"      )) return false;
    if(!test(60, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "with"            )) return false;
    if(!test(60, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "structure"       )) return false;
    if(!test(60, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "and"             )) return false;
    if(!test(60, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "u0837"           )) return false;
    if(!test(60, Token::Type::AccessMember, 0, "."               )) return false;
    if(!test(60, Token::Type::Identifier,   0, "stuff"           )) return false;
    if(!test(62, Token::Type::Identifier,   0, "a"               )) return false;
    if(!test(62, Token::Type::Assign,       0, "="               )) return false;
    if(!test(62, Token::Type::Identifier,   0, "b"               )) return false;
    if(!test(62, Token::Type::Divide,       0, "/"               )) return false;
    if(!test(62, Token::Type::Identifier,   0, "c"               )) return false;
    if(!test(62, Token::Type::Semicolon,    0, ";"               )) return false;
    if(!test(64, Token::Type::String,       0, "This string should be concatenated with this one")) return false;
    if(!test(68, Token::Type::Import,       0, "import"          )) return false;
    if(!test(68, Token::Type::String,       0, "Some Header File")) return false;

    if(!test(81, Token::Type::Increment,      0, "++")) return false;
    if(!test(81, Token::Type::Decrement,      0, "--")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::Equal,          0, "==")) return false;
    if(!test(81, Token::Type::Assign,         0, "=" )) return false;
    if(!test(81, Token::Type::Decrement,      0, "--")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::SubtractAssign, 0, "-=")) return false;
    if(!test(81, Token::Type::Subtract,       0, "-" )) return false;
    if(!test(81, Token::Type::Increment,      0, "++")) return false;
    if(!test(81, Token::Type::Increment,      0, "++")) return false;
    if(!test(81, Token::Type::Add,            0, "+" )) return false;

    if(!test(82, Token::Type::CastOp,     0, "@")) return false;
    if(!test(82, Token::Type::OpenCurly,  0, "{")) return false;
    if(!test(83, Token::Type::CloseCurly, 0, "}")) return false;

    if(!test(85, Token::Type::String, 0,
             "This is a string of keywords private pin net\n"
             "\n \t \v \b \r \f \a \\ \? \' \" "
             "\x0F""BC \xE2\x86\x92""AB \xF0\x9F\x98\x81""EFEFEFE \xF0\x9F\x98\x8E "
             "\xE2\x80\x93\n"
             "Some more string")) return false;

    Number n; n = 1234567890; n.mul(1000000000); n.add(987654321);
    if(!test(90, Token::Type::Literal, n, "1234567890987654321.0")) return false;
    Number m = n; m.div(1000000000); m.div(10000000000);
    if(!test(91, Token::Type::Literal, m, "0.1234567890987654321")) return false;
    n.add(m);
    for(int e = 0; e < 50; e++) n.div(10);
    if(!test(92, Token::Type::Literal, n, "1234567890987654321.1234567890987654321e-50")) return false;
    if(!test(93, Token::Type::Literal, 123, "123.000")) return false;

    if(!test(95, Token::Type::Literal, 123, "123_")) return false;
    if(!test(95, Token::Type::Add, 0, "+")) return false;
    n = 794; n.div(100000000); n.mul(0, 1);
    if(!test(95, Token::Type::Literal, n, "7.94e-6__j___")) return false;

    if(!test(96, Token::Type::Literal,  9, "9__")) return false;
    if(!test(96, Token::Type::Subtract, 0, "-")) return false;
    n = 427; n.div(100); n.mul(0, 16);
    if(!test(96, Token::Type::Literal, n, "4.27p4___i___")) return false;

    if(!test(96, Token::Type::EndOfFile, 0, "")) return false;

    printf(ANSI_FG_GREEN "PASS\n" ANSI_RESET);
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    Scanner _scanner;
    scanner = &_scanner;
    setupTerminal();

    printf("\n\n");
    if(!testKeywords() ) goto MainError;
    if(!testOperators()) goto MainError;
    if(!testOthers()   ) goto MainError;

    printf(ANSI_FG_GREEN "All OK\n\n" ANSI_RESET);
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        return -1;
}
//------------------------------------------------------------------------------

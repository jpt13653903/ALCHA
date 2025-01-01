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

struct Expected{
    int         line;
    Token::Type type;
    Number      value;
    const char* data;
};
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s... ", name); \

    string filename;
    filename  = "testScanner/";
    filename += name;
    filename += ".alc";

    if(!scanner->open(filename.c_str())){
        error("Cannot open file %s", filename.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

inline char to_string(int i)
{
    return "0123456789abcdef"[i];
}
//------------------------------------------------------------------------------

string expand(const string& s)
{
    string result;
    for(auto c: s){
        if(c == '\e' || c >= 0x20){
            result += c;
        }else{
            unsigned u = (unsigned char)c;
            result += "\\x";
            result += to_string(u / 16);
            result += to_string(u % 16);
        }
    }
    return result;
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
           ANSI_FG_GREEN      "    Expected: %s\n"
           ANSI_FG_BRIGHT_RED "    Got:      %s\n"
           ANSI_RESET,
           expand(expected).c_str(),
           expand(got).c_str());
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

bool runTest(Expected* expected)
{
    int n;
    for(n = 0; expected[n].type != Token::Type::EndOfFile; n++){
        if(!test(expected[n].line, expected[n].type, expected[n].value, expected[n].data))
            return false;
    }
    if(!test(expected[n].line, expected[n].type, expected[n].value, expected[n].data))
        return false;
    return true;
}
//------------------------------------------------------------------------------

void endTest()
{
    printf(ANSI_FG_GREEN "PASS\n" ANSI_RESET);
}
//------------------------------------------------------------------------------

bool testKeywords()
{
    if(!startTest("Keywords")) return false;

    Expected expected[] = {
        {  1, Token::Type::Void,      0, "void"      },
        {  2, Token::Type::Auto,      0, "auto"      },
        {  4, Token::Type::Pin,       0, "pin"       },
        {  5, Token::Type::Net,       0, "net"       },
        {  7, Token::Type::Byte,      0, "byte"      },
        {  8, Token::Type::Char,      0, "char"      },
        {  9, Token::Type::Num,       0, "num"       },
        { 11, Token::Type::True,      0, "true"      },
        { 12, Token::Type::False,     0, "false"     },
        { 14, Token::Type::Class,     0, "class"     },
        { 15, Token::Type::Enum,      0, "enum"      },
        { 16, Token::Type::Struct,    0, "struct"    },
        { 17, Token::Type::Group,     0, "group"     },
        { 18, Token::Type::Alias,     0, "alias"     },
        { 20, Token::Type::Input,     0, "input"     },
        { 21, Token::Type::Output,    0, "output"    },
        { 23, Token::Type::Public,    0, "public"    },
        { 24, Token::Type::Private,   0, "private"   },
        { 25, Token::Type::Protected, 0, "protected" },
        { 27, Token::Type::If,        0, "if"        },
        { 28, Token::Type::Else,      0, "else"      },
        { 29, Token::Type::For,       0, "for"       },
        { 30, Token::Type::In,        0, "in"        },
        { 31, Token::Type::While,     0, "while"     },
        { 32, Token::Type::Loop,      0, "loop"      },
        { 34, Token::Type::Switch,    0, "switch"    },
        { 35, Token::Type::Case,      0, "case"      },
        { 36, Token::Type::Default,   0, "default"   },
        { 38, Token::Type::Import,    0, "import"    },
        { 39, Token::Type::As,        0, "as"        },
        { 41, Token::Type::Return,    0, "return"    },
        { 42, Token::Type::Break,     0, "break"     },
        { 43, Token::Type::Continue,  0, "continue"  },
        { 44, Token::Type::GoTo,      0, "goto"      },
        { 46, Token::Type::Func,      0, "func"      },
        { 47, Token::Type::Inline,    0, "inline"    },
        { 48, Token::Type::Operator,  0, "operator"  },
        { 50, Token::Type::RTL,       0, "rtl"       },
        { 51, Token::Type::FSM,       0, "fsm"       },
        { 52, Token::Type::HDL,       0, "hdl"       },
        { 54, Token::Type::Stimulus,  0, "stimulus"  },
        { 55, Token::Type::Emulate,   0, "emulate"   },
        { 56, Token::Type::Sequence,  0, "sequence"  },
        { 57, Token::Type::Assert,    0, "assert"    },
        { 58, Token::Type::Wait,      0, "wait"      },
        { 60, Token::Type::PosEdge,   0, "posedge"   },
        { 61, Token::Type::NegEdge,   0, "negedge"   },
        { 61, Token::Type::EndOfFile, 0, ""          }
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testOperators()
{
    if(!startTest("Operators")) return false;

    Expected expected[] = {
        {   2, Token::Type::TernaryIf,                0, "?"   },
        {   3, Token::Type::TernaryElse,              0, ":"   },
        {   4, Token::Type::Elvis,                    0, "?:"  },
        {   7, Token::Type::BitOr,                    0, "|"   },
        {   8, Token::Type::BitNor,                   0, "~|"  },
        {   9, Token::Type::BitXor,                   0, "^"   },
        {  10, Token::Type::BitXnor,                  0, "~^"  },
        {  11, Token::Type::BitAnd,                   0, "&"   },
        {  12, Token::Type::BitNand,                  0, "~&"  },
        {  15, Token::Type::Equal,                    0, "=="  },
        {  16, Token::Type::NotEqual,                 0, "!="  },
        {  17, Token::Type::Less,                     0, "<"   },
        {  18, Token::Type::Greater,                  0, ">"   },
        {  19, Token::Type::LessEqual,                0, "<="  },
        {  20, Token::Type::GreaterEqual,             0, ">="  },
        {  23, Token::Type::ShiftLeft,                0, "<<"  },
        {  24, Token::Type::ShiftRight,               0, ">>"  },
        {  27, Token::Type::Add,                      0, "+"   },
        {  28, Token::Type::Subtract,                 0, "-"   },
        {  29, Token::Type::Multiply,                 0, "*"   },
        {  30, Token::Type::Divide,                   0, "/"   },
        {  31, Token::Type::Modulus,                  0, "%"   },
        {  32, Token::Type::Exponential,              0, "**"  },
        {  33, Token::Type::Factorial,                0, "!"   },
        {  36, Token::Type::Replicate,                0, "`"   },
        {  37, Token::Type::Concatenate,              0, ":("  },
        {  38, Token::Type::ArrayConcatenate,         0, ":["  },
        {  41, Token::Type::Stringify,                0, "$"   },
        {  42, Token::Type::StringifyExpression,      0, "$("  },
        {  45, Token::Type::AndReduce,                0, "&"   },
        {  46, Token::Type::NandReduce,               0, "~&"  },
        {  47, Token::Type::OrReduce,                 0, "|"   },
        {  48, Token::Type::NorReduce,                0, "~|"  },
        {  49, Token::Type::XorReduce,                0, "^"   },
        {  50, Token::Type::XnorReduce,               0, "~^"  },
        {  51, Token::Type::LogicalNot,               0, "!"   },
        {  54, Token::Type::To,                       0, ".."  },
        {  55, Token::Type::Step,                     0, ":"   },
        {  58, Token::Type::Negate,                   0, "-"   },
        {  59, Token::Type::BitNot,                   0, "~"   },
        {  60, Token::Type::RawBits,                  0, ":"   },
        {  61, Token::Type::Increment,                0, "++"  },
        {  62, Token::Type::Decrement,                0, "--"  },
        {  65, Token::Type::AccessMember,             0, "."   },
        {  66, Token::Type::AccessMemberSafe,         0, "?."  },
        {  67, Token::Type::AccessMemberPush,         0, ".{"  },
        {  68, Token::Type::AccessAttribute,          0, "'"   },
        {  71, Token::Type::CastOp,                   0, "@"   },
        {  74, Token::Type::Assign,                   0, "="   },
        {  75, Token::Type::RawAssign,                0, ":="  },
        {  76, Token::Type::AppendAssign,             0, "~="  },
        {  77, Token::Type::AddAssign,                0, "+="  },
        {  78, Token::Type::SubtractAssign,           0, "-="  },
        {  79, Token::Type::MultiplyAssign,           0, "*="  },
        {  80, Token::Type::DivideAssign,             0, "/="  },
        {  81, Token::Type::ModulusAssign,            0, "%="  },
        {  82, Token::Type::XorAssign,                0, "^="  },
        {  83, Token::Type::AndAssign,                0, "&="  },
        {  84, Token::Type::OrAssign,                 0, "|="  },
        {  85, Token::Type::ExponentialAssign,        0, "**=" },
        {  86, Token::Type::ShiftLeftAssign,          0, "<<=" },
        {  87, Token::Type::ShiftRightAssign,         0, ">>=" },
        {  90, Token::Type::OpenRound,                0, "("   },
        {  91, Token::Type::CloseRound,               0, ")"   },
        {  92, Token::Type::OpenSquare,               0, "["   },
        {  93, Token::Type::CloseSquare,              0, "]"   },
        {  94, Token::Type::OpenCurly,                0, "{"   },
        {  95, Token::Type::CloseCurly,               0, "}"   },
        {  96, Token::Type::OpenAngle,                0, "<"   },
        {  97, Token::Type::CloseAngle,               0, ">"   },
        {  98, Token::Type::Comma,                    0, ","   },
        {  99, Token::Type::Colon,                    0, ":"   },
        { 100, Token::Type::Semicolon,                0, ";"   },
        { 103, Token::Type::WaitFor,                  0, "#"   },
        { 104, Token::Type::WaitOn,                   0, "@"   },
        { 105, Token::Type::WaitCycles,               0, "##"  },
        { 106, Token::Type::RepetitionConsecutive,    0, "[*"  },
        { 107, Token::Type::RepetitionGoTo,           0, "[->" },
        { 108, Token::Type::RepetitionNonConsecutive, 0, "[="  },
        { 109, Token::Type::AssertImplies,            0, "|->" },
        { 110, Token::Type::AssertImpliesNext,        0, "|=>" },
        { 111, Token::Type::Or,                       0, "||"  },
        { 112, Token::Type::And,                      0, "&&"  },
        { 113, Token::Type::Intersect,                0, "&&&" },
        { 113, Token::Type::EndOfFile,                0, ""    }
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testOthers()
{
    if(!startTest("Others")) return false;

    Number value[5];

    Number n; n = 1234567890; n.mul(1000000000); n.add(987654321);
    value[0] = n;

    Number m = n; m.div(1000000000); m.div(10000000000);
    value[1] = m;

    n += m;
    for(int e = 0; e < 50; e++) n.div(10);
    value[2] = n;

    n = 794; n.div(100000000); n.mul(0, 1);
    value[3] = n;

    n = 427; n.div(100); n.mul(0, 16);
    value[4] = n;

    Expected expected[] = {
        {   1, Token::Type::Identifier, 0, "Hello"       },
        {   2, Token::Type::Literal,  123, "123"         },
        {   3, Token::Type::String,     0, "Hello There" },

        {  21, Token::Type::Identifier, 0, "Some" },
        {  21, Token::Type::Identifier, 0, "text" },
        {  21, Token::Type::Identifier, 0, "and"  },

        {  23, Token::Type::Identifier,   0, "tar"        },
        {  23, Token::Type::Identifier,   0, "Testing"    },
        {  23, Token::Type::Identifier,   0, "the"        },
        {  23, Token::Type::Identifier,   0, "scanner"    },
        {  23, Token::Type::For,          0, "for"        },
        {  23, Token::Type::While,        0, "while"      },
        {  23, Token::Type::If,           0, "if"         },
        {  24, Token::Type::Identifier,   0, "ςιτη"       },
        {  24, Token::Type::Identifier,   0, "σομε"       },
        {  24, Token::Type::Identifier,   0, "γρεεκ"      },
        {  24, Token::Type::Identifier,   0, "ψηαραψτερσ" },
        {  24, Token::Type::Identifier,   0, "τηροςν"     },
        {  24, Token::Type::Identifier,   0, "ιν"         },
        {  24, Token::Type::AccessMember, 0, "."          },
        {  26, Token::Type::Identifier,   0, "And"        },
        {  26, Token::Type::Identifier,   0, "some"       },
        {  26, Token::Type::Identifier,   0, "numbers"    },
        {  26, Token::Type::Colon,        0, ":"          },
        {  27, Token::Type::Identifier,   0, "ABC"        },

        {  29, Token::Type::Literal, Number(158422625) / 1000, "0b1001_1010_1011_0101_1010_1000p-6" },
        {  30, Token::Type::Literal, Number(158422625) / 1000, "0x___9____A____B____5____A____8p-6" },

        {  32, Token::Type::Literal, (double)0x09a935fe50, "0x09A935Fe50" },
        {  33, Token::Type::Literal, 27264, "0b011010101p7" },

        {  35, Token::Type::Literal, Number(123456) / 1000, "123.456" },
        {  35, Token::Type::CastOp,     0, "@"  },
        {  35, Token::Type::OpenRound,  0, "("  },
        {  35, Token::Type::Literal,   16, "16" },
        {  35, Token::Type::Comma,      0, ","  },
        {  35, Token::Type::Literal,    8, "8"  },
        {  35, Token::Type::CloseRound, 0, ")"  },

        {  36, Token::Type::Negate,        0, "-"         },
        {  36, Token::Type::Literal, 1269632, "0x9AFCp+5" },
        {  36, Token::Type::CastOp,        0, "@"         },
        {  36, Token::Type::Literal,      24, "24"        },

        {  37, Token::Type::Literal, Number(0x3774F) / 128, "0o673517p-7" },

        {  39, Token::Type::Literal, Number(0x09A935F) / 0x10000,    "0x09A.935F"      },
        {  40, Token::Type::Literal, Number(0xD5) / 0x20,            "0b0110.10101"    },
        {  42, Token::Type::Literal, Number(0x09A935F) / 0x10000000, "0x09A.935Fp-12"  },
        {  43, Token::Type::Literal, Number(0x354),                  "0b0110.10101p+7" },

        {  45, Token::Type::Identifier, 0, "Or"        },
        {  45, Token::Type::Identifier, 0, "different" },
        {  45, Token::Type::Identifier, 0, "style"     },
        {  45, Token::Type::Identifier, 0, "numbers"   },
        {  45, Token::Type::Colon,      0, ":"         },
        {  46, Token::Type::Negate,     0, "-"         },
        {  46, Token::Type::Literal,    Number(0x1BA827CE) / 0x10000, "0x1B_A8.27_CE" },
        {  46, Token::Type::CastOp,     0, "@"         },
        {  46, Token::Type::OpenRound,  0, "("         },
        {  46, Token::Type::Literal,   16, "16"        },
        {  46, Token::Type::Comma,      0, ","         },
        {  46, Token::Type::Literal,   16, "16"        },
        {  46, Token::Type::CloseRound, 0, ")"         },

        {  48, Token::Type::String,    0, "A string \" with escaped \" stuff""Ω""\n" },
        {  49, Token::Type::Semicolon, 0, ";" },
        {  51, Token::Type::String,    0, "\x37""Fa"                 },
        {  51, Token::Type::Semicolon, 0, ";" },
        {  52, Token::Type::String,    0, "\xE1\x88\xB4"             },
        {  52, Token::Type::Semicolon, 0, ";" },
        {  53, Token::Type::String,    0, "\xFC\x92\xB7\xAA\xBC\xB4" },
        {  53, Token::Type::Semicolon, 0, ";" },
        {  54, Token::Type::String,    0, "\x7A"                     },
        {  54, Token::Type::Semicolon, 0, ";" },
        {  56, Token::Type::String,    0, "A multi-line\nstring"     },

        {  59, Token::Type::Identifier,   0, "Base"             },
        {  59, Token::Type::AccessMember, 0, "."                },
        {  59, Token::Type::Identifier,   0, "Member"           },
        {  59, Token::Type::OpenRound,    0, "("                },
        {  59, Token::Type::CloseRound,   0, ")"                },
        {  59, Token::Type::Semicolon,    0, ";"                },
        {  60, Token::Type::Identifier,   0, "Identifier"       },
        {  60, Token::Type::AccessMember, 0, "."                },
        {  60, Token::Type::Identifier,   0, "with"             },
        {  60, Token::Type::AccessMember, 0, "."                },
        {  60, Token::Type::Identifier,   0, "structure"        },
        {  60, Token::Type::AccessMember, 0, "."                },
        {  60, Token::Type::Identifier,   0, "and"              },
        {  60, Token::Type::AccessMember, 0, "."                },
        {  60, Token::Type::Identifier,   0, "u0837"            },
        {  60, Token::Type::AccessMember, 0, "."                },
        {  60, Token::Type::Identifier,   0, "stuff"            },
        {  62, Token::Type::Identifier,   0, "a"                },
        {  62, Token::Type::Assign,       0, "="                },
        {  62, Token::Type::Identifier,   0, "b"                },
        {  62, Token::Type::Divide,       0, "/"                },
        {  62, Token::Type::Identifier,   0, "c"                },
        {  62, Token::Type::Semicolon,    0, ";"                },
        {  64, Token::Type::String,       0, "This string should be concatenated with this one" },
        {  68, Token::Type::Import,       0, "import"           },
        {  68, Token::Type::String,       0, "Some Header File" },

        {  81, Token::Type::Increment,      0, "++" },
        {  81, Token::Type::Decrement,      0, "--" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::Equal,          0, "==" },
        {  81, Token::Type::Assign,         0, "="  },
        {  81, Token::Type::Decrement,      0, "--" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::SubtractAssign, 0, "-=" },
        {  81, Token::Type::Subtract,       0, "-"  },
        {  81, Token::Type::Increment,      0, "++" },
        {  81, Token::Type::Increment,      0, "++" },
        {  81, Token::Type::Add,            0, "+"  },

        {  82, Token::Type::CastOp,     0, "@" },
        {  82, Token::Type::OpenCurly,  0, "{" },
        {  83, Token::Type::CloseCurly, 0, "}" },

        {  85, Token::Type::String, 0,
           "This is a string of keywords private pin net\n"
           "\n \t \v \b \r \f \e \a \\ \? \' \" "
           "\xc2\xab"
           " \x0F"
           "BC \xE2\x86\x92"
           "AB \xF0\x9F\x98\x81"
           "EFEFEFE \xF0\x9F\x98\x8E "
           "\xE2\x80\x93\n"
           "Some more string" },

        {  90, Token::Type::Literal, value[0], "1234567890987654321.0" },
        {  91, Token::Type::Literal, value[1], "0.1234567890987654321" },
        {  92, Token::Type::Literal, value[2], "1234567890987654321.1234567890987654321e-50" },
        {  93, Token::Type::Literal, 123,      "123.000" },

        {  95, Token::Type::Literal, 123,      "123_"          },
        {  95, Token::Type::Add,       0,      "+"             },
        {  95, Token::Type::Literal, value[3], "7.94e-6__j___" },
        {  96, Token::Type::Literal,   9,      "9__"           },
        {  96, Token::Type::Subtract , 0,      "-"             },
        {  96, Token::Type::Literal, value[4], "4.27p4___i___" },

        {  98, Token::Type::InterpolatedStringPart, 0, "I have " },
        {  98, Token::Type::Identifier,             0, "x"       },
        {  98, Token::Type::InterpolatedStringEnd,  0, " sheep"  },
        {  98, Token::Type::Semicolon,              0, ";"       },

        {  99, Token::Type::InterpolatedStringPart, 0, "I have " },
        {  99, Token::Type::Identifier,             0, "x"       },
        {  99, Token::Type::Add,                    0, "+"       },
        {  99, Token::Type::Identifier,             0, "b"       },
        {  99, Token::Type::InterpolatedStringEnd,  0, " sheep"  },
        {  99, Token::Type::Semicolon,              0, ";"       },

        { 100, Token::Type::InterpolatedStringPart, 0, "I have " },
        { 100, Token::Type::Identifier,             0, "x"  },
        { 100, Token::Type::InterpolatedStringPart, 0, " sheep and " },
        { 100, Token::Type::Identifier,             0, "y"  },
        { 100, Token::Type::InterpolatedStringPart, 0, " goats and this part is not interpolated { }while this one is the " },
        { 102, Token::Type::Literal,                3, "3"  },
        { 102, Token::Type::InterpolatedStringPart, 0, "rd string.and this the " },
        { 103, Token::Type::Literal,                4, "4"  },
        { 103, Token::Type::InterpolatedStringEnd,  0, "th" },
        { 103, Token::Type::Semicolon,              0, ";"  },

        { 105, Token::Type::InterpolatedStringPart, 0, "I have " },
        { 105, Token::Type::Identifier,             0, "x"       },
        { 105, Token::Type::Add,                    0, "+"       },
        { 105, Token::Type::Identifier,             0, "b"       },
        { 105, Token::Type::Comma,                  0, ","       },
        { 105, Token::Type::String,                 0, "04d"     },
        { 105, Token::Type::InterpolatedStringEnd,  0, " sheep"  },
        { 105, Token::Type::Semicolon,              0, ";"       },

        { 106, Token::Type::InterpolatedStringPart, 0, "I have " },
        { 106, Token::Type::Identifier,             0, "x"       },
        { 106, Token::Type::Add,                    0, "+"       },
        { 106, Token::Type::Identifier,             0, "b"       },
        { 106, Token::Type::Comma,                  0, ","       },
        { 106, Token::Type::Identifier,             0, "format"  },
        { 106, Token::Type::InterpolatedStringEnd,  0, " sheep"  },
        { 106, Token::Type::Semicolon,              0, ";"       },

        { 108, Token::Type::Identifier,             0, "MyClass"            },
        { 109, Token::Type::Identifier,             0, "myInstance"         },
        { 110, Token::Type::Identifier,             0, "_my_local"          },
        { 111, Token::Type::Identifier,             0, "ξένηΜεταβλητή"      },
        { 112, Token::Type::Identifier,             0, "адәныҟатәиАԥсахга"  },
        { 113, Token::Type::Identifier,             0, "внешняяПеременная"  },
        { 114, Token::Type::Identifier,             0, "متغيرأجنبي"         },
        { 115, Token::Type::Identifier,             0, "ตัวแปรต่างประเทศ"   },
        { 116, Token::Type::Identifier,             0, "משתנהזר"            },
        { 117, Token::Type::Identifier,             0, "विदेशीचर"           },
        { 118, Token::Type::Identifier,             0, "外部变量"           },
        { 119, Token::Type::Identifier,             0, "외국변수"           },

        { 119, Token::Type::EndOfFile, 0, "" },
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    Scanner _scanner;
    scanner = &_scanner;
    setupTerminal();

    printf("\n");
    if(!testKeywords ()) goto MainError;
    if(!testOperators()) goto MainError;
    if(!testOthers   ()) goto MainError;

    printf(ANSI_FG_GREEN "\nAll OK\n" ANSI_FG_BRIGHT_BLACK
           "----------------------------------------"
           "----------------------------------------\n\n"
           ANSI_RESET);
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        return -1;
}
//------------------------------------------------------------------------------

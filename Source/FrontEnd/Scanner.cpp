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

#include "Utilities.h"
#include "TokenTree.h"
#include "FileWrapper.h"
#include "CharacterNames.h"
//------------------------------------------------------------------------------

#include <map>
#include <time.h>

using std::string;
using std::to_string;
using std::map;
//------------------------------------------------------------------------------

static bool initialised = false;

static TokenTree spaces;
static TokenTree keywords;
static TokenTree operators;

typedef map<string, const byte*> Characters;
static Characters characters;
//------------------------------------------------------------------------------

Scanner::Scanner()
{
    if(!initialised){
        // Spaces
            spaces.insert("\x20"        , Token::Type::Space); // U+0020: Space
            spaces.insert("\x09"        , Token::Type::Space); // U+0009: Character Tabulation
            spaces.insert("\xC2\xA0"    , Token::Type::Space); // U+00A0: No-break Space
            spaces.insert("\xE1\x9A\x80", Token::Type::Space); // U+1680: Ogham Space Mark
            spaces.insert("\xE2\x80\x80", Token::Type::Space); // U+2000: En Quad
            spaces.insert("\xE2\x80\x81", Token::Type::Space); // U+2001: Em Quad
            spaces.insert("\xE2\x80\x82", Token::Type::Space); // U+2002: En Space
            spaces.insert("\xE2\x80\x83", Token::Type::Space); // U+2003: Em Space
            spaces.insert("\xE2\x80\x84", Token::Type::Space); // U+2004: Three-per-em Space
            spaces.insert("\xE2\x80\x85", Token::Type::Space); // U+2005: Four-per-em Space
            spaces.insert("\xE2\x80\x86", Token::Type::Space); // U+2006: Six-per-em Space
            spaces.insert("\xE2\x80\x87", Token::Type::Space); // U+2007: Figure Space
            spaces.insert("\xE2\x80\x88", Token::Type::Space); // U+2008: Punctuation Space
            spaces.insert("\xE2\x80\x89", Token::Type::Space); // U+2009: Thin Space
            spaces.insert("\xE2\x80\x8A", Token::Type::Space); // U+200A: Hair Space
            spaces.insert("\xE2\x80\xAF", Token::Type::Space); // U+202F: Narrow No-break Space
            spaces.insert("\xE2\x80\x8B", Token::Type::Space); // U+200B: Zero Width Space
            spaces.insert("\xE2\x81\x9F", Token::Type::Space); // U+205F: Medium Mathematical Space
            spaces.insert("\xE3\x80\x80", Token::Type::Space); // U+3000: Ideographic Space
            spaces.insert("\xEF\xBB\xBF", Token::Type::Space); // U+FEFF: Zero Width Non-breaking Space

        // Newlines
            spaces.insert("\n"          , Token::Type::Newline);
            spaces.insert("\r"          , Token::Type::Newline);
            spaces.insert("\r\n"        , Token::Type::Newline);
            spaces.insert("\n\r"        , Token::Type::Newline);
            spaces.insert("\x0B"        , Token::Type::Newline); // Vertical Tab
            spaces.insert("\x0C"        , Token::Type::Newline); // Form Feed
            spaces.insert("\xC2\x85"    , Token::Type::Newline); // U+0085: NEL
            spaces.insert("\xE2\x80\xA8", Token::Type::Newline); // U+2028: line Separator
            spaces.insert("\xE2\x80\xA9", Token::Type::Newline); // U+2029: Paragraph Separator

        // Keywords
            keywords.insert("__FILE__"     , Token::Type::File);
            keywords.insert("__LINE__"     , Token::Type::Line);
            keywords.insert("__DATE__"     , Token::Type::Date);
            keywords.insert("__TIME__"     , Token::Type::Time);
            // __CLASS__, __FUNCTION__, etc. are implemented as special variables

            keywords.insert("void"         , Token::Type::Void);
            keywords.insert("auto"         , Token::Type::Auto);

            keywords.insert("pin"          , Token::Type::Pin);
            keywords.insert("net"          , Token::Type::Net);

            keywords.insert("byte"         , Token::Type::Byte);
            keywords.insert("char"         , Token::Type::Char);
            keywords.insert("num"          , Token::Type::Num );

            keywords.insert("true"         , Token::Type::True );
            keywords.insert("false"        , Token::Type::False);

            keywords.insert("class"        , Token::Type::Class );
            keywords.insert("enum"         , Token::Type::Enum  );
            keywords.insert("struct"       , Token::Type::Struct);
            keywords.insert("group"        , Token::Type::Group );
            keywords.insert("alias"        , Token::Type::Alias );

            keywords.insert("input"        , Token::Type::Input );
            keywords.insert("output"       , Token::Type::Output);

            keywords.insert("public"       , Token::Type::Public   );
            keywords.insert("private"      , Token::Type::Private  );
            keywords.insert("protected"    , Token::Type::Protected);

            keywords.insert("if"           , Token::Type::If   );
            keywords.insert("else"         , Token::Type::Else );
            keywords.insert("for"          , Token::Type::For  );
            keywords.insert("in"           , Token::Type::In   );
            keywords.insert("while"        , Token::Type::While);
            keywords.insert("loop"         , Token::Type::Loop );

            keywords.insert("switch"       , Token::Type::Switch );
            keywords.insert("case"         , Token::Type::Case   );
            keywords.insert("default"      , Token::Type::Default);

            keywords.insert("import"       , Token::Type::Import);
            keywords.insert("as"           , Token::Type::As    );

            keywords.insert("return"       , Token::Type::Return  );
            keywords.insert("break"        , Token::Type::Break   );
            keywords.insert("continue"     , Token::Type::Continue);
            keywords.insert("goto"         , Token::Type::GoTo    );

            keywords.insert("func"         , Token::Type::Func  );
            keywords.insert("inline"       , Token::Type::Inline);
            keywords.insert("operator"     , Token::Type::Operator);

            keywords.insert("rtl"          , Token::Type::RTL);
            keywords.insert("fsm"          , Token::Type::FSM);
            keywords.insert("hdl"          , Token::Type::HDL);

            keywords.insert("stimulus"     , Token::Type::Stimulus);
            keywords.insert("emulate"      , Token::Type::Emulate );
            keywords.insert("assert"       , Token::Type::Assert  );
            keywords.insert("wait"         , Token::Type::Wait    );

            keywords.insert("posedge"      , Token::Type::PosEdge);
            keywords.insert("negedge"      , Token::Type::NegEdge);

        // Conditional Expression
            operators.insert("?"  , Token::Type::TernaryIf  );
            operators.insert(":"  , Token::Type::TernaryElse);
            operators.insert("?:" , Token::Type::Elvis      );

        // Bit-wise
            operators.insert("|"  , Token::Type::BitOr     );
            operators.insert("~|" , Token::Type::BitNor    );
            operators.insert("^"  , Token::Type::BitXor    );
            operators.insert("~^" , Token::Type::BitXnor   );
            operators.insert("&"  , Token::Type::BitAnd    );
            operators.insert("~&" , Token::Type::BitNand   );

        // Relational
            operators.insert("==" , Token::Type::Equal       );
            operators.insert("!=" , Token::Type::NotEqual    );
            operators.insert("<"  , Token::Type::Less        );
            operators.insert(">"  , Token::Type::Greater     );
            operators.insert("<=" , Token::Type::LessEqual   );
            operators.insert(">=" , Token::Type::GreaterEqual);

        // Shift
            operators.insert("<<" , Token::Type::ShiftLeft );
            operators.insert(">>" , Token::Type::ShiftRight);

        // Arithmetic
            operators.insert("+"  , Token::Type::Add        );
            operators.insert("-"  , Token::Type::Subtract   );
            operators.insert("*"  , Token::Type::Multiply   );
            operators.insert("/"  , Token::Type::Divide     );
            operators.insert("%"  , Token::Type::Modulus    );
            operators.insert("**" , Token::Type::Exponential);
            operators.insert("!"  , Token::Type::Factorial  );

        // Concatenation of vectors and arrays
            operators.insert("`"  , Token::Type::Replicate       );
            operators.insert(":(" , Token::Type::Concatenate     );
            operators.insert(":[" , Token::Type::ArrayConcatenate);

        // Stringification
            operators.insert("$"  , Token::Type::Stringify          );
            operators.insert("$(" , Token::Type::StringifyExpression);

        // Reduction
            operators.insert("&"  , Token::Type::AndReduce );
            operators.insert("~&" , Token::Type::NandReduce);
            operators.insert("|"  , Token::Type::OrReduce  );
            operators.insert("~|" , Token::Type::NorReduce );
            operators.insert("^"  , Token::Type::XorReduce );
            operators.insert("~^" , Token::Type::XnorReduce);
            operators.insert("!"  , Token::Type::LogicalNot);

        // Array
            operators.insert(".." , Token::Type::To  );
            operators.insert(":"  , Token::Type::Step);

        // Unary
            operators.insert("-"  , Token::Type::Negate   );
            operators.insert("~"  , Token::Type::BitNot  );
            operators.insert(":"  , Token::Type::RawBits  );
            operators.insert("++" , Token::Type::Increment);
            operators.insert("--" , Token::Type::Decrement);

        // Accessors
            operators.insert("."  , Token::Type::AccessMember    );
            operators.insert("?." , Token::Type::AccessMemberSafe);
            operators.insert(".{" , Token::Type::AccessMemberPush);
            operators.insert("'"  , Token::Type::AccessAttribute );

        // Cast
            operators.insert("@"  , Token::Type::CastOp);

        // Assignment
            operators.insert("="  , Token::Type::Assign           );
            operators.insert(":=" , Token::Type::RawAssign        );
            operators.insert("~=" , Token::Type::AppendAssign     );
            operators.insert("+=" , Token::Type::AddAssign        );
            operators.insert("-=" , Token::Type::SubtractAssign   );
            operators.insert("*=" , Token::Type::MultiplyAssign   );
            operators.insert("/=" , Token::Type::DivideAssign     );
            operators.insert("**=", Token::Type::ExponentialAssign);
            operators.insert("%=" , Token::Type::ModulusAssign    );
            operators.insert("&=" , Token::Type::AndAssign        );
            operators.insert("|=" , Token::Type::OrAssign         );
            operators.insert("^=" , Token::Type::XorAssign        );
            operators.insert("<<=", Token::Type::ShiftLeftAssign  );
            operators.insert(">>=", Token::Type::ShiftRightAssign );

        // Punctuators
            operators.insert("("  , Token::Type::OpenRound  );
            operators.insert(")"  , Token::Type::CloseRound );
            operators.insert("["  , Token::Type::OpenSquare );
            operators.insert("]"  , Token::Type::CloseSquare);
            operators.insert("{"  , Token::Type::OpenCurly  );
            operators.insert("}"  , Token::Type::CloseCurly );
            operators.insert("<"  , Token::Type::OpenAngle  );
            operators.insert(">"  , Token::Type::CloseAngle );
            operators.insert(","  , Token::Type::Comma      );
            operators.insert(":"  , Token::Type::Colon      );
            operators.insert(";"  , Token::Type::Semicolon  );

        // Simulation operators
            operators.insert("#"  , Token::Type::WaitFor               );
            operators.insert("@"  , Token::Type::WaitOn                );
            operators.insert("##" , Token::Type::WaitCycles            );
            operators.insert("[*" , Token::Type::SequenceConsecutive   );
            operators.insert("[->", Token::Type::SequenceGoTo          );
            operators.insert("[=" , Token::Type::SequenceNonConsecutive);
            operators.insert("|->", Token::Type::AssertImplies         );
            operators.insert("|=>", Token::Type::AssertImpliesNext     );
            operators.insert("||" , Token::Type::Or                    );
            operators.insert("&&" , Token::Type::And                   );
            operators.insert("&&&", Token::Type::Intersect             );

        spaces   .balance();
        keywords .balance();
        operators.balance();

        for(int n = 0; characterNames[n]; n += 2){
            characters[characterNames[n]] = (const byte*)characterNames[n+1];
        }

        initialised = true;
    }
}
//------------------------------------------------------------------------------

Scanner::~Scanner()
{
    if(buffer) delete[] buffer;
}
//------------------------------------------------------------------------------

void Scanner::printError(const char* message)
{
    error = true;
    ::printError(line, filename, message);
}
//------------------------------------------------------------------------------

bool Scanner::lineComment()
{
    int count;

    if(buffer[index] != '/' || buffer[index+1] != '/') return false;
    index += 2;

    while(buffer[index]){
        if(spaces.match(buffer+index, &count) == Token::Type::Newline){
            line  ++;
            index += count;
            return true;
        }

        if(count) index += count;
        else      index ++;
    }
    printError("Incomplete line comment");
    return false;
}
//------------------------------------------------------------------------------

bool Scanner::todoComment()
{
    int    count;
    string comment;

    if(buffer[index] != '!' || buffer[index+1] != '!') return false;
    index += 2;

    while(buffer[index]){
        if(spaces.match(buffer+index, &count) == Token::Type::Newline){
            if(!comment.empty()){
                printf(
                    ANSI_FG_BRIGHT_BLACK   "line "
                    ANSI_FG_CYAN           "%05d "
                    ANSI_FG_BRIGHT_BLACK   "of "
                    ANSI_FG_YELLOW         "%s\n"
                    ANSI_FG_BRIGHT_MAGENTA "  Todo:"
                    ANSI_RESET             " %s\n",
                    line,
                    filename.c_str(),
                    comment .c_str()
                );
            }
            line  ++;
            index += count;
            return true;
        }
        if(count){
            for(int n = 0; n < count; n++) comment += buffer[index++];
        }else{
            comment += buffer[index++];
        }
    }
    printError("Incomplete todo comment");
    return false;
}
//------------------------------------------------------------------------------

bool Scanner::blockComment()
{
    int count;

    if(buffer[index] != '/' || buffer[index+1] != '*') return false;
    index += 2;

    while(buffer[index]){
        if(buffer[index] == '*' && buffer[index+1] == '/'){
            index += 2;
            return true;
        }
        if(spaces.match(buffer+index, &count) == Token::Type::Newline){
            line  ++;
            index += count;
        }else{
            index++;
        }
    }
    printError("Incomplete block comment");
    return false;
}
//------------------------------------------------------------------------------

bool Scanner::nestedComment()
{
    int count;

    if(buffer[index] != '/' || buffer[index+1] != '+') return false;
    index += 2;

    while(buffer[index]){
        if(buffer[index] == '+' && buffer[index+1] == '/'){
            index += 2;
            return true;
        }

        if(nestedComment()) continue;
        if(error) return false;

        if(spaces.match(buffer+index, &count) == Token::Type::Newline){
            line  ++;
            index += count;
        }else{
            index++;
        }
    }
    printError("Incomplete nested comment");
    return false;
}
//------------------------------------------------------------------------------

void Scanner::whiteSpace()
{
    Token::Type type;
    int         count;

    while(buffer[index]){
        while(lineComment() || todoComment() || blockComment() | nestedComment());

        type = spaces.match(buffer+index, &count);
        switch(type){
            case Token::Type::Space:
                index += count;
                break;

            case Token::Type::Newline:
                line++;
                index += count;
                break;

            default: return;
        }
    }
}
//------------------------------------------------------------------------------

bool Scanner::digit()
{
    return (buffer[index] >= '0' && buffer[index] <= '9');
}
//------------------------------------------------------------------------------

bool Scanner::nonDigit()
{
    int count;
    if(buffer[index] >= 0x80){
        if(spaces.match(buffer+index, &count) != Token::Type::Unknown) return false;
    }

    return (buffer[index] >= 'a' && buffer[index] <= 'z' ) ||
           (buffer[index] >= 'A' && buffer[index] <= 'Z' ) ||
           (buffer[index] == '_' || buffer[index] >= 0x80);
}
//------------------------------------------------------------------------------

bool Scanner::getIdentifier(Token* token)
{
    time_t _timer;
    tm*    _time;

    if(!nonDigit()) return false;

    token->data += buffer[index++];
    while(buffer[index]){
        if(!digit() && !nonDigit()) break;
        token->data += buffer[index++];
    }
    token->type = keywords.find((const byte*)token->data.c_str());

    switch(token->type){
        case Token::Type::Unknown:
            token->type = Token::Type::Identifier;
            break;

        case Token::Type::File:
            token->type = Token::Type::String;
            token->data = filename;
            break;

        case Token::Type::Line:
            token->type  = Token::Type::Literal;
            token->value = token->line;
            break;

        case Token::Type::Date:{
            token->type = Token::Type::String;
            token->data.clear();
            time(&_timer);
            _time = localtime(&_timer);
            char s[0x10];
            snprintf(s, 0x10, "%04d-%02d-%02d",
                     _time->tm_year+1900, _time->tm_mon+1, _time->tm_mday);
            token->data += s;
            break;
        }

        case Token::Type::Time:{
            token->type = Token::Type::String;
            token->data.clear();
            time(&_timer);
            _time = localtime(&_timer);
            char s[0x10];
            snprintf(s, 0x10, "%02d:%02d:%02d",
                     _time->tm_hour, _time->tm_min, _time->tm_sec);
            token->data += s;
            break;
        }

        default:
            break;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Scanner::getOperator(Token* token)
{
    int count;

    token->type = operators.match(buffer+index, &count);
    if(count){
        while(count){
            token->data += buffer[index++];
            count--;
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------

bool Scanner::getDigit(unsigned* digit, unsigned base)
{
    if(buffer[index] >= '0' && buffer[index] <= '9'){
        *digit = buffer[index] - '0';
        return *digit < base;
    }
    if(buffer[index] >= 'A' && buffer[index] <= 'Z'){
        *digit = buffer[index] - 'A' + 0xA;
        return *digit < base;
    }
    if(buffer[index] >= 'a' && buffer[index] <= 'z'){
        *digit = buffer[index] - 'a' + 0xA;
        return *digit < base;
    }
    return false;
}
//------------------------------------------------------------------------------

unsigned Scanner::getExponent(bool* sign, Token* token)
{
    unsigned exponent = 0;

    token->data += buffer[index++];
    while(buffer[index] == '_') token->data += buffer[index++];

    *sign = false;
    if(buffer[index] == '-'){
        *sign = true;
        token->data += buffer[index++];
    }else if(buffer[index] == '+'){
        token->data += buffer[index++];
    }

    while(buffer[index] == '_') token->data += buffer[index++];
    if(buffer[index] < '0' || buffer[index] > '9'){
        printError("Exponent digit expected");
        return 0;
    }

    while(buffer[index]){
        while(buffer[index] == '_') token->data += buffer[index++];

        if(buffer[index] < '0' || buffer[index] > '9') break;

        exponent = 10*exponent + buffer[index] - '0';
        token->data += buffer[index++];
    }
    return exponent;
}
//------------------------------------------------------------------------------

bool Scanner::getNumber(Token* token, unsigned base)
{
    unsigned digit;

    while(buffer[index] == '_') token->data += buffer[index++];
    if(!getDigit(&digit, base) && buffer[index] != '.'){
        printError("Illegal literal format");
        return false;
    }

    mpz_t num, den, exp;
    mpz_init_set_ui(num, 0);
    mpz_init_set_ui(den, 1);
    mpz_init_set_ui(exp, 1);

    while(buffer[index]){
        while(buffer[index] == '_') token->data += buffer[index++];

        if(!getDigit(&digit, base)) break;

        mpz_mul_ui(num, num, base);
        mpz_add_ui(num, num, digit);
        token->data += buffer[index++];
    }

    if(buffer[index] == '.' && buffer[index+1] != '.'){
        token->data += buffer[index++];
        while(buffer[index]){
            while(buffer[index] == '_') token->data += buffer[index++];

            if(!getDigit(&digit, base)) break;

            mpz_mul_ui(num, num, base);
            mpz_mul_ui(den, den, base);
            mpz_add_ui(num, num, digit);
            token->data += buffer[index++];
        }
    }

    bool     sign     = false;
    unsigned exponent = 0;

    if(base == 10 && (buffer[index] == 'e' || buffer[index] == 'E')){
        exponent = getExponent(&sign, token);
        mpz_ui_pow_ui(exp, 10, exponent);

    }else if(buffer[index] == 'p' || buffer[index] == 'P'){
        exponent = getExponent(&sign, token);
        mpz_ui_pow_ui(exp, 2, exponent);
    }

    if(sign) mpz_mul(den, den, exp);
    else     mpz_mul(num, num, exp);

    token->value.set(num, den);

    if(buffer[index] == 'i' || buffer[index] == 'j'){
        token->data += buffer[index++];
        while(buffer[index] == '_') token->data += buffer[index++];
        token->value.mul(0, 1);
    }

    mpz_clear(num);
    mpz_clear(den);
    mpz_clear(exp);

    return true;
}
//------------------------------------------------------------------------------

bool Scanner::getLiteral(Token* token)
{
    token->type = Token::Type::Literal;

    if(
        (                        buffer[index  ] < '0' || buffer[index  ] > '9') &&
        (buffer[index] != '.' || buffer[index+1] < '0' || buffer[index+1] > '9')
    ) return false;

    if(buffer[index] == '0'){
        switch(buffer[index+1]){
            case 'b':
                token->data += buffer[index++];
                token->data += buffer[index++];
                return getNumber(token, 2);

            case 'o':
                token->data += buffer[index++];
                token->data += buffer[index++];
                return getNumber(token, 8);

            case 'x':
                token->data += buffer[index++];
                token->data += buffer[index++];
                return getNumber(token, 16);

            default : break;
        }
    }
    return getNumber(token, 10);
}
//------------------------------------------------------------------------------

bool Scanner::getString(Token* token)
{
    bool     interpolated = false;
    int      n;
    unsigned digit, utf32;

    if(inInterpolatedString){
        switch(buffer[index]){
            case '$':
                if(buffer[index+1] == '"')
                    printError("Cannot nest interpolated strings");
                return false;

            case '"':
                index++;
                token->type = Token::Type::String;
                break;

            case '}':
                index++;
                interpolated = true;
                inInterpolatedString = false;
                token->type = Token::Type::InterpolatedStringPart;
                break;

            default:
                return false;
        }
    }else{
        switch(buffer[index]){
            case '"':
                index++;
                token->type = Token::Type::String;
                break;

            case '$':
                if(buffer[index+1] != '"') return false;
                index += 2;
                interpolated = true;
                token->type = Token::Type::InterpolatedStringPart;
                break;

            default:
                return false;
        }
    }


    while(buffer[index]){
        if(buffer[index] == '"'){
            index++;
            whiteSpace();
            if(buffer[index] == '"'){ // Concatenate the next string
                index++;
                interpolated = false;
                continue;
            }else if(buffer[index] == '$' && buffer[index+1] == '"'){
                index += 2;
                interpolated = true;
                continue;
            }else{
                if(token->type == Token::Type::InterpolatedStringPart)
                    token->type = Token::Type::InterpolatedStringEnd;
                return true;
            }
        }
        if(interpolated && buffer[index] == '{'){
            index++;
            inInterpolatedString = true;
            return true;
        }
        if(buffer[index] == '\\'){
            index++;
            switch(buffer[index]){
                case 'n' : token->data += '\n'; index++; break;
                case 't' : token->data += '\t'; index++; break;
                case 'v' : token->data += '\v'; index++; break;
                case 'b' : token->data += '\b'; index++; break;
                case 'r' : token->data += '\r'; index++; break;
                case 'f' : token->data += '\f'; index++; break;
                case 'e' : token->data += '\e'; index++; break;
                case 'a' : token->data += '\a'; index++; break;
                case '\\': token->data += '\\'; index++; break;
                case '?' : token->data += '\?'; index++; break;
                case '\'': token->data += '\''; index++; break;
                case '"' : token->data += '\"'; index++; break;
                case '{' : token->data += '{';  index++; break;
                case '}' : token->data += '}';  index++; break;

                case '&':{ // HTML character name
                    index++;
                    for(n = index; buffer[n] && buffer[n] != ';'; n++);
                    if(!buffer[n]){
                        printError("Invalid \\& code");
                        return false;
                    }
                    buffer[n] = 0;
                    auto s = characters.find((const char*)(buffer+index));
                    if(s == characters.end()){
                        printError("Invalid \\& code");
                        return false;
                    }
                    index = n+1;
                    token->data += (const char*)s->second;
                    break;
                }

                case 'x' : // Hexadecimal number
                    index++;
                    utf32 = 0;
                    for(n = 0; n < 2; n++){
                        if(!getDigit(&digit, 16)){
                            printError("Invalid \\x code");
                            return false;
                        }
                        utf32 = utf32*0x10 + digit;
                        index++;
                    }
                    token->data.append(utfConverter.toUtf8((char32_t)utf32));
                    break;

                case 'u' : // 16-bit Unicode
                    index++;
                    utf32 = 0;
                    for(n = 0; n < 4; n++){
                        if(!getDigit(&digit, 16)){
                            printError("Invalid \\u code");
                            return false;
                        }
                        utf32 = utf32*0x10 + digit;
                        index++;
                    }
                    token->data.append(utfConverter.toUtf8((char32_t)utf32));
                    break;

                case 'U' : // 32-bit Unicode
                    index++;
                    utf32 = 0;
                    for(n = 0; n < 8; n++){
                        if(!getDigit(&digit, 16)){
                            printError("Invalid \\U code");
                            return false;
                        }
                        utf32 = utf32*0x10 + digit;
                        index++;
                    }
                    token->data.append(utfConverter.toUtf8((char32_t)utf32));
                    break;

                default: // Could be an octal number...
                    utf32 = 0;
                    for(n = 0; n < 11; n++){
                        if(buffer[index] < '0' || buffer[index] > '7'){
                            if(n) break;
                            printError("Invalid escape sequence");
                            return false;
                        }
                        utf32 = utf32*8 + buffer[index++] - '0';
                    }
                    token->data.append(utfConverter.toUtf8((char32_t)utf32));
                    break;
            }
        }else{
            if(spaces.match(buffer+index, &n) == Token::Type::Newline){
                line++;
                while(n){
                    token->data += buffer[index++];
                    n--;
                }
            }else{
                token->data += buffer[index++];
            }
        }
    }
    printError("Incomplete string");
    return false;
}
//------------------------------------------------------------------------------

bool Scanner::open(const char* filename)
{
    FileWrapper fs;

    line  = 1;
    index = 0;
    error = false;

    if(buffer) delete[] buffer;

    this->filename = filename;

    buffer = (byte*)fs.readAll(filename);
    if(!buffer){
        error("Cannot read file: %s\n", filename);
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------

const char* Scanner::getFilename()
{
    return filename.c_str();
}
//------------------------------------------------------------------------------

bool Scanner::getToken(Token* token)
{
    token->line  = line;
    token->type  = Token::Type::Unknown;
    token->value = 0;
    token->data.clear();

    whiteSpace();

    if(!buffer[index]){
        token->type = Token::Type::EndOfFile;
        return false;
    }
    if(error) return false;

    token->line = line;

    // In order of least to most expensive match
    if(getString    (token)) { return true; } if(error) { return false; }
    if(getLiteral   (token)) { return true; } if(error) { return false; }
    if(getIdentifier(token)) { return true; } if(error) { return false; }
    if(getOperator  (token)) { return true; } if(error) { return false; }

    if(buffer[index]){
        char s[0x100];
        sprintf(s, "Unknown token near \"%.10s\"", buffer+index);
        printError(s);
    }
    token->type = Token::Type::Unknown;
    return false;
}
//------------------------------------------------------------------------------

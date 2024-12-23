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
#include "characterNames.h"
//------------------------------------------------------------------------------

using std::string;
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
        initialised = true;

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

        spaces.insert("\n"          , Token::Type::Newline);
        spaces.insert("\r"          , Token::Type::Newline);
        spaces.insert("\r\n"        , Token::Type::Newline);
        spaces.insert("\n\r"        , Token::Type::Newline);
        spaces.insert("\x0B"        , Token::Type::Newline); // Vertical Tab
        spaces.insert("\x0C"        , Token::Type::Newline); // Form Feed
        spaces.insert("\xC2\x85"    , Token::Type::Newline); // U+0085: NEL
        spaces.insert("\xE2\x80\xA8", Token::Type::Newline); // U+2028: line Separator
        spaces.insert("\xE2\x80\xA9", Token::Type::Newline); // U+2029: Paragraph Separator

        keywords.insert("__FILE__"     , Token::Type::FILE);
        keywords.insert("__LINE__"     , Token::Type::LINE);
        keywords.insert("__DATE__"     , Token::Type::DATE);
        keywords.insert("__TIME__"     , Token::Type::TIME);
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
        keywords.insert("goto"         , Token::Type::Goto    );

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
        operators.insert("|"  , Token::Type::Bit_OR     );
        operators.insert("~|" , Token::Type::Bit_NOR    );
        operators.insert("^"  , Token::Type::Bit_XOR    );
        operators.insert("~^" , Token::Type::Bit_XNOR   );
        operators.insert("&"  , Token::Type::Bit_AND    );
        operators.insert("~&" , Token::Type::Bit_NAND   );

        // Relational
        operators.insert("==" , Token::Type::Equal        );
        operators.insert("!=" , Token::Type::Not_Equal    );
        operators.insert("<"  , Token::Type::Less         );
        operators.insert(">"  , Token::Type::Greater      );
        operators.insert("<=" , Token::Type::Less_Equal   );
        operators.insert(">=" , Token::Type::Greater_Equal);

        // Shift
        operators.insert("<<" , Token::Type::Shift_Left );
        operators.insert(">>" , Token::Type::Shift_Right);

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
        operators.insert("&"  , Token::Type::AND_Reduce );
        operators.insert("~&" , Token::Type::NAND_Reduce);
        operators.insert("|"  , Token::Type::OR_Reduce  );
        operators.insert("~|" , Token::Type::NOR_Reduce );
        operators.insert("^"  , Token::Type::XOR_Reduce );
        operators.insert("~^" , Token::Type::XNOR_Reduce);
        operators.insert("!"  , Token::Type::Logical_NOT);

        // Array
        operators.insert(".." , Token::Type::To  );
        operators.insert(":"  , Token::Type::Step);

        // Unary
        operators.insert("-"  , Token::Type::Negate   );
        operators.insert("~"  , Token::Type::Bit_NOT  );
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
        operators.insert("="  , Token::Type::Assign            );
        operators.insert(":=" , Token::Type::Raw_Assign        );
        operators.insert("~=" , Token::Type::Append_Assign     );
        operators.insert("+=" , Token::Type::Add_Assign        );
        operators.insert("-=" , Token::Type::Subtract_Assign   );
        operators.insert("*=" , Token::Type::Multiply_Assign   );
        operators.insert("/=" , Token::Type::Divide_Assign     );
        operators.insert("%=" , Token::Type::Modulus_Assign    );
        operators.insert("^=" , Token::Type::XOR_Assign        );
        operators.insert("&=" , Token::Type::AND_Assign        );
        operators.insert("|=" , Token::Type::OR_Assign         );
        operators.insert("**=", Token::Type::Exponential_Assign);
        operators.insert("<<=", Token::Type::Shift_Left_Assign );
        operators.insert(">>=", Token::Type::Shift_Right_Assign);

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
        operators.insert("@"  , Token::Type::WaitUntil             );
        operators.insert("##" , Token::Type::WaitCycles            );
        operators.insert("[*" , Token::Type::SequenceConsecutive   );
        operators.insert("[->", Token::Type::SequenceGoto          );
        operators.insert("[=" , Token::Type::SequenceNonConsecutive);
        operators.insert("|->", Token::Type::AssertImplies         );
        operators.insert("|=>", Token::Type::AssertImpliesNext     );
        operators.insert("||" , Token::Type::Or                    );
        operators.insert("&&" , Token::Type::And                   );
        operators.insert("&&&", Token::Type::Intersect             );

        spaces   .balance();
        keywords .balance();
        operators.balance();

        for(int j = 0; characterNames[j]; j += 2){
            characters[characterNames[j]] = (const byte*)characterNames[j+1];
        }
    }

    line   = 1;
    index  = 0;
    error  = false;
    buffer = 0;
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

        case Token::Type::FILE:
            token->type = Token::Type::String;
            token->data = filename.c_str();
            break;

        case Token::Type::LINE:
            token->type  = Token::Type::Literal;
            token->value = token->line;
            break;

        case Token::Type::DATE:
            token->type = Token::Type::String;
            token->data.clear();
            time(&_timer);
            _time = localtime(&_timer);
            token->data += _time->tm_year + 1900;
            token->data += "-";
            if(_time->tm_mon < 9) token->data += "0";
            token->data += _time->tm_mon + 1;
            token->data += "-";
            if(_time->tm_mday < 10) token->data += "0";
            token->data += _time->tm_mday;
            break;

        case Token::Type::TIME:
            token->type = Token::Type::String;
            token->data.clear();
            time(&_timer);
            _time = localtime(&_timer);
            if(_time->tm_hour < 10) token->data += "0";
            token->data += _time->tm_hour;
            token->data += ":";
            if(_time->tm_min < 10) token->data += "0";
            token->data += _time->tm_min;
            token->data += ":";
            if(_time->tm_sec < 10) token->data += "0";
            token->data += _time->tm_sec;
            break;

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
        printError("exponent digit expected");
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
    // mpz_init_set_ui(num, 0);
    // mpz_init_set_ui(den, 1);
    // mpz_init_set_ui(exp, 1);
    num = 0;
    den = 1;
    exp = 1;

    while(buffer[index]){
        while(buffer[index] == '_') token->data += buffer[index++];

        if(!getDigit(&digit, base)) break;

        num *= base;  // mpz_mul_ui(num, num, base);
        num += digit; // mpz_add_ui(num, num, digit);
        token->data += buffer[index++];
    }

    if(buffer[index] == '.' && buffer[index+1] != '.'){
        token->data += buffer[index++];
        while(buffer[index]){
            while(buffer[index] == '_') token->data += buffer[index++];

            if(!getDigit(&digit, base)) break;

            num *= base;  // mpz_mul_ui(num, num, base);
            den *= base;  // mpz_mul_ui(den, den, base);
            num += digit; // mpz_add_ui(num, num, digit);
            token->data += buffer[index++];
        }
    }

    bool     sign     = false;
    unsigned exponent = 0;

    if(base == 10 && (buffer[index] == 'e' || buffer[index] == 'E')){
        exponent = getExponent(&sign, token);
        exp = pow(10, exponent); // mpz_ui_pow_ui(exp, 10, exponent);

    }else if(buffer[index] == 'p' || buffer[index] == 'P'){
        exponent = getExponent(&sign, token);
        exp = pow(2, exponent); // mpz_ui_pow_ui(exp, 2, exponent);
    }

    if(sign) den *= exp; // mpz_mul(den, den, exp);
    else     num *= exp; // mpz_mul(num, num, exp);

    token->value.set(num, den);

    if(buffer[index] == 'i' || buffer[index] == 'j'){
        token->data += buffer[index++];
        while(buffer[index] == '_') token->data += buffer[index++];
        token->value.mul(0, 1);
    }

    // mpz_clear(num);
    // mpz_clear(den);
    // mpz_clear(exp);

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
    int      j;
    unsigned digit, utf32;
    Characters::iterator s;

    if(buffer[index] != '"') return false;

    token->type = Token::Type::String;

    index++;

    while(buffer[index]){
        if(buffer[index] == '"'){
            index++;
            whiteSpace();
            if(buffer[index] == '"'){ // Concatenate the next string
                index++;
                continue;
            }else{
                return true;
            }
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
                case 'a' : token->data += '\a'; index++; break;
                case '\\': token->data += '\\'; index++; break;
                case '?' : token->data += '\?'; index++; break;
                case '\'': token->data += '\''; index++; break;
                case '"' : token->data += '\"'; index++; break;

                case '&': // HTML character name
                    index++;
                    for(j = index; buffer[j] && buffer[j] != ';'; j++);
                    if(!buffer[j]){
                        printError("Invalid \\& code");
                        return false;
                    }
                    buffer[j] = 0;
                    s = characters.find((const char*)(buffer+index));
                    if(s == characters.end()){
                        printError("Invalid \\& code");
                        return false;
                    }
                    index = j+1;
                    token->data += (const char*)s->second;
                    break;

                case 'x' : // Hexadecimal number
                    index++;
                    utf32 = 0;
                    for(j = 0; j < 2; j++){
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
                    for(j = 0; j < 4; j++){
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
                    for(j = 0; j < 8; j++){
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
                    for(j = 0; j < 11; j++){
                        if(buffer[index] < '0' || buffer[index] > '7'){
                            if(j) break;
                            printError("Invalid escape sequence");
                            return false;
                        }
                        utf32 = utf32*8 + buffer[index++] - '0';
                    }
                    token->data.append(utfConverter.toUtf8((char32_t)utf32));
                    break;
            }
        }else{
            if(spaces.match(buffer+index, &j) == Token::Type::Newline){
                line++;
                while(j){
                    token->data += buffer[index++];
                    j--;
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

bool Scanner::getToken(Token* token)
{
    token->line = line;
    token->type = Token::Type::Unknown;
    token->data.clear();

    if(!buffer[index]) return false;
    if( error        ) return false;

    whiteSpace();

    token->line = line;

    if(getString    (token)) return true; // This is the least expensive match
    if(getLiteral   (token)) return true;
    if(getIdentifier(token)) return true;
    if(getOperator  (token)) return true; // This is the most expensive match

    if(buffer[index]){
        char s[0x100];
        sprintf(s, "Unknown token near \"%.5s\"", buffer+index);
        printError(s);
    }
    token->type = Token::Type::Unknown;
    return false;
}
//------------------------------------------------------------------------------

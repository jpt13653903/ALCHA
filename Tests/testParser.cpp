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

#include "Parser.h"
//------------------------------------------------------------------------------

#include <string>
using std::string;

AST::AST* ast = 0;
Parser*   parser;
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s...\n", name);

    string filename;
    filename  = "testParser/";
    filename += name;
    filename += ".alc";

    if(ast) delete ast;
    ast = parser->parse(filename.c_str());

    if(!ast){
        error("Cannot parse file %s", filename.c_str());
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
        if(c >= 0x20){
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

bool test(int testIndex, AST::AST* node, const char* expected)
{
    assert(node, return false);

    string got = node->print();

    if(got != expected){
        printf(ANSI_FG_BRIGHT_RED "FAILED: "
               ANSI_RESET         "Test %d\n"
               ANSI_FG_GREEN      "    Expected: %s\n"
               ANSI_FG_BRIGHT_RED "    Got:      %s\n"
               ANSI_RESET,
               testIndex+1,
               expand(expected).c_str(),
               expand(got).c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool runTest(const char** expected)
{
    int n = 0;
    auto node = ast;
    while(node){
        if(!expected[n]){
            printf(ANSI_FG_BRIGHT_RED "FAILED:\n"
                   ANSI_RESET         "    More AST nodes than expected\n"
                   ANSI_FG_BRIGHT_RED "    Got: %s\n"
                   ANSI_RESET,
                   node->print().c_str());
            // return false;
        }else{
            if(!test(n, node, expected[n])); // return false;
            n++;
        }
        node = node->next;
    }
    if(expected[n]){
        printf(ANSI_FG_BRIGHT_RED "FAILED:\n"
               ANSI_RESET         "    Fewer AST nodes than expected\n");
        return false;
    }

    assert(!node, return false);
    return true;
}
//------------------------------------------------------------------------------

void endTest(const char* message = "PASS")
{
    printf(ANSI_FG_GREEN "%s\n" ANSI_FG_BRIGHT_BLACK
           "----------------------------------------"
           "----------------------------------------\n\n"
           ANSI_RESET, message);
}
//------------------------------------------------------------------------------

bool testLiterals()
{
    if(!startTest("Literals")) return false;

    const char* expected[] = {
        "X = 15432/125 (~123.456)",
        "X = 85/16 (~5.3125)",
        "X = 21399/256 (~83.5898)",
        "X = 11259375/4096 (~2748.87)",
        "X = 85/16 (~5.3125)",
        "X = 21399/256 (~83.5898)",
        "X = 11259375/4096 (~2748.87)",
        "X = 85/16 (~5.3125)",
        "X = 21399/256 (~83.5898)",
        "X = 11259375/4096 (~2748.87)",
        "X = 1234560000000 (~1.23456e+12)",
        "X = 15802368 (~1.58024e+07)",
        "X = 152919552 (~1.5292e+08)",
        "X = 89/10j (~8.9j)",
        "X = \"A\"",
        "X = \"ABC\"",
        "X = \"ABC\nlkjh\rklkjh\123hi"
            "\xE2\x87\x9B"
            "\xc2\xab"
            "\xEA\xAF\x8D"
            "efg"
            "\xFE\x82\xAB\xB3\x9E\xBC\x92"
            "34567\"",
        "X = $\"I have {x} sheep\"",
        "X = $\"I have {(x) + (y)} sheep\"",
        "X = $\"I have {(x) + (y), \"03x\"} sheep\"",
        "X = $\"I have {(x) + (y), format} sheep\"",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testIdentifiers()
{
    if(!startTest("Identifiers")) return false;

    const char* expected[] = {

        "X = variable",
        "X = anotherVariable",
        "X = autoVariable",
        "X = αΓρεεκΩαριαβλε",
        "X = πΓρεεκΩαριαβλε",
        "X = pi",
        "X = π",
        "X = e",
        "X = i",
        "X = j",
        "X = __YEAR__",
        "X = __MONTH__",
        "X = __DAY__",
        "X = __HOUR__",
        "X = __MINUTE__",
        "X = __SECOND__",
        "X = __WEEKDAY__",
        "X = __YEARDAY__",
        "X = \"testParser/Identifiers.alc\"",
        "X = 30 (~30)",
        "X = __CLASS__",
        "X = __FUNCTION__",
        "X = __NAMESPACE__",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testExpressions()
{
    if(!startTest("Expressions")) return false;

    const char* expected[] = {
        "X = (A) ? (B) : (C)",
        "X = (A) ?: (B)",
        "X = (A) | (B)",
        "X = (A) ~| (B)",
        "X = (A) & (B)",
        "X = (A) ~& (B)",
        "X = (A) ^ (B)",
        "X = (A) ~^ (B)",
        "X = (A) == (B)",
        "X = (A) != (B)",
        "X = (A) < (B)",
        "X = (A) > (B)",
        "X = (A) <= (B)",
        "X = (A) >= (B)",
        "X = (A) << (B)",
        "X = (A) >> (B)",
        "X = (A) + (B)",
        "X = (A) - (B)",
        "X = (A) * (B)",
        "X = (A) / (B)",
        "X = (A) % (B)",
        "X = (A) ** (B)",

        "X = (A) ` (B)",
        "X = $(A)",
        "X = $(A)",
        "X = $(A, B)",
        "X =  & (A)",
        "X =  ~& (A)",
        "X =  | (A)",
        "X =  ~| (A)",
        "X =  ^ (A)",
        "X =  ~^ (A)",
        "X =  ! (A)",
        "X = (1 (~1)) .. (6 (~6))",
        "X = (1 (~1)) .. (6 (~6)):(3 (~3))",
        "X = (A) .. (B)",
        "X = (A) .. (B):(C)",
        "X =  - (A)",
        "X =  ~ (A)",
        "X =  : (A)",
        "X =  ++ (A)",
        "X =  -- (A)",
        "X = (A)[ B, C, D ]",
        "X = A(A, B, C)",
        "X = (A) . (B)",
        "X = (A) ?. (B)",
        "X = (A) ' (B)",
        "X = (A) ++ ",
        "X = (A) -- ",
        "X = (A) ! ",
        "X = (A) @ (B)",
        "X = (A) @ (123 (~123))",
        "X = (A) @ (A)",
        "X = A",
        "X =  ' (A)",
        "X = 123 (~123)",
        "X = 1 (~1)",
        "X = 0 (~0)",
        "X = )",
        "X = ]",
        "X = [ A, B, C ]",
        "X = \"Hello There\"",
        "X = (A) + (B)",

        "X = ((A) + ((B) * (C))) - (D)",
        "X = ( & (A)) + ((D) @ (6 (~6)))",
        "X = (((A) . (B)) . (C)) ' (E)",
        "X = ((A) . (B)) . (C)(E, F, G)",
        "X = A(B = C, D := E)",
        "X = A = B",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testModules()
{
    if(!startTest("Modules")) return false;

    const char* expected[] = {
        "ABC:",
        "pin A",
        "net A",
        "void A",
        "auto A",
        "byte A",
        "char A",
        "num A",
        "func A",
        "ABC A",
        "(ABC) . (DEF) A, B, C",

        "pin (5 (~5)) A",
        "net (123 (~123), 456 (~456)) A",
        "void (A, B) A",
        "auto (C) A",
        "byte (((A) . (B)) . (C)) A",
        "char (((A) . (B)) ' (C)) A",
        "num ((A)[ B ]) A",
        "func (A(B)) A",
        "ABC (7 (~7), 9 (~9), X = 10 (~10)) A",
        "(ABC) . (DEF) (7 (~7), Y := 9 (~9), X = 10 (~10)) A, B, C",

        "pin (5 (~5)) <A = a, B = b> A",
        "net (123 (~123), 456 (~456)) <A = a, B = b> A",
        "void (A, B) <A = a, B = b> A",
        "auto (C) <A = a, B = b> A",
        "byte (((A) . (B)) . (C)) <A = a, B = b> A",
        "char (((A) . (B)) ' (C)) <A = a, B = b> A",
        "num ((A)[ B ]) <A = a, B = b> A",
        "func (A(B)) <A = a, B = b> A",
        "ABC (7 (~7), 9 (~9), X = 10 (~10)) <A = a, B = b> A",
        "(ABC) . (DEF) (7 (~7), Y := 9 (~9), X = 10 (~10)) <A = a, B = b> A, B, C",

        "class MyClass{\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class MyClass(num D, pin E, byte F){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class <A = 3 (~3), B = 1 (~1), C = 7 (~7)> MyClass(num D, pin E, byte F){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class MyClass(num D, pin E, byte F): ParentClass1(A, B, C), ParentClass2(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class <A = 3 (~3), B = 1 (~1), C = 7 (~7)> MyClass(num D, pin E, byte F): ParentClass1(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class <A = 3 (~3), B = 1 (~1), C = 7 (~7)> MyClass(num D, pin E, byte F): ParentClass1(A, B, C), ParentClass2(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",

        "auto ABC[5 (~5)](num A, pin B[], net C){\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "auto ABC(num A, pin (8 (~8)) B, net (3 (~3), 15 (~15)) C = 5 (~5)){\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "void ABC(A, B, C){\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "inline void ABC(A, B, C){\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",

        "enum ABC { A }",
        "enum ABC { A, B, C }",

        "alias A = ((B) . (C)) * (D)",
        "import \"../whatnot/thingy\"",
        "import \"../whatnot/thingy\" as whatnot",
        "struct <A = 3 (~3), B = 7 (~7)> ABC {\n"
        "    pin (15 (~15), 8 (~8)) A\n"
        "    net (17 (~17)) B, C\n"
        "    struct BCD {\n"
        "        pin (15 (~15), 8 (~8)) A\n"
        "        net (17 (~17)) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "struct <A = 3 (~3), B = 7 (~7)> {\n"
        "    pin (15 (~15), 8 (~8)) A\n"
        "    net (17 (~17)) B, C\n"
        "    struct {\n"
        "        pin (15 (~15), 8 (~8)) A\n"
        "        net (17 (~17)) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "group <A = 3 (~3), B = 7 (~7)> ABC {\n"
        "    pin (15 (~15), 8 (~8)) A\n"
        "    net (17 (~17)) B, C\n"
        "    group <C = 8 (~8), E = 5 (~5)> BCD {\n"
        "        pin (15 (~15), 8 (~8)) A\n"
        "        net (17 (~17)) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "group <voltage = 33/10 (~3.3), capacitance = 1/100000000000 (~1e-11), external_min_delay = 1/2000000000 (~5e-10), external_max_delay = 1/1000000000 (~1e-09)> SD {\n"
        "    pin <location = \"AB6\"> CLK\n"
        "    pin <location = \"W8\"> CMD\n"
        "    pin (4 (~4)) <location = [ \"U7\", \"T7\", \"V8\", \"T8\" ]> DAT\n"
        "}",

        "pin A[7 (~7)][8 (~8)] := [ [ 1 (~1), 2 (~2), 3 (~3) ], [ 4 (~4), 5 (~5), 6 (~6) ], [ 7 (~7), 8 (~8), 9 (~9) ] ]",
        "num A = 4 (~4), B = 7 (~7), C = 1 (~1)",

        "if ((A) == (B)) {\n"
        "    C = D\n"
        "} else {\n"
        "    E = F\n"
        "}",
        "if ((A) == (B)) {\n"
        "    C = D\n"
        "} else {\n"
        "    if ((A) == (B)) {\n"
        "        E = F\n"
        "    } else {\n"
        "        if ((A) == (B)) {\n"
        "            G = H\n"
        "        } else {\n"
        "            if ((A) == (B)) {\n"
        "                I = J\n"
        "            } else {\n"
        "                if ((A) == (B)) {\n"
        "                    K = L\n"
        "                } else {\n"
        "                    if ((A) == (B)) {\n"
        "                        M = N\n"
        "                    } else {\n"
        "                        O = P\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",
        "if ((A) == (B)) {\n"
        "    C = D\n"
        "} else {\n"
        "    E = F\n"
        "}",
        "if ((A) == (B)) {\n"
        "    C = D\n"
        "}",
        "if ((A) == (B)) {\n"
        "    C = D\n"
        "} else {\n"
        "    if ((A) == (B)) {\n"
        "        E = F\n"
        "    } else {\n"
        "        if ((A) == (B)) {\n"
        "            G = H\n"
        "        } else {\n"
        "            if ((A) == (B)) {\n"
        "                I = J\n"
        "            } else {\n"
        "                if ((A) == (B)) {\n"
        "                    K = L\n"
        "                } else {\n"
        "                    if ((A) == (B)) {\n"
        "                        M = N\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",
        "if ((A) == (B)) {\n"
        "    C = D\n"
        "}",
        "for ((A) in ((1 (~1)) .. (6 (~6)))) {\n"
        "    X++\n"
        "}",
        "for ((A) in (G)) {\n"
        "    X++\n"
        "}",
        "while ((A) < (7 (~7))) {\n"
        "    X++\n"
        "}",
        "loop (7 (~7)) {\n"
        "    X++\n"
        "}",
        "loop {\n"
        "    X++\n"
        "}",
        "enum STATE { Idle, Writing, Done, Others }",
        "STATE State",
        "switch (State) {\n"
        "    case (Idle) {\n"
        "        State = Writing\n"
        "    }\n"
        "    case (Writing) {\n"
        "        State = Done\n"
        "    }\n"
        "    case (Done) {\n"
        "        State = Idle\n"
        "    }\n"
        "    default {\n"
        "        print(\"The Default State\")\n"
        "    }\n"
        "}",
        "return",
        "break",
        "continue",
        "return 5 (~5)",
        "break 6 (~6)",
        "continue 7 (~7)",
        "goto Label",

        "rtl <A = 3 (~3), B = \"Hello\"> (Clk, Reset){\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "rtl <A = 3 (~3), B = \"Hello\"> {\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "rtl (Clk, Reset){\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "rtl (Clk){\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "rtl {\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "fsm <A = 3 (~3), B = \"Hello\"> (Clk, Reset){\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "fsm <A = 3 (~3), B = \"Hello\"> {\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "fsm (Clk, Reset){\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "fsm {\n"
        "    A = 5 (~5)\n"
        "    B = (C) + (3 (~3))\n"
        "}",
        "hdl <A = 3 (~3), B = \"Hello\"> ABC(\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "){\n"
        "    input {\n"
        "        pin Clk\n"
        "    }\n"
        "    input {\n"
        "        pin Reset\n"
        "    }\n"
        "    output {\n"
        "        pin (16 (~16)) Data\n"
        "    }\n"
        "    pin (16 (~16), 5 (~5)) AnotherPin\n"
        "}",

        "stimulus  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "stimulus  <A = 3 (~3), B = 5 (~5)> ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "stimulus  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "stimulus  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "stimulus  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "emulate  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "emulate  <A = 3 (~3), B = 5 (~5)> ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "emulate  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "emulate  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "emulate  <A = 3 (~3), B = 5 (~5)>  (1 (~1), 2 (~2), 3 (~3)) ABC {\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "}",
        "{\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "} || {\n"
        "    X++\n"
        "    Y--\n"
        "}",
        "{\n"
        "    A = 3 (~3)\n"
        "    B = 5 (~5)\n"
        "} && {\n"
        "    X++\n"
        "    Y--\n"
        "}",
        "stimulus  (1/1000000000 (~1e-09)) {\n"
        "    A = 5 (~5)\n"
        "    #5 (~5)\n"
        "    B = 7 (~7)\n"
        "    @(posedge Clk)\n"
        "    fence\n"
        "    @(Clk, Reset)\n"
        "    fence\n"
        "    @(posedge Clk, negedge Reset)\n"
        "    fence\n"
        "    wait ((A) == (7 (~7)))\n"
        "    fence\n"
        "    loop {\n"
        "        #7 (~7)\n"
        "        C++\n"
        "    }\n"
        "}",
        "assert (A) == (B)",

        "num operator?: (A, B){\n"
        "    return A\n"
        "}",
        "num operator| (A, B){\n"
        "    return A\n"
        "}",
        "num operator~| (A, B){\n"
        "    return A\n"
        "}",
        "num operator^ (A, B){\n"
        "    return A\n"
        "}",
        "num operator~^ (A, B){\n"
        "    return A\n"
        "}",
        "num operator& (A, B){\n"
        "    return A\n"
        "}",
        "num operator~& (A, B){\n"
        "    return A\n"
        "}",
        "num operator== (A, B){\n"
        "    return A\n"
        "}",
        "num operator!= (A, B){\n"
        "    return A\n"
        "}",
        "num operator< (A, B){\n"
        "    return A\n"
        "}",
        "num operator> (A, B){\n"
        "    return A\n"
        "}",
        "num operator<= (A, B){\n"
        "    return A\n"
        "}",
        "num operator>= (A, B){\n"
        "    return A\n"
        "}",
        "num operator<< (A, B){\n"
        "    return A\n"
        "}",
        "num operator>> (A, B){\n"
        "    return A\n"
        "}",
        "num operator+ (A, B){\n"
        "    return A\n"
        "}",
        "num operator- (A, B){\n"
        "    return A\n"
        "}",
        "num operator* (A, B){\n"
        "    return A\n"
        "}",
        "num operator/ (A, B){\n"
        "    return A\n"
        "}",
        "num operator% (A, B){\n"
        "    return A\n"
        "}",
        "num operator** (A, B){\n"
        "    return A\n"
        "}",
        "num operator! (A, B){\n"
        "    return A\n"
        "}",
        "num operator` (A, B){\n"
        "    return A\n"
        "}",
        "num operator$ (A, B){\n"
        "    return A\n"
        "}",
        "num operator.. (A, B){\n"
        "    return A\n"
        "}",
        "num operator~ (A, B){\n"
        "    return A\n"
        "}",
        "num operator: (A, B){\n"
        "    return A\n"
        "}",
        "num operator++ (A, B){\n"
        "    return A\n"
        "}",
        "num operator-- (A, B){\n"
        "    return A\n"
        "}",
        "num operator@ (A, B){\n"
        "    return A\n"
        "}",
        "num operator= (A, B){\n"
        "    return A\n"
        "}",
        "num operator:= (A, B){\n"
        "    return A\n"
        "}",
        "num operator~= (A, B){\n"
        "    return A\n"
        "}",
        "num operator+= (A, B){\n"
        "    return A\n"
        "}",
        "num operator-= (A, B){\n"
        "    return A\n"
        "}",
        "num operator*= (A, B){\n"
        "    return A\n"
        "}",
        "num operator/= (A, B){\n"
        "    return A\n"
        "}",
        "num operator**= (A, B){\n"
        "    return A\n"
        "}",
        "num operator%= (A, B){\n"
        "    return A\n"
        "}",
        "num operator&= (A, B){\n"
        "    return A\n"
        "}",
        "num operator|= (A, B){\n"
        "    return A\n"
        "}",
        "num operator^= (A, B){\n"
        "    return A\n"
        "}",
        "num operator<<= (A, B){\n"
        "    return A\n"
        "}",
        "num operator>>= (A, B){\n"
        "    return A\n"
        "}",
        "num operator[* (A, B){\n"
        "    return A\n"
        "}",
        "num operator[-> (A, B){\n"
        "    return A\n"
        "}",
        "num operator[= (A, B){\n"
        "    return A\n"
        "}",
        "num operator|-> (A, B){\n"
        "    return A\n"
        "}",
        "num operator|=> (A, B){\n"
        "    return A\n"
        "}",
        "num operator|| (A, B){\n"
        "    return A\n"
        "}",
        "num operator&& (A, B){\n"
        "    return A\n"
        "}",
        "num operator&&& (A, B){\n"
        "    return A\n"
        "}",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testAutogen()
{
    if(!startTest("Autogen")) return false;

    const char* expected[] = {
        "class RegistersDecoder(Bus): AvalonInterface(32 (~32), 4096 (~4096)){\n"
        "    (Bus) . (Attach)(this)\n"

        "    private {\n"
        "         ' (RdRegisters) = [  ]\n"
        "         ' (WrRegisters) = [  ]\n"
        "         ' (LiveRegisters) = [  ]\n"
        "        num Count = 0 (~0)\n"
        "    }\n"

        "    public {\n"
        "        void ReadOnly(Register){\n"
        "            (Register) ' (Address) = (Count) ++ \n"
        "            ( ' (RdRegisters)) . (append)(Register)\n"
        "        }\n"
        "        void Writeable(Register){\n"
        "            (Register) ' (Address) = (Count) ++ \n"
        "            ( ' (WrRegisters)) . (append)(Register)\n"
        "        }\n"
        "        void Live(RdRegister, WrRegister, WrStrobe){\n"
        "            (RdRegister) ' (Address) = Count\n"
        "            (WrRegister) ' (Address) = Count\n"
        "            (WrStrobe) ' (Address) = Count\n"
        "            ( ' (LiveRegisters)) . (append)(Read = RdRegister, Write = WrRegister, Strobe = WrStrobe)\n"
        "            Count++\n"
        "        }\n"

        "        net (32 (~32)) Resize(x){\n"
        "            result := x\n"
        "            num N = (x) ' (width)\n"
        "            if ((((x) ' (fullscale)) < (0 (~0))) & ((N) < (31 (~31)))) {\n"
        "                result((31 (~31)) .. ((N) + (1 (~1)))) := (x(N)) ` ((31 (~31)) - (N))\n"
        "            }\n"
        "        }\n"

        "        void GenerateRegs(){\n"
        "            rtl ((Bus) . (Clock), (Bus) . (Reset)){\n"
        "                WaitRequest = 0 (~0)\n"
        "                switch (Address) {\n"
        "                    for ((Register) in ( ' (RdRegisters))) {\n"
        "                        case ((Register) ' (Address)) {\n"
        "                            ReadData = Resize(Register)\n"
        "                        }\n"
        "                    }\n"
        "                    for ((Register) in ( ' (WrRegisters))) {\n"
        "                        case ((Register) ' (Address)) {\n"
        "                            ReadData = Resize(Register)\n"
        "                        }\n"
        "                    }\n"
        "                    for ((Register) in ( ' (LiveRegisters))) {\n"
        "                        case (((Register) ' (Read)) ' (Address)) {\n"
        "                            ReadData = Resize((Register) ' (Read))\n"
        "                        }\n"
        "                    }\n"
        "                }\n"
        "                ReadValid = Read\n"

        "                if (Write) {\n"
        "                    switch (Address) {\n"
        "                        for ((Register) in ( ' (WrRegisters))) {\n"
        "                            case ((Register) ' (Address)) {\n"
        "                                Register := WriteData\n"
        "                            }\n"
        "                        }\n"
        "                        for ((Register) in ( ' (LiveRegisters))) {\n"
        "                            case (((Register) ' (Write)) ' (Address)) {\n"
        "                                (Register) ' (Write) := WriteData\n"
        "                            }\n"
        "                        }\n"
        "                    }\n"
        "                    for ((Register) in ( ' (LiveRegisters))) {\n"
        "                        (Register) ' (Strobe) = ((Address) == (((Register) ' (Strobe)) ' (Address))) & (Write)\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        }\n"

        "        void GenerateCpp(string Filename){\n"
        "        }\n"

        "        void GenerateLaTeX(string Filename){\n"
        "        }\n"

        "        string CppFilename = \"\"\n"
        "        string LaTeXFilename = \"\"\n"

        "        void finally(){\n"
        "            GenerateRegs()\n"
        "            if ((CppFilename) ' (length)) {\n"
        "                GenerateCpp(CppFilename)\n"
        "            }\n"
        "            if ((LaTeXFilename) ' (langth)) {\n"
        "                GenerateLaTeX(LaTeXFilename)\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

bool testParser()
{
    if(!startTest("Parser")) return false;

    const char* expected[] = {
        "num a",
        "num b",
        "num c, d",

        "a = 3 (~3)",
        "b = 5 (~5)",
        "print((a) + (b))",

        "a++",
        "a()",
        "a--",
        "a <<= ((a) * (b)) + ((c) * (d))",

        "a = ((a) ++ ) + ( ++ (b))",
        "a = (a) ? (b) : (c)",
        "a = ((x) | (x)) ~| (((x) ^ (x)) ~^ (((x) & (x)) ~& (((x) == (x)) != (((((x) < (x)) > (x)) <= (x)) >= (((x) << (x)) >> (((x) + (x)) - ((((x) * (x)) / (x)) % ((x) ** ((x) ` (x))))))))))",
        "a = (((((((((((((((((((((x) ` (x)) ** (x)) % (x)) / (x)) * (x)) - (x)) + (x)) >> (x)) << (x)) >= (x)) <= (x)) > (x)) < (x)) != (x)) == (x)) ~& (x)) & (x)) ~^ (x)) ^ (x)) ~| (x)) | (x)",
        "(((a)[ 5 (~5) ])[ 6 (~6) ])[ (7 (~7)) .. (9 (~9)), 8 (~8), (1 (~1)) .. (10 (~10)):(2 (~2)) ] = ((((b) . (c)) . (d)) . (e)) ' (attribute)",

        "MyLabel:",

        "((a) . (b)) . (c).{\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "}",

        "num myFunc(a, b, c = 5 (~5)){\n"
        "    print(((a) * (b)) + (c))\n"
        "}",
        "myFuct(1 (~1), 2 (~2), 3 (~3))",

        "inline num (15 (~15), 7 (~7)) <a = b, c = d> myFunc("
             "a, num b, c = 5 (~5), pin d = 8 (~8), pin (13 (~13), 9 (~9)) e, "
             "MyClass myClass, MyClass (123 (~123), 456 (~456)) myClass){\n"
        "    print(((a) * (b)) + (c))\n"
        "}",

        "MyClass myClass",
        "MyClass myClass",
        "MyClass (A) myClass",
        "MyClass (A, B) myClass",
        "MyClass (A, B, C) myClass",

        "MyClass <a = b> myClass",
        "MyClass <a = b> myClass",
        "MyClass (A) <a = b> myClass",
        "MyClass (A, B) <a = b> myClass",
        "MyClass (A, B, C) <a = b> myClass",

        "MyClass <a = b, c = d> myClass",
        "MyClass <a = b, c = d> myClass",
        "MyClass (A) <a = b, c = d> myClass",
        "MyClass (A, B) <a = b, c = d> myClass",
        "MyClass (A, B, C) <a = b, c = d> myClass",

        "MyClass ((a) + ((d) * (c)), (b) - (e)) <a = (b) ** (2 (~2)), c = (d) / (r), t = (g) > (5 (~5))> myClass",

        "class MyClass{\n"
        "    print(\"Hello\")\n"
        "}",
        "class <a = b, c = d> MyClass{\n"
        "    print(\"Hello\")\n"
        "}",
        "class <a = b, c = d> MyClass(a, b, c = 123 (~123)){\n"
        "    print(\"Hello\")\n"
        "}",
        "class MyClass: A{\n"
        "    num a\n"
        "}",
        "class MyClass: A, B{\n"
        "    num a\n"
        "}",
        "class MyClass: A(a, (b) + ((c) * (d))), B{\n"
        "    num a\n"
        "}",
        "class MyClass: A(a, (b) + ((c) * (d))), B{\n"
        "    num a\n"
        "}",

        "public {\n    num a\n}",
        "private {\n    num a\n}",
        "protected {\n    num a\n}",

        "public input {\n    num a\n}",
        "private input {\n    num a\n}",
        "protected input {\n    num a\n}",

        "public output {\n    num a\n}",
        "private output {\n    num a\n}",
        "protected output {\n    num a\n}",

        "input {\n    num a\n}",
        "output {\n    num a\n}",

        "public input {\n"
        "    num a\n"
        "    num b\n"
        "}",

        "num operator?: (A, B){\n    return A\n}",
        "num operator| (A, B){\n    return A\n}",
        "num operator~| (A, B){\n    return A\n}",
        "num operator^ (A, B){\n    return A\n}",
        "num operator~^ (A, B){\n    return A\n}",
        "num operator& (A, B){\n    return A\n}",
        "num operator~& (A, B){\n    return A\n}",
        "num operator== (A, B){\n    return A\n}",
        "num operator!= (A, B){\n    return A\n}",
        "num operator< (A, B){\n    return A\n}",
        "num operator> (A, B){\n    return A\n}",
        "num operator<= (A, B){\n    return A\n}",
        "num operator>= (A, B){\n    return A\n}",
        "num operator<< (A, B){\n    return A\n}",
        "num operator>> (A, B){\n    return A\n}",
        "num operator+ (A, B){\n    return A\n}",
        "num operator- (A, B){\n    return A\n}",
        "num operator* (A, B){\n    return A\n}",
        "num operator/ (A, B){\n    return A\n}",
        "num operator% (A, B){\n    return A\n}",
        "num operator** (A, B){\n    return A\n}",
        "num operator! (A, B){\n    return A\n}",
        "num operator` (A, B){\n    return A\n}",
        "num operator$ (A, B){\n    return A\n}",
        "num operator.. (A, B){\n    return A\n}",
        "num operator~ (A, B){\n    return A\n}",
        "num operator: (A, B){\n    return A\n}",
        "num operator++ (A, B){\n    return A\n}",
        "num operator-- (A, B){\n    return A\n}",
        "num operator@ (A, B){\n    return A\n}",
        "num operator= (A, B){\n    return A\n}",
        "num operator:= (A, B){\n    return A\n}",
        "num operator~= (A, B){\n    return A\n}",
        "num operator+= (A, B){\n    return A\n}",
        "num operator-= (A, B){\n    return A\n}",
        "num operator*= (A, B){\n    return A\n}",
        "num operator/= (A, B){\n    return A\n}",
        "num operator**= (A, B){\n    return A\n}",
        "num operator%= (A, B){\n    return A\n}",
        "num operator&= (A, B){\n    return A\n}",
        "num operator|= (A, B){\n    return A\n}",
        "num operator^= (A, B){\n    return A\n}",
        "num operator<<= (A, B){\n    return A\n}",
        "num operator>>= (A, B){\n    return A\n}",
        "num operator[* (A, B){\n    return A\n}",
        "num operator[-> (A, B){\n    return A\n}",
        "num operator[= (A, B){\n    return A\n}",
        "num operator|-> (A, B){\n    return A\n}",
        "num operator|=> (A, B){\n    return A\n}",
        "num operator|| (A, B){\n    return A\n}",
        "num operator&& (A, B){\n    return A\n}",
        "num operator&&& (A, B){\n    return A\n}",

        "enum colours { red }",
        "enum colours { red, green, blue }",

        " ' (GlobalAttribute) = 5 (~5)",
        "A =  ' (GlobalAttribute)",

        "(((((A) . (B)) . (C)) ' (D)) . (E)) ' (F) = 9 (~9)",

        "stimulus {\n"
        "    #10 (~10)\n"
        "    clk = 1 (~1)\n"
        "    #10 (~10)\n"
        "    clk = 2 (~2)\n"
        "}",
        "emulate {\n"
        "    @(posedge clk, negedge reset)\n"
        "    {\n"
        "        {\n"
        "            a = b\n"
        "        } || {\n"
        "            a = b\n"
        "        }\n"
        "    } && {\n"
        "        a = b\n"
        "    }\n"
        "}",

        "##3 (~3)",
        "c = d",
        "@(enable)",
        "Q = D",

        "a = $(whatnot)",
        "A = $(b)",

        "a = $(whatnot, \"%d\")",
        "a = $(whatnot, ((A) . (B)) . (C))",
        "A = $(((b) . (c)) . (d))",

        "assert ((a) + (b)) == (c)",
        "a = $\"I have {x} sheep\"",
        "a = $\"I have {(x) + (b)} sheep\"",
        "a = $\"I have {(x) + (b), \"04d\"} sheep\"",
        "a = $\"I have {(x) + (b), format} sheep\"",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    Parser _parser;
    parser = &_parser;

    setupTerminal();

    printf("\n");

    if(!testLiterals   ()) goto MainError;
    if(!testIdentifiers()) goto MainError;
    if(!testExpressions()) goto MainError;
    if(!testModules    ()) goto MainError;
    if(!testAutogen    ()) goto MainError;
    if(!testParser     ()) goto MainError;

    endTest("All OK");
    if(ast) delete ast;
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        if(ast) delete ast;
        return -1;
}
//------------------------------------------------------------------------------

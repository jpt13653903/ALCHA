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
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s...\n", name);

    string filename;
    filename  = "testParser/";
    filename += name;
    filename += ".alc";

    if(ast) delete ast;

    Parser parser;
    ast = parser.parse(filename.c_str());

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
            return false;
        }else{
            if(!test(n, node, expected[n])) return false;
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
        "X = 1234560000000",
        "X = 15802368",
        "X = 152919552",
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
        "X = 30",
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
        "X = (1) .. (6)",
        "X = (1) .. (6):(3)",
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
        "X = (A) @ (123)",
        "X = (A) @ (A)",
        "X = A",
        "X =  ' (A)",
        "X = 123",
        "X = 1",
        "X = 0",
        "X = :( A, B, C )",
        "X = :[ A, B, C ]",
        "X = [ A, B, C ]",
        "X = \"Hello There\"",
        "X = (A) + (B)",

        "X = ((A) + ((B) * (C))) - (D)",
        "X = ( & (A)) + ((D) @ (6))",
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

        "pin (5) A",
        "net (123, 456) A",
        "void (A, B) A",
        "auto (C) A",
        "byte (((A) . (B)) . (C)) A",
        "char (((A) . (B)) ' (C)) A",
        "num ((A)[ B ]) A",
        "func (A(B)) A",
        "ABC (7, 9, X = 10) A",
        "(ABC) . (DEF) (7, Y := 9, X = 10) A, B, C",

        "pin (5) <A = a, B = b> A",
        "net (123, 456) <A = a, B = b> A",
        "void (A, B) <A = a, B = b> A",
        "auto (C) <A = a, B = b> A",
        "byte (((A) . (B)) . (C)) <A = a, B = b> A",
        "char (((A) . (B)) ' (C)) <A = a, B = b> A",
        "num ((A)[ B ]) <A = a, B = b> A",
        "func (A(B)) <A = a, B = b> A",
        "ABC (7, 9, X = 10) <A = a, B = b> A",
        "(ABC) . (DEF) (7, Y := 9, X = 10) <A = a, B = b> A, B, C",

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
        "class <A = 3, B = 1, C = 7> MyClass(num D, pin E, byte F){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class MyClass(num D, pin E, byte F): ParentClass1(A, B, C), ParentClass2(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class <A = 3, B = 1, C = 7> MyClass(num D, pin E, byte F): ParentClass1(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",
        "class <A = 3, B = 1, C = 7> MyClass(num D, pin E, byte F): ParentClass1(A, B, C), ParentClass2(A, B, C){\n"
        "    D = A\n"
        "    E = B\n"
        "    F = C\n"
        "}",

        "auto ABC[5](num A, pin B[], net C){\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "auto ABC(num A, pin (8) B, net (3, 15) C = 5){\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "void ABC(A, B, C){\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "inline void ABC(A, B, C){\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",

        "enum ABC { A }",
        "enum ABC { A, B, C }",

        "alias A = ((B) . (C)) * (D)",

        "import \"../whatnot/thingy\"",
        "import \"../whatnot/thingy\" as whatnot",

        "import $\"../{whatnot}/thingy\"",
        "import $\"../{whatnot}/thingy\" as whatnot",

        "import $(filename)",
        "import $(filename) as whatnot",

        "import $(filename)",
        "import $(filename) as whatnot",

        "struct <A = 3, B = 7> ABC {\n"
        "    pin (15, 8) A\n"
        "    net (17) B, C\n"
        "    struct BCD {\n"
        "        pin (15, 8) A\n"
        "        net (17) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "struct <A = 3, B = 7> {\n"
        "    pin (15, 8) A\n"
        "    net (17) B, C\n"
        "    struct {\n"
        "        pin (15, 8) A\n"
        "        net (17) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "group <A = 3, B = 7> ABC {\n"
        "    pin (15, 8) A\n"
        "    net (17) B, C\n"
        "    group <C = 8, E = 5> BCD {\n"
        "        pin (15, 8) A\n"
        "        net (17) B, C\n"
        "    }\n"
        "    num X\n"
        "}",
        "group <voltage = 33/10 (~3.3), capacitance = 1/100000000000 (~1e-11), external_min_delay = 1/2000000000 (~5e-10), external_max_delay = 1/1000000000 (~1e-09)> SD {\n"
        "    pin <location = \"AB6\"> CLK\n"
        "    pin <location = \"W8\"> CMD\n"
        "    pin (4) <location = [ \"U7\", \"T7\", \"V8\", \"T8\" ]> DAT\n"
        "}",

        "pin A[7][8] := [ [ 1, 2, 3 ], [ 4, 5, 6 ], [ 7, 8, 9 ] ]",
        "num A = 4, B = 7, C = 1",

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
        "for ((A) in ((1) .. (6))) {\n"
        "    X++\n"
        "}",
        "for ((A) in (G)) {\n"
        "    X++\n"
        "}",
        "while ((A) < (7)) {\n"
        "    X++\n"
        "}",
        "loop (7) {\n"
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
        "return 5",
        "break 6",
        "continue 7",
        "goto Label",

        "rtl <A = 3, B = \"Hello\"> (Clk, Reset){\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "rtl <A = 3, B = \"Hello\"> {\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "rtl (Clk, Reset){\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "rtl (Clk){\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "rtl {\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "fsm <A = 3, B = \"Hello\"> (Clk, Reset){\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "fsm <A = 3, B = \"Hello\"> {\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "fsm (Clk, Reset){\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "fsm {\n"
        "    A = 5\n"
        "    B = (C) + (3)\n"
        "}",
        "hdl <A = 3, B = \"Hello\"> ABC(\n"
        "    A = 3\n"
        "    B = 5\n"
        "){\n"
        "    input {\n"
        "        pin Clk\n"
        "    }\n"
        "    input {\n"
        "        pin Reset\n"
        "    }\n"
        "    output {\n"
        "        pin (16) Data\n"
        "    }\n"
        "    pin (16, 5) AnotherPin\n"
        "}",

        "stimulus  <A = 3, B = 5>  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "stimulus  <A = 3, B = 5> ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "stimulus  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "stimulus  <A = 3, B = 5>  (1, 2, 3) {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "stimulus  <A = 3, B = 5>  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "emulate  <A = 3, B = 5>  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "emulate  <A = 3, B = 5> ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "emulate  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "emulate  <A = 3, B = 5>  (1, 2, 3) {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "emulate  <A = 3, B = 5>  (1, 2, 3) ABC {\n"
        "    A = 3\n"
        "    B = 5\n"
        "}",
        "{\n"
        "    A = 3\n"
        "    B = 5\n"
        "} || {\n"
        "    X++\n"
        "    Y--\n"
        "}",
        "{\n"
        "    A = 3\n"
        "    B = 5\n"
        "} && {\n"
        "    X++\n"
        "    Y--\n"
        "}",
        "stimulus  (1/1000000000 (~1e-09)) {\n"
        "    A = 5\n"
        "    #5\n"
        "    B = 7\n"
        "    @(posedge Clk)\n"
        "    fence\n"
        "    @(Clk, Reset)\n"
        "    fence\n"
        "    @(posedge Clk, negedge Reset)\n"
        "    fence\n"
        "    wait ((A) == (7))\n"
        "    fence\n"
        "    loop {\n"
        "        #7\n"
        "        C++\n"
        "    }\n"
        "}",
        "assert {\n"
        "    (A) == (B)\n"
        "}",

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
        "class RegistersDecoder(Bus): AvalonInterface(32, 4096){\n"
        "    (Bus) . (Attach)(this)\n"

        "    private {\n"
        "         ' (RdRegisters) = [  ]\n"
        "         ' (WrRegisters) = [  ]\n"
        "         ' (LiveRegisters) = [  ]\n"
        "        num Count = 0\n"
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

        "        net (32) Resize(x){\n"
        "            result := x\n"
        "            num N = (x) ' (width)\n"
        "            if ((((x) ' (fullscale)) < (0)) & ((N) < (31))) {\n"
        "                result((31) .. ((N) + (1))) := (x(N)) ` ((31) - (N))\n"
        "            }\n"
        "        }\n"

        "        void GenerateRegs(){\n"
        "            rtl ((Bus) . (Clock), (Bus) . (Reset)){\n"
        "                WaitRequest = 0\n"
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

bool testVerification()
{
    if(!startTest("Verification")) return false;

    const char* expected[] = {
        "a = (((b) + (c)) + (d)) + (e)",
        "wait (((((a) ##(1) (b)) ##(2) (c)) ##(3) (d)) ##(50) (1))",
        "fence",
        "assert {\n"
        "    (a) ##([ (1) .. (4) ]) (b)\n"
        "}",
        "assert {\n"
        "    (s1) ##(2) (s2)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [*(4) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [->(4) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [=(4) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [*((4) .. (8)) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [->((4) .. (8)) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    ((s1) ##(2) ((s2) [=((4) .. (8)) ])) ##(5) (s3)\n"
        "}",
        "assert {\n"
        "    (s1) [*((2) .. (3)) ]\n"
        "}",
        "net (8) Adder(net (8) A, net (8) B, net C){\n"
        "    :( C, Adder ) = (A) + (B)\n"
        "}",
        "net c",
        "net (8) a, b, y = Adder(a, b, c)",
        "stimulus  (1/1000000000 (~1e-09)) {\n"
        "    #1\n"
        "    a = 0\n"
        "    b = 0\n"
        "    assert {\n"
        "        (y) == (0)\n"
        "        (c) == (0)\n"
        "    }\n"
        "    #1\n"
        "    a = 255\n"
        "    b = 1\n"
        "    assert {\n"
        "        (y) == (0)\n"
        "        (c) == (1)\n"
        "    }\n"
        "    #1\n"
        "    a = 1\n"
        "    b = 255\n"
        "    assert {\n"
        "        (y) == (0)\n"
        "        (c) == (1)\n"
        "    }\n"
        "    #1\n"
        "    a = 255\n"
        "    b = 255\n"
        "    assert {\n"
        "        (y) == (254)\n"
        "        (c) == (1)\n"
        "    }\n"
        "    #1\n"
        "    a = 127\n"
        "    b = 128\n"
        "    assert {\n"
        "        (y) == (255)\n"
        "        (c) == (0)\n"
        "    }\n"
        "}",
        "assert {\n"
        "    (((a) == (0)) && ((b) == (0))) |-> (((y) == (0)) && ((c) == (0)))\n"
        "    (((a) == (255)) && ((b) == (1))) |-> (((y) == (0)) && ((c) == (1)))\n"
        "    (((a) == (1)) && ((b) == (255))) |-> (((y) == (0)) && ((c) == (1)))\n"
        "    (((a) == (255)) && ((b) == (255))) |-> (((y) == (254)) && ((c) == (1)))\n"
        "    (((a) == (127)) && ((b) == (128))) |-> (((y) == (255)) && ((c) == (0)))\n"
        "}",
        "sequence Handshake {\n"
        "    (((Go) ##([ (1) .. (5) ]) (Busy)) ##([ (1) .. (10) ]) ( ! (Go))) ##([ (1) .. (100) ]) ( ! (Busy))\n"
        "}",
        "assert (1/1000000000 (~1e-09), Clk){\n"
        "    if ( ~ (Reset)) {\n"
        "        (Go) |-> (Handshake)\n"
        "        (rose(Go)) |-> ( ! (Busy))\n"
        "        (fell(Busy)) |-> ( ! (Go))\n"
        "        ((Go) & ( ! (Busy))) |=> (stable(Data))\n"
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

        "a = 3",
        "b = 5",
        "print((a) + (b))",

        "a++",
        "a()",
        "a--",
        "a <<= ((a) * (b)) + ((c) * (d))",

        "a = ((a) ++ ) + ( ++ (b))",
        "a = (a) ? (b) : (c)",
        "a = ((x) | (x)) ~| (((x) ^ (x)) ~^ (((x) & (x)) ~& (((x) == (x)) != (((((x) < (x)) > (x)) <= (x)) >= (((x) << (x)) >> (((x) + (x)) - ((((x) * (x)) / (x)) % ((x) ** ((x) ` (x))))))))))",
        "a = (((((((((((((((((((((x) ` (x)) ** (x)) % (x)) / (x)) * (x)) - (x)) + (x)) >> (x)) << (x)) >= (x)) <= (x)) > (x)) < (x)) != (x)) == (x)) ~& (x)) & (x)) ~^ (x)) ^ (x)) ~| (x)) | (x)",
        "(((a)[ 5 ])[ 6 ])[ (7) .. (9), 8, (1) .. (10):(2) ] = ((((b) . (c)) . (d)) . (e)) ' (attribute)",

        "MyLabel:",

        "((a) . (b)) . (c).{\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "    num a, b, c\n"
        "}",

        "num myFunc(a, b, c = 5){\n"
        "    print(((a) * (b)) + (c))\n"
        "}",
        "myFuct(1, 2, 3)",

        "inline num (15, 7) <a = b, c = d> myFunc("
             "a, num b, c = 5, pin d = 8, pin (13, 9) e, "
             "MyClass myClass, MyClass (123, 456) myClass){\n"
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

        "MyClass ((a) + ((d) * (c)), (b) - (e)) <a = (b) ** (2), c = (d) / (r), t = (g) > (5)> myClass",

        "class MyClass{\n"
        "    print(\"Hello\")\n"
        "}",
        "class <a = b, c = d> MyClass{\n"
        "    print(\"Hello\")\n"
        "}",
        "class <a = b, c = d> MyClass(a, b, c = 123){\n"
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

        " ' (GlobalAttribute) = 5",
        "A =  ' (GlobalAttribute)",

        "(((((A) . (B)) . (C)) ' (D)) . (E)) ' (F) = 9",

        "stimulus {\n"
        "    #10\n"
        "    clk = 1\n"
        "    #10\n"
        "    clk = 2\n"
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

        "##3",
        "c = d",
        "@(enable)",
        "Q = D",

        "a = $(whatnot)",
        "A = $(b)",

        "a = $(whatnot, \"%d\")",
        "a = $(whatnot, ((A) . (B)) . (C))",
        "A = $(((b) . (c)) . (d))",

        "assert {\n"
        "    ((a) + (b)) == (c)\n"
        "}",
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

bool testAlchaCaseStudyCombined()
{
    if(!startTest("AlchaCaseStudyCombined")) return false;

    const char* expected[] = {
        "net DelayedReset(net Clk, net Reset, num Delay_ms){\n"
        "    num Delay_cycles = round(((Clk) ' (frequency)) * ((Delay_ms) * (1/1000 (~0.001))))\n"
        "    num N = ceil(log2(Delay_cycles))\n"
        "    net (N) Count = 0\n"
        "    result = 1\n"
        "    rtl (Clk, Reset){\n"
        "        if ((Count) != (Delay_cycles)) {\n"
        "            Count++\n"
        "        } else {\n"
        "            result = 0\n"
        "        }\n"
        "    }\n"
        "}",

        "class PLL_CycloneV(net Clk, net Reset, Output_MHz){\n"
        "    private {\n"
        "        num N = (Output_MHz) ' (length)\n"
        "        num OutputFreq[17]\n"
        "        for ((n) in ((0) .. (16))) {\n"
        "            if ((n) < (N)) {\n"
        "                (OutputFreq)[ n ] = (Output_MHz)[ n ]\n"
        "            } else {\n"
        "                (OutputFreq)[ n ] = 0\n"
        "            }\n"
        "        }\n"
        "        hdl altera_pll(\n"
        "            fractional_vco_multiplier = \"false\"\n"
        "            reference_clock_frequency = :[ $(((Clk) ' (frequency)) / (1000000)), \" MHz\" ]\n"
        "            operation_mode = \"direct\"\n"
        "            number_of_clocks = N\n"
        "            output_clock_frequency0 = :[ $((OutputFreq)[ 0 ]), \" MHz\" ]\n"
        "            phase_shift0 = \"0 ps\"\n"
        "            duty_cycle0 = 50\n"
        "            output_clock_frequency1 = :[ $((OutputFreq)[ 1 ]), \" MHz\" ]\n"
        "            phase_shift1 = \"0 ps\"\n"
        "            duty_cycle1 = 50\n"
        "            output_clock_frequency2 = :[ $((OutputFreq)[ 2 ]), \" MHz\" ]\n"
        "            phase_shift2 = \"0 ps\"\n"
        "            duty_cycle2 = 50\n"
        "            output_clock_frequency3 = :[ $((OutputFreq)[ 3 ]), \" MHz\" ]\n"
        "            phase_shift3 = \"0 ps\"\n"
        "            duty_cycle3 = 50\n"
        "            output_clock_frequency4 = :[ $((OutputFreq)[ 4 ]), \" MHz\" ]\n"
        "            phase_shift4 = \"0 ps\"\n"
        "            duty_cycle4 = 50\n"
        "            output_clock_frequency5 = :[ $((OutputFreq)[ 5 ]), \" MHz\" ]\n"
        "            phase_shift5 = \"0 ps\"\n"
        "            duty_cycle5 = 50\n"
        "            output_clock_frequency6 = :[ $((OutputFreq)[ 6 ]), \" MHz\" ]\n"
        "            phase_shift6 = \"0 ps\"\n"
        "            duty_cycle6 = 50\n"
        "            output_clock_frequency7 = :[ $((OutputFreq)[ 7 ]), \" MHz\" ]\n"
        "            phase_shift7 = \"0 ps\"\n"
        "            duty_cycle7 = 50\n"
        "            output_clock_frequency8 = :[ $((OutputFreq)[ 8 ]), \" MHz\" ]\n"
        "            phase_shift8 = \"0 ps\"\n"
        "            duty_cycle8 = 50\n"
        "            output_clock_frequency9 = :[ $((OutputFreq)[ 9 ]), \" MHz\" ]\n"
        "            phase_shift9 = \"0 ps\"\n"
        "            duty_cycle9 = 50\n"
        "            output_clock_frequency10 = :[ $((OutputFreq)[ 10 ]), \" MHz\" ]\n"
        "            phase_shift10 = \"0 ps\"\n"
        "            duty_cycle10 = 50\n"
        "            output_clock_frequency11 = :[ $((OutputFreq)[ 11 ]), \" MHz\" ]\n"
        "            phase_shift11 = \"0 ps\"\n"
        "            duty_cycle11 = 50\n"
        "            output_clock_frequency12 = :[ $((OutputFreq)[ 12 ]), \" MHz\" ]\n"
        "            phase_shift12 = \"0 ps\"\n"
        "            duty_cycle12 = 50\n"
        "            output_clock_frequency13 = :[ $((OutputFreq)[ 13 ]), \" MHz\" ]\n"
        "            phase_shift13 = \"0 ps\"\n"
        "            duty_cycle13 = 50\n"
        "            output_clock_frequency14 = :[ $((OutputFreq)[ 14 ]), \" MHz\" ]\n"
        "            phase_shift14 = \"0 ps\"\n"
        "            duty_cycle14 = 50\n"
        "            output_clock_frequency15 = :[ $((OutputFreq)[ 15 ]), \" MHz\" ]\n"
        "            phase_shift15 = \"0 ps\"\n"
        "            duty_cycle15 = 50\n"
        "            output_clock_frequency16 = :[ $((OutputFreq)[ 16 ]), \" MHz\" ]\n"
        "            phase_shift16 = \"0 ps\"\n"
        "            duty_cycle16 = 50\n"
        "            output_clock_frequency17 = :[ $((OutputFreq)[ 17 ]), \" MHz\" ]\n"
        "            phase_shift17 = \"0 ps\"\n"
        "            duty_cycle17 = 50\n"
        "            pll_type = \"General\"\n"
        "            pll_subtype = \"General\"\n"
        "        ){\n"
        "            net refclk = Clk\n"
        "            net rst = Reset\n"
        "            net fbclk = 0\n"
        "            net (N) outclk\n"
        "            net locked\n"
        "            net fboutclk\n"
        "        }\n"
        "        altera_pll PLL\n"
        "    }\n"
        "    public {\n"
        "        net Locked = (PLL) . (locked)\n"
        "        net Output[N]\n"
        "        for ((n) in ((0) .. ((N) - (1)))) {\n"
        "            ((Output)[ n ]) ' (frequency) = (Output_MHz) * (1000000)\n"
        "            (Output)[ n ] = ((PLL) . (outclk))[ n ]\n"
        "        }\n"
        "    }\n"
        "}",

        "class WhatchDog(net Clk, net Kick, num Timeout_ms, bool EdgeSensitive = 0){\n"
        "    private {\n"
        "        num Timeout_Cycles = round(((Clk) ' (frequency)) * ((Timeout_ms) * (1/1000 (~0.001))))\n"
        "        num N = ceil(log2(Delay_cycles))\n"
        "        net (N) Count = (Timeout_Cycles) - (1)\n"
        "        if (EdgeSensitive) {\n"
        "            net (2) KickEdge\n"
        "            rtl (Clk){\n"
        "                KickEdge = :( (KickEdge)[ 0 ], Kick )\n"
        "            }\n"
        "            net Reset = (KickEdge) == (1)\n"
        "        } else {\n"
        "            net Reset = Kick\n"
        "        }\n"
        "    }\n"
        "    output {\n"
        "        net Error = 0\n"
        "    }\n"
        "    rtl (Clk, Reset){\n"
        "        if (Count) {\n"
        "            Count--\n"
        "        } else {\n"
        "            Error = 1\n"
        "        }\n"
        "    }\n"
        "}",

        "class I2C(net Clk, net Reset, num Baud_kHz = 50){\n"
        "    input {\n"
        "        net Quiet = 0\n"
        "    }\n"
        "    output {\n"
        "        net Error = 0\n"
        "    }\n"
        "    private {\n"
        "        num Baud_Cycles = round(((Clk) ' (frequency)) / ((Baud_kHz) * (3)))\n"
        "        num Baud_N = ceil(log2(Baud_Cycles))\n"
        "        net (Baud_N) Baud_Count\n"
        "        rtl (Clk){\n"
        "            if (Baud_Count) {\n"
        "                Baud_Count--\n"
        "            } else {\n"
        "                Baud_Count = (Baud_Cycles) - (1)\n"
        "            }\n"
        "        }\n"
        "        net Baud_Trigger =  ~| (Baud_Count)\n"
        "        net ClkIn, ClkOut = 1\n"
        "        net DataIn, DataOut = 1\n"
        "    }\n"
        "    void ConnectBus(pin Clock, pin Data){\n"
        "        (Clock) . (enable) |=  ~ (ClkOut)\n"
        "        (Data) . (enable) |=  ~ (DataOut)\n"
        "        rtl (Clk){\n"
        "            ClkIn = (Clock) . (pad)\n"
        "            DataIn = (Data) . (pad)\n"
        "        }\n"
        "    }\n"
        "    net (8) Transaction(net (8) TxData, bool Start = 0, bool R_nW = 0, bool Ack = 1, bool Stop = 0){\n"
        "        net (8) Data\n"
        "        fsm (Clk, Reset, ((Baud_Trigger) & ((ClkIn) == (ClkOut))) & ( ! (Quiet))){\n"
        "            Data = TxData\n"
        "            if ((Start) | (ClkOut)) {\n"
        "                if (( ! (ClkOut)) & ( ! (DataOut))) {\n"
        "                    DataOut = 1\n"
        "                }\n"
        "                if (( ! (ClkOut)) & (DataOut)) {\n"
        "                    ClkOut = 1\n"
        "                }\n"
        "                if ((ClkOut) & (DataOut)) {\n"
        "                    DataOut = 0\n"
        "                }\n"
        "                if ((ClkOut) & ( ! (DataOut))) {\n"
        "                    ClkOut = 0\n"
        "                }\n"
        "            }\n"
        "            for ((n) in ((0) .. (7))) {\n"
        "                if (R_nW) {\n"
        "                    DataOut = 1\n"
        "                } else {\n"
        "                    DataOut = (Data)[ 7 ]\n"
        "                }\n"
        "                ClkOut = 1\n"
        "                ClkOut = 0\n"
        "                Data = :( (Data)[ (6) .. (0) ], DataIn )\n"
        "            }\n"
        "            if (R_nW) {\n"
        "                result = Data\n"
        "                DataOut =  ! (Ack)\n"
        "                ClkOut = 1\n"
        "                ClkOut = 0\n"
        "            } else {\n"
        "                DataOut = 1\n"
        "                ClkOut = 1\n"
        "                if ((DataIn) != (Ack)) {\n"
        "                    Error = 1\n"
        "                    return\n"
        "                } else {\n"
        "                    ClkOut = 0\n"
        "                    Error = 0\n"
        "                }\n"
        "            }\n"
        "            if (Stop) {\n"
        "                if ((ClkOut) & (DataOut)) {\n"
        "                    ClkOut = 0\n"
        "                }\n"
        "                if (( ! (ClkOut)) & (DataOut)) {\n"
        "                    DataOut = 0\n"
        "                }\n"
        "                if (( ! (ClkOut)) & ( ! (DataOut))) {\n"
        "                    ClkOut = 1\n"
        "                }\n"
        "                if ((ClkOut) & ( ! (DataOut))) {\n"
        "                    DataOut = 1\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "import \"../Memory/FullDualPortRAM\"",

        "class FFT(net Clk, net Reset, num NumPoints){\n"
        "    assert {\n"
        "        ((2) ** (round(log2(NumPoints)))) == (NumPoints)\n"
        "    }\n"
        "    private {\n"
        "        DualPortROM (Clk, Reset, 36, NumPoints) TwiddleFactors\n"
        "        FullDualPortRAM (Clk, Reset, 80, NumPoints) Buffer\n"
        "    }\n"
        "    IQ_Stream Run(Input, bool ComplexInput){\n"
        "        result = IQ_Stream(31,  - (1))\n"
        "        ((result) . (I)) ' (overflow) = \"clip\"\n"
        "        ((result) . (Q)) ' (overflow) = \"clip\"\n"
        "        num BufferFullScale\n"
        "        if (ComplexInput) {\n"
        "            BufferFullScale = (((Input) . (I)) ' (fullscale)) * (NumPoints)\n"
        "        } else {\n"
        "            BufferFullScale = (((Input) . (Data)) ' (fullscale)) * (NumPoints)\n"
        "        }\n"
        "        struct ComplexData {\n"
        "            net (39, BufferFullScale) I, Q\n"
        "        }\n"
        "        ComplexData Buffer_WrData_A, Buffer_WrData_B\n"
        "        ComplexData Buffer_RdData_A, Buffer_RdData_B\n"
        "        (Buffer) . (WrData_A) := :( (Buffer_WrData_A) . (I), (Buffer_WrData_A) . (Q) )\n"
        "        (Buffer) . (WrData_B) := :( (Buffer_WrData_B) . (I), (Buffer_WrData_B) . (Q) )\n"
        "        :( (Buffer_RdData_A) . (I), (Buffer_RdData_A) . (Q) ) := (Buffer) . (RdData_A)\n"
        "        :( (Buffer_RdData_B) . (I), (Buffer_RdData_B) . (Q) ) := (Buffer) . (RdData_B)\n"
        "    }\n"
        "}",

        "DataStream operator| (DataStream Input, FFT Engine){\n"
        "    AddDebug((Input) . (SoP))\n"
        "    AddDebug((Input) . (EoP))\n"
        "    AddDebug((Input) . (Valid))\n"
        "    AddDebug((result) . (Ready))\n"
        "    return (Engine) . (Run)(Input, 0)\n"
        "}",

        "IQ_Stream operator| (IQ_Stream Input, FFT Engine){\n"
        "    AddDebug((Input) . (SoP))\n"
        "    AddDebug((Input) . (EoP))\n"
        "    AddDebug((Input) . (Valid))\n"
        "    AddDebug((result) . (Ready))\n"
        "    return (Engine) . (Run)(Input, 1)\n"
        "}",

        "import \"../Memory/DualPortRAM\"",

        "class FIFO(net Clk, net Reset, num Length){\n"
        "    DataStream Run(Input){\n"
        "        num Width = ((Input) . (Data)) ' (width)\n"
        "        DualPortRAM (Clk, Reset, (Width) + (2), Length) RAM\n"
        "        result = DataStream(Width,  - (1))\n"
        "        (RAM) . (WrAddress) = 0\n"
        "        (RAM) . (WrData) := :( (Input) . (SoP), (Input) . (EoP), (Input) . (Data) )\n"
        "        (RAM) . (WrEnable) = (Input) . (Valid)\n"
        "        (RAM) . (RdEnable) = (Output) . (Ready)\n"
        "        (RAM) . (RdAddress) = 0\n"
        "        rtl (Clk, Reset){\n"
        "            if ((Input) . (Valid)) {\n"
        "                (RAM) . (WrAddress)++\n"
        "            }\n"
        "        }\n"
        "        net Valid = 0\n"
        "        num (((RAM) . (RdAddress)) ' (width)) NumItems = ((RAM) . (WrAddress)) - ((RAM) . (RdAddress))\n"
        "        rtl (Clk, Reset, (result) . (Ready)){\n"
        "            :( (result) . (SoP), (result) . (EoP), (result) . (Data) ) := (RAM) . (RdData)\n"
        "            (result) . (Valid) = Valid\n"
        "            if ((NumItems) != (0)) {\n"
        "                Valid = 1\n"
        "                (RAM) . (RdAddress)++\n"
        "            } else {\n"
        "                Valid = 0\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "DataStream operator| (Input, FIFO Queue){\n"
        "    AddDebug((Input) . (SoP))\n"
        "    AddDebug((Input) . (EoP))\n"
        "    AddDebug((Input) . (Valid))\n"
        "    AddDebug((result) . (Ready))\n"
        "    return (Queue) . (Run)(Input)\n"
        "}",

        "import \"SinCos\"",
        "class NCO(net Clk, net Reset){\n"
        "    input {\n"
        "        net (32, ( - ((Clk) ' (frequency))) / (2)) Frequency\n"
        "    }\n"
        "    output {\n"
        "        net (18,  - (1)) Sin, Cos\n"
        "    }\n"
        "    private {\n"
        "        net (32,  - (pi)) Phase = 0\n"
        "    }\n"
        "    rtl (Clk, Reset){\n"
        "        Phase := ( : (Phase)) + ( : (Frequency))\n"
        "    }\n"
        "    SinCos(Clk, Phase, Sin, Cos)\n"
        "}",

        "void SinCos(net Clk, net (20,  - (pi)) Angle, net (18,  - (1)) Sin, Cos, num N = 20){\n"
        "    num K = product(cos(atan((2) ** ((0) .. ((N) - (1))))))\n"
        "    num n\n"
        "    net (21,  - (2)) x[(N) + (1)], y[(N) + (1)]\n"
        "    net (21, ( - (2)) * (pi)) a[(N) + (1)], A[(N) + (1)]\n"
        "    rtl (Clk){\n"
        "        switch ((Angle)[ 20, 19 ]) {\n"
        "            case (0) {\n"
        "                (x)[ 0 ] = K\n"
        "                (y)[ 0 ] = 0\n"
        "                (a)[ 0 ] = 0\n"
        "            }\n"
        "            case (1) {\n"
        "                (x)[ 0 ] = 0\n"
        "                (y)[ 0 ] = K\n"
        "                (a)[ 0 ] = (pi) / (2)\n"
        "            }\n"
        "            case (2) {\n"
        "                (x)[ 0 ] =  - (K)\n"
        "                (y)[ 0 ] = 0\n"
        "                (a)[ 0 ] =  - (pi)\n"
        "            }\n"
        "            case (3) {\n"
        "                (x)[ 0 ] = 0\n"
        "                (y)[ 0 ] =  - (K)\n"
        "                (a)[ 0 ] = ( - (pi)) / (2)\n"
        "            }\n"
        "        }\n"
        "        (A)[ 0 ] = Angle\n"
        "        for ((n) in ((0) .. ((N) - (1)))) {\n"
        "            if (((A)[ n ]) >= ((a)[ n ])) {\n"
        "                (x)[ (n) + (1) ] = ((x)[ n ]) - (((y)[ n ]) / ((2) ** (n)))\n"
        "                (y)[ (n) + (1) ] = ((y)[ n ]) + (((x)[ n ]) / ((2) ** (n)))\n"
        "                (a)[ (n) + (1) ] = ((a)[ n ]) + (atan((2) ** ( - (n))))\n"
        "            } else {\n"
        "                (x)[ (n) + (1) ] = ((x)[ n ]) + (((y)[ n ]) / ((2) ** (n)))\n"
        "                (y)[ (n) + (1) ] = ((y)[ n ]) - (((x)[ n ]) / ((2) ** (n)))\n"
        "                (a)[ (n) + (1) ] = ((a)[ n ]) - (atan((2) ** ( - (n))))\n"
        "            }\n"
        "            (A)[ (n) + (1) ] = (A)[ n ]\n"
        "        }\n"
        "    }\n"
        "    group <overflow = \"clip\"> {\n"
        "        Sin = (y)[ N ]\n"
        "        Cos = (x)[ N ]\n"
        "    }\n"
        "}",

        "class DataStream(num Width, num Fullscale =  - (1)){\n"
        "    net SoP\n"
        "    net EoP\n"
        "    net (Width, Fullscale) Data\n"
        "    net Valid = 0\n"
        "    net Ready = 1\n"
        "}",

        "class IQ_Stream(num Width, num Fullscale =  - (1)){\n"
        "    net SoP\n"
        "    net EoP\n"
        "    net (Width, Fullscale) I\n"
        "    net (Width, Fullscale) Q\n"
        "    net Valid = 0\n"
        "    net Ready = 1\n"
        "}",

        "import \"../Memory/DualPortROM\"",
        "class Window(net Clk, net Reset, num Length, string Function){\n"
        "    DualPortROM (Clk, Reset, 18, Length) ROM\n"
        "    private {\n"
        "        num n[Length] = (0) .. ((Length) - (1))\n"
        "        switch (Function) {\n"
        "            case (\"Hann\") {\n"
        "                (ROM) . (Initial) = (sin(((pi) * (n)) / (Length))) ** (2)\n"
        "            }\n"
        "            case (\"Hamming\") {\n"
        "                num a = (25) / (46)\n"
        "                (ROM) . (Initial) = (a) - (((1) - (a)) * (cos((((2) * (pi)) * (n)) / (Length))))\n"
        "            }\n"
        "            default {\n"
        "                (ROM) . (Initial) = 1\n"
        "            }\n"
        "        }\n"
        "        num Sum = 0\n"
        "        for ((c) in ((ROM) . (Initial))) {\n"
        "            Sum += c\n"
        "        }\n"
        "        (ROM) . (Initial) /= Sum\n"
        "        num Max = 1/1000000000000000000000000 (~1e-24)\n"
        "        for ((c) in ((ROM) . (Initial))) {\n"
        "            if ((Max) < (c)) {\n"
        "                Max = c\n"
        "            }\n"
        "        }\n"
        "        num Fullscale = 1\n"
        "        while ((Fullscale) > (Max)) {\n"
        "            Fullscale /= 2\n"
        "        }\n"
        "        while ((Fullscale) < (Max)) {\n"
        "            Fullscale *= 2\n"
        "        }\n"
        "        (ROM) . (Initial) = round(((ROM) . (Initial)) * (((262144) - (1)) / (Fullscale)))\n"
        "        net (Width, Fullscale) Coefficient := (ROM) . (Data_A)\n"
        "    }\n"
        "    auto Run(Input, bool Complex){\n"
        "        net WaitForROM = 1\n"
        "        net Ready =  ! (WaitForROM)\n"
        "        if (Complex) {\n"
        "            result = IQ_Stream(((Input) . (Width)) + (18),  - (Fullscale))\n"
        "        } else {\n"
        "            result = DataStream(((Input) . (Width)) + (18),  - (Fullscale))\n"
        "        }\n"
        "        (Input) . (Ready) = ((result) . (Ready)) & (Ready)\n"
        "        (ROM) . (ClkEnable_A) = (((result) . (Ready)) & ((Input) . (Valid))) | (WaitForROM)\n"
        "        rtl (Clk, Reset, (result) . (Ready)){\n"
        "            if (Ready) {\n"
        "                (result) . (SoP) = (Input) . (SoP)\n"
        "                (result) . (EoP) = (Input) . (EoP)\n"
        "                (result) . (Valid) = (Input) . (Valid)\n"
        "                if (Complex) {\n"
        "                    (result) . (I) = ((Input) . (I)) * (Coefficient)\n"
        "                    (result) . (Q) = ((Input) . (Q)) * (Coefficient)\n"
        "                } else {\n"
        "                    (result) . (Data) = ((Input) . (Data)) * (Coefficient)\n"
        "                }\n"
        "            }\n"
        "            if (WaitForROM) {\n"
        "                (ROM) . (Address_A)++\n"
        "                WaitForROM = 0\n"
        "            } else {\n"
        "                if ((Input) . (Valid)) {\n"
        "                    if ((Input) . (EoP)) {\n"
        "                        (ROM) . (Address_A) = 0\n"
        "                        WaitForROM = 1\n"
        "                    } else {\n"
        "                        (ROM) . (Address_A)++\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "DataStream operator| (DataStream Input, Window Instance){\n"
        "    AddDebug((Input) . (SoP))\n"
        "    AddDebug((Input) . (EoP))\n"
        "    AddDebug((Input) . (Valid))\n"
        "    AddDebug((result) . (Ready))\n"
        "    return (Instance) . (Run)(Input, 0)\n"
        "}",

        "IQ_Stream operator| (IQ_Stream Input, Window Instance){\n"
        "    AddDebug((Input) . (SoP))\n"
        "    AddDebug((Input) . (EoP))\n"
        "    AddDebug((Input) . (Valid))\n"
        "    AddDebug((result) . (Ready))\n"
        "    return (Instance) . (Run)(Input, 1)\n"
        "}",

        "class AvalonInterface(num Width, num Depth, num MaxBurstCount = 1){\n"
        "    num AddressWidth = ceil(log2(Depth))\n"
        "    num NumBytes = ceil((Width) / (8))\n"
        "    num BurstCountWidth = ceil(log2((MaxBurstCount) + (1)))\n"
        "    net (AddressWidth) Address\n"
        "    net (NumBytes) ByteEnable = ((2) ** (NumBytes)) - (1)\n"
        "    net (BurstCountWidth) BurstCount = 1\n"
        "    net WaitRequest = 1\n"
        "    net (Width) WriteData\n"
        "    net Write = 0\n"
        "    net Read = 0\n"
        "    net (Width) ReadData\n"
        "    net ReadValid = 0\n"
        "}",

        "import \"Library/Interfaces/AvalonInterface\"",

        "class AvalonMaster(net Clock, net Reset, num Width, num Depth): AvalonInterface(Width, Depth){\n"
        "     ' (BaseAddress) = 0\n"
        "    private {\n"
        "        num Used = 0\n"
        "        net ReadingWaitRequest = 0\n"
        "        net InterfacesWaitRequest = 0\n"
        "    }\n"
        "    ReadData = 0\n"
        "    WaitRequest = (ReadingWaitRequest) | (InterfacesWaitRequest)\n"
        "    void Attach(AvalonInterface Interface){\n"
        "        assert {\n"
        "            ((Interface) . (Width)) == (Width)\n"
        "        }\n"
        "        assert {\n"
        "            ((Interface) . (Depth)) < ((Depth) - (Used))\n"
        "        }\n"
        "        (Interface) . (Address) = (Address) - (Used)\n"
        "        (Interface) . (ByteEnable) = ByteEnable\n"
        "        InterfacesWaitRequest |= (Interface) . (WaitRequest)\n"
        "        net AddressValid = ((Address) >= (Used)) & ((Address) < ((Used) + ((Interface) . (Depth))))\n"
        "        (Interface) . (WriteData) = WriteData\n"
        "        (Interface) . (Write) = ((AddressValid) & (Write)) & ( ! (WaitRequest))\n"
        "        (Interface) . (Read) = ((AddressValid) & (Read)) & ( ! (WaitRequest))\n"
        "        ReadData |= ((Interface) . (ReadValid)) ? ((Interface) . (ReadData)) : (0)\n"
        "        ReadValid |= (Interface) . (ReadValid)\n"
        "        (Interface) ' (BaseAddress) = ( ' (BaseAddress)) + ((Used) * ((Width) / (8)))\n"
        "        Used += (Interface) . (Depth)\n"
        "    }\n"
        "    rtl (Clock, Reset){\n"
        "        if ((Read) & ( ! (WaitRequest))) {\n"
        "            ReadingWaitRequest = 1\n"
        "        } else {\n"
        "            if (ReadValid) {\n"
        "                ReadingWaitRequest = 0\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "import \"Library/Interfaces/AvalonInterface\"",
        "class AvalonSlave(net Clock, net Reset, num Width, num Depth, num MaxBurstCount = 1): AvalonInterface(Width, Depth, MaxBurstCount){\n"
        "    void Attach(Master){\n"
        "    }\n"
        "    void finally(){\n"
        "    }\n"
        "}",

        "class RegistersDecoder(Bus): AvalonInterface(32, 4096){\n"
        "    (Bus) . (Attach)(this)\n"
        "    private {\n"
        "         ' (RdRegisters) = [  ]\n"
        "         ' (WrRegisters) = [  ]\n"
        "         ' (LiveRegisters) = [  ]\n"
        "        num Count = 0\n"
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
        "        net (32) Resize(x){\n"
        "            result := x\n"
        "            num N = (x) ' (width)\n"
        "            if ((((x) ' (fullscale)) < (0)) & ((N) < (31))) {\n"
        "                (result)[ (31) .. ((N) + (1)) ] := (x(N)) ` ((31) - (N))\n"
        "            }\n"
        "        }\n"
        "        void GenerateRegs(){\n"
        "            rtl ((Bus) . (Clock), (Bus) . (Reset)){\n"
        "                WaitRequest = 0\n"
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

        "class DualPortRAM(net Clk, net Reset, num Width, num Depth){\n"
        "    private {\n"
        "        string RamBlockType\n"
        "        switch ( ' (target_series)) {\n"
        "            case (\"MAX 10\") {\n"
        "                RamBlockType = \"M9K\"\n"
        "            }\n"
        "            case (\"Cyclone V\") {\n"
        "                RamBlockType = \"M10K\"\n"
        "            }\n"
        "            case (\"Arria 10\") {\n"
        "                RamBlockType = \"M20K\"\n"
        "            }\n"
        "            default {\n"
        "                assert {\n"
        "                    0\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "        num AddressWidth = ceil(log2(Depth))\n"
        "        hdl altsyncram(\n"
        "            address_aclr_b = \"NONE\"\n"
        "            address_reg_b = \"CLOCK1\"\n"
        "            clock_enable_input_a = \"NORMAL\"\n"
        "            clock_enable_input_b = \"NORMAL\"\n"
        "            clock_enable_output_a = \"BYPASS\"\n"
        "            clock_enable_output_b = \"BYPASS\"\n"
        "            indata_reg_b = \"CLOCK1\"\n"
        "            intended_device_family =  ' (target_series)\n"
        "            lpm_type = \"altsyncram\"\n"
        "            maximum_depth = 1024\n"
        "            numwords_a = Depth\n"
        "            numwords_b = Depth\n"
        "            operation_mode = \"DUAL_PORT\"\n"
        "            outdata_aclr_b = \"NONE\"\n"
        "            outdata_reg_b = \"UNREGISTERED\"\n"
        "            power_up_uninitialized = \"FALSE\"\n"
        "            ram_block_type = RamBlockType\n"
        "            read_during_write_mode_mixed_ports = \"DONT_CARE\"\n"
        "            widthad_a = AddressWidth\n"
        "            widthad_b = AddressWidth\n"
        "            width_a = Width\n"
        "            width_b = Width\n"
        "            width_byteena_a = 1\n"
        "            width_byteena_b = 1\n"
        "        ){\n"
        "            net clock0 = Clk\n"
        "            net clocken0 = 1\n"
        "            net (AddressWidth) address_a\n"
        "            net (Width) data_a\n"
        "            net wren_a\n"
        "            net clock1 = Clk\n"
        "            net clocken1\n"
        "            net (AddressWidth) address_b\n"
        "            net (Width) q_b\n"
        "        }\n"
        "        fence\n"
        "        altsyncram RAM\n"
        "    }\n"
        "    public {\n"
        "        input {\n"
        "            net (AddressWidth) WrAddress\n"
        "        }\n"
        "        input {\n"
        "            net (Width) WrData\n"
        "        }\n"
        "        input {\n"
        "            net WrEnable\n"
        "        }\n"
        "        input {\n"
        "            net (AddressWidth) RdAddress\n"
        "        }\n"
        "        input {\n"
        "            net RdEnable = 1\n"
        "        }\n"
        "        output {\n"
        "            net (Width) RdData\n"
        "        }\n"
        "        (RAM) . (address_a) = WrAddress\n"
        "        (RAM) . (data_a) = WrData\n"
        "        (RAM) . (wren_a) = WrEnable\n"
        "        (RAM) . (clocken1) = RdEnable\n"
        "        (RAM) . (address_b) = RdAddress\n"
        "        RdData = (RAM) . (q_b)\n"
        "    }\n"
        "}",

        "class DualPortROM(net Clk, net Reset, num Width, num Depth){\n"
        "    public {\n"
        "        num Initial[Depth]\n"
        "    }\n"
        "    private {\n"
        "        string RamBlockType\n"
        "        switch ( ' (target_series)) {\n"
        "            case (\"MAX 10\") {\n"
        "                RamBlockType = \"M9K\"\n"
        "            }\n"
        "            case (\"Cyclone V\") {\n"
        "                RamBlockType = \"M10K\"\n"
        "            }\n"
        "            case (\"Arria 10\") {\n"
        "                RamBlockType = \"M20K\"\n"
        "            }\n"
        "            default {\n"
        "                assert {\n"
        "                    0\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "        num AddressWidth = ceil(log2(Depth))\n"
        "        MIF_File = :[ (this) ' (identifier), \".mif\" ]\n"
        "        hdl altsyncram(\n"
        "            address_aclr_b = \"NONE\"\n"
        "            address_reg_b = \"CLOCK1\"\n"
        "            clock_enable_input_a = \"NORMAL\"\n"
        "            clock_enable_input_b = \"NORMAL\"\n"
        "            clock_enable_output_a = \"BYPASS\"\n"
        "            clock_enable_output_b = \"BYPASS\"\n"
        "            indata_reg_b = \"CLOCK1\"\n"
        "            init_file = MIF_File\n"
        "            intended_device_family =  ' (target_series)\n"
        "            lpm_type = \"altsyncram\"\n"
        "            maximum_depth = 1024\n"
        "            numwords_a = Depth\n"
        "            numwords_b = Depth\n"
        "            operation_mode = \"BIDIR_DUAL_PORT\"\n"
        "            outdata_aclr_a = \"NONE\"\n"
        "            outdata_aclr_b = \"NONE\"\n"
        "            outdata_reg_a = \"UNREGISTERED\"\n"
        "            outdata_reg_b = \"UNREGISTERED\"\n"
        "            power_up_uninitialized = \"FALSE\"\n"
        "            ram_block_type = RamBlockType\n"
        "            read_during_write_mode_mixed_ports = \"DONT_CARE\"\n"
        "            widthad_a = AddressWidth\n"
        "            widthad_b = AddressWidth\n"
        "            width_a = Width\n"
        "            width_b = Width\n"
        "            width_byteena_a = 1\n"
        "            width_byteena_b = 1\n"
        "            wrcontrol_wraddress_reg_b = \"CLOCK1\"\n"
        "        ){\n"
        "            net clock0 = Clk\n"
        "            net clocken0\n"
        "            net (AddressWidth) address_a\n"
        "            net (Width) q_a\n"
        "            net clock1 = Clk\n"
        "            net clocken1\n"
        "            net (AddressWidth) address_b\n"
        "            net (Width) q_b\n"
        "        }\n"
        "        fence\n"
        "        altsyncram ROM\n"
        "    }\n"
        "    public {\n"
        "        input {\n"
        "            net ClkEnable_A = 1\n"
        "        }\n"
        "        input {\n"
        "            net (AddressWidth) Address_A = 0\n"
        "        }\n"
        "        output {\n"
        "            net (Width) Data_A\n"
        "        }\n"
        "        input {\n"
        "            net ClkEnable_B = 1\n"
        "        }\n"
        "        input {\n"
        "            net (AddressWidth) Address_B = 0\n"
        "        }\n"
        "        output {\n"
        "            net (Width) Data_B\n"
        "        }\n"
        "        (ROM) . (clocken0) = ClkEnable_A\n"
        "        (ROM) . (address_a) = Address_A\n"
        "        Data_A := (ROM) . (q_a)\n"
        "        (ROM) . (clocken1) = ClkEnable_B\n"
        "        (ROM) . (address_b) = Address_B\n"
        "        Data_B := (ROM) . (q_b)\n"
        "    }\n"
        "    void finally(){\n"
        "        char Buffer = :[ \"-- Autogenerated by DualPortROM.alc\n"
                                 "\", \"\n"
                                 "\", \"WIDTH=\", $(Width), \";\n"
                                 "\", \"DEPTH=\", $(Depth), \";\n"
                                 "\", \"\n"
                                 "\", \"ADDRESS_RADIX=HEX;\n"
                                 "\", \"DATA_RADIX=HEX;\n"
                                 "\", \"\n"
                                 "\", \"CONTENT BEGIN\n"
                                 "\" ]\n"
        "        num n = 0\n"
        "        for ((Value) in (Initial)) {\n"
        "            (Buffer) . (append)(:[ \"  \", $(n, \"04X\"), \": \", $(Value, \"08X\"), \";\n"
        "\" ])\n"
        "            n++\n"
        "        }\n"
        "        (Buffer) . (append)(\"END;\n"
        "\")\n"
        "        textwrite(MIF_File, Buffer)\n"
        "    }\n"
        "}",

        "class DualPortRAM(net Clk, net Reset, num Width, num Depth){\n"
        "    private {\n"
        "        string RamBlockType\n"
        "        switch ( ' (target_series)) {\n"
        "            case (\"MAX 10\") {\n"
        "                RamBlockType = \"M9K\"\n"
        "            }\n"
        "            case (\"Cyclone V\") {\n"
        "                RamBlockType = \"M10K\"\n"
        "            }\n"
        "            case (\"Arria 10\") {\n"
        "                RamBlockType = \"M20K\"\n"
        "            }\n"
        "            default {\n"
        "                assert {\n"
        "                    0\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "        num AddressWidth = ceil(log2(Depth))\n"
        "        hdl altsyncram(\n"
        "            address_aclr_b = \"NONE\"\n"
        "            address_reg_b = \"CLOCK1\"\n"
        "            clock_enable_input_a = \"NORMAL\"\n"
        "            clock_enable_input_b = \"NORMAL\"\n"
        "            clock_enable_output_a = \"BYPASS\"\n"
        "            clock_enable_output_b = \"BYPASS\"\n"
        "            indata_reg_b = \"CLOCK1\"\n"
        "            intended_device_family =  ' (target_series)\n"
        "            lpm_type = \"altsyncram\"\n"
        "            maximum_depth = 1024\n"
        "            numwords_a = Depth\n"
        "            numwords_b = Depth\n"
        "            operation_mode = \"BIDIR_DUAL_PORT\"\n"
        "            outdata_aclr_a = \"NONE\"\n"
        "            outdata_aclr_b = \"NONE\"\n"
        "            outdata_reg_a = \"UNREGISTERED\"\n"
        "            outdata_reg_b = \"UNREGISTERED\"\n"
        "            power_up_uninitialized = \"FALSE\"\n"
        "            ram_block_type = RamBlockType\n"
        "            read_during_write_mode_mixed_ports = \"DONT_CARE\"\n"
        "            read_during_write_mode_port_a = \"NEW_DATA_NO_NBE_READ\"\n"
        "            read_during_write_mode_port_b = \"NEW_DATA_NO_NBE_READ\"\n"
        "            widthad_a = AddressWidth\n"
        "            widthad_b = AddressWidth\n"
        "            width_a = Width\n"
        "            width_b = Width\n"
        "            width_byteena_a = 1\n"
        "            width_byteena_b = 1\n"
        "            wrcontrol_wraddress_reg_b = \"CLOCK1\"\n"
        "        ){\n"
        "            net clock0 = Clk\n"
        "            net clocken0\n"
        "            net (AddressWidth) address_a\n"
        "            net (Width) data_a\n"
        "            net wren_a\n"
        "            net (Width) q_a\n"
        "            net clock1 = Clk\n"
        "            net clocken1\n"
        "            net (AddressWidth) address_b\n"
        "            net (Width) data_b\n"
        "            net wren_b\n"
        "            net (Width) q_b\n"
        "        }\n"
        "        fence\n"
        "        altsyncram RAM\n"
        "    }\n"
        "    public {\n"
        "        input {\n"
        "            net ClkEnable_A\n"
        "        }\n"
        "        input {\n"
        "            net (AddressWidth) Address_A\n"
        "        }\n"
        "        input {\n"
        "            net (Width) WrData_A\n"
        "        }\n"
        "        input {\n"
        "            net WrEnable_A\n"
        "        }\n"
        "        output {\n"
        "            net (Width) RdData_A\n"
        "        }\n"
        "        input {\n"
        "            net ClkEnable_B\n"
        "        }\n"
        "        input {\n"
        "            net (AddressWidth) Address_B\n"
        "        }\n"
        "        input {\n"
        "            net (Width) WrData_B\n"
        "        }\n"
        "        input {\n"
        "            net WrEnable_B\n"
        "        }\n"
        "        output {\n"
        "            net (Width) RdData_B\n"
        "        }\n"
        "        (RAM) . (clocken0) = ClkEnable_A\n"
        "        (RAM) . (address_a) = Address_A\n"
        "        (RAM) . (data_a) = WrData_A\n"
        "        (RAM) . (wren_a) = WrEnable_A\n"
        "        RdData_A = (RAM) . (q_a)\n"
        "        (RAM) . (clocken1) = ClkEnable_B\n"
        "        (RAM) . (address_b) = Address_B\n"
        "        (RAM) . (data_b) = WrData_B\n"
        "        (RAM) . (wren_b) = WrEnable_B\n"
        "        RdData_B = (RAM) . (q_b)\n"
        "    }\n"
        "}",

        "class FirmwareVersion(num Major, num Minor){\n"
        "    private {\n"
        "        num HexToNum(string S){\n"
        "            result = 0\n"
        "            for ((s) in (S)) {\n"
        "                if (((s) >= (\"0\")) & ((s) <= (\"9\"))) {\n"
        "                    result = ((16) * (result)) | ((s) - (\"0\"))\n"
        "                } else {\n"
        "                    if (((s) >= (\"a\")) & ((s) <= (\"f\"))) {\n"
        "                        result = ((16) * (result)) | (((s) - (\"a\")) + (10))\n"
        "                    } else {\n"
        "                        if (((s) >= (\"A\")) & ((s) <= (\"F\"))) {\n"
        "                            result = ((16) * (result)) | (((s) - (\"A\")) + (10))\n"
        "                        }\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    output {\n"
        "        net (32) Version = ((Major) << (16)) | (Minor)\n"
        "        net (32) Date = HexToNum(\"2024-12-29\")\n"
        "        net (28) Time = HexToNum(\"16:58:22\")\n"
        "        net (32) GitHash = HexToNum(shell(\"git rev-parse --short=8 HEAD\"))\n"
        "    }\n"
        "}",

        "class MutEx(net Clk, net Reset){\n"
        "     ' (Modules) = [  ]\n"
        "    void Add(Module){\n"
        "        ( ' (Modules)) . (append)(Module)\n"
        "    }\n"
        "    void finally(){\n"
        "        num N = ( ' (Modules)) ' (length)\n"
        "        assert {\n"
        "            (N) > (0)\n"
        "        }\n"
        "        net (N) Request\n"
        "        net (N) Grant = 0\n"
        "        net (N) Device = 1\n"
        "        rtl (Clk, Reset){\n"
        "            if ((Request) & (Device)) {\n"
        "                Grant = Device\n"
        "            } else {\n"
        "                Grant = 0\n"
        "                Device = :( (Device)[ (N) - ((2) .. (0)) ], (Device)[ (N) - (1) ] )\n"
        "            }\n"
        "        }\n"
        "        for ((n) in ((0) .. ((N) - (1)))) {\n"
        "            (Request)[ n ] = ((( ' (Modules))[ n ]) . (MutEx)) . (Request)\n"
        "            ((( ' (Modules))[ n ]) . (MutEx)) . (Grant) = (Grant)[ n ]\n"
        "        }\n"
        "    }\n"
        "}",

        "class ADS7056(net Clk, net Reset){\n"
        "    assert {\n"
        "        ((Clk) ' (frequency)) <= (120000000)\n"
        "    }\n"
        "    private {\n"
        "        net nCS = 1\n"
        "        net SClk = 0\n"
        "        net Data\n"
        "        net (14) Shift\n"
        "        net (6) Count = 0\n"
        "    }\n"
        "    output {\n"
        "        group Output {\n"
        "            net (14) Data\n"
        "            net Valid = 0\n"
        "        }\n"
        "    }\n"
        "    void ConnectPins(pin nCS, pin SClk, pin SDO){\n"
        "        nCS = (this) . (nCS)\n"
        "        SClk = (this) . (SClk)\n"
        "        Data = SDO\n"
        "        (SClk) ' (create_generated_clock) = divide_by = 2\n"
        "        ((nCS) ' (set_output_delay)) ' (max) += 0\n"
        "        ((nCS) ' (set_output_delay)) ' (min) +=  - (3/1000000000 (~3e-09))\n"
        "        ((SDO) ' (set_input_delay)) ' (min) += 1/400000000 (~2.5e-09)\n"
        "        ((SDO) ' (set_input_delay)) ' (max) += 1/100000000 (~1e-08)\n"
        "        ((SDO) ' (set_multicycle_path)) ' (to) = Clk\n"
        "        ((SDO) ' (set_multicycle_path)) ' (setup) = 2\n"
        "        ((SDO) ' (set_multicycle_path)) ' (hold) = 1\n"
        "    }\n"
        "    rtl (Clk, Reset){\n"
        "        if ((Count) == (47)) {\n"
        "            Count = 0\n"
        "        } else {\n"
        "            Count++\n"
        "        }\n"
        "        switch (Count) {\n"
        "            case (0) {\n"
        "                nCS = 0\n"
        "            }\n"
        "            case ((1) .. (36)) {\n"
        "                SClk =  ~ (SClk)\n"
        "            }\n"
        "            case (37) {\n"
        "                nCS = 1\n"
        "            }\n"
        "            default {\n"
        "                SClk = 0\n"
        "            }\n"
        "        }\n"
        "        if ((SClk) == (0)) {\n"
        "            Shift = :( (Shift)[ (12) .. (0) ], Data )\n"
        "        }\n"
        "        if ((Count) == (32)) {\n"
        "            (Output) . (Data) = Shift\n"
        "            (Output) . (Valid) = 1\n"
        "        } else {\n"
        "            (Output) . (Valid) = 0\n"
        "        }\n"
        "    }\n"
        "}",

        "import \"Library/Comms/I2C/I2C\"",

        "class LTC2991(net Clk, net Reset, net (3) Address, num Baud_kHz = 50): I2C(Clk, Reset, Baud_kHz){\n"
        "    input {\n"
        "        net (5) Enable = 31\n"
        "    }\n"
        "    input {\n"
        "        net (32) Control = 0\n"
        "    }\n"
        "    enum TYPE { Voltage, Differential, Temperature }\n"
        "    void SetType(num Channel, TYPE Type){\n"
        "        assert {\n"
        "            ((Channel) >= (0)) & ((Channel) < (4))\n"
        "        }\n"
        "        switch (Type) {\n"
        "            case (Voltage) {\n"
        "                (Control)[ (((4) * (Channel)) + (3)) .. ((4) * (Channel)) ] = 0\n"
        "            }\n"
        "            case (Differential) {\n"
        "                (Control)[ (((4) * (Channel)) + (3)) .. ((4) * (Channel)) ] = 1\n"
        "            }\n"
        "            case (Temperature) {\n"
        "                (Control)[ (((4) * (Channel)) + (3)) .. ((4) * (Channel)) ] = 2\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    output {\n"
        "        net ( - (15)) Vcc\n"
        "    }\n"
        "    output {\n"
        "        net ( - (15)) V[8]\n"
        "    }\n"
        "    output {\n"
        "        net ( - (15)) InternalTemp\n"
        "    }\n"
        "    group MutEx {\n"
        "        output {\n"
        "            net Request\n"
        "        }\n"
        "        input {\n"
        "            net Grant = Request\n"
        "        }\n"
        "    }\n"
        "    void MakeRegisters(Registers){\n"
        "        (Registers) . (ReadOnly)(Error)\n"
        "        (Registers) . (ReadOnly)(Vcc)\n"
        "        for ((n) in ((0) .. (7))) {\n"
        "            (Registers) . (ReadOnly)((V)[ n ])\n"
        "        }\n"
        "        (Registers) . (ReadOnly)(InternalTemp)\n"
        "    }\n"
        "    fsm (Clk, Reset){\n"
        "    }\n"
        "}",

        "class AD9915(net Clk, net Reset, num Baud_kHz = 1000){\n"
        "    public {\n"
        "        input {\n"
        "            net (32) ipFreqLowerLimit = 0\n"
        "        }\n"
        "        input {\n"
        "            net (32) ipFreqUpperLimit = 0\n"
        "        }\n"
        "        input {\n"
        "            net (32) ipStepUp = 0\n"
        "        }\n"
        "        input {\n"
        "            net (32) ipStepDown = 0\n"
        "        }\n"
        "        input {\n"
        "            net (16) ipSlopeUp = 1\n"
        "        }\n"
        "        input {\n"
        "            net (16) ipSlopeDown = 1\n"
        "        }\n"
        "        input {\n"
        "            net ipUpdate = 0\n"
        "        }\n"
        "        output {\n"
        "            net opBusy = 1\n"
        "        }\n"
        "        input {\n"
        "            net ipTrigger = 0\n"
        "        }\n"
        "    }\n"
        "    private {\n"
        "        net (32) FreqLowerLimit =  ~ (ipFreqLowerLimit)\n"
        "        net (32) FreqUpperLimit =  ~ (ipFreqUpperLimit)\n"
        "        net (32) StepUp =  ~ (ipStepUp)\n"
        "        net (32) StepDown =  ~ (ipStepDown)\n"
        "        net (16) SlopeUp =  ~ (ipSlopeUp)\n"
        "        net (16) SlopeDown =  ~ (ipSlopeDown)\n"
        "    }\n"
        "    private {\n"
        "        net opSClk = 0\n"
        "        net opnCS = 0\n"
        "        net opSDIO = 0\n"
        "        net opSyncIO = 0\n"
        "        net opIO_Update = 0\n"
        "        net opDR_Control = 0\n"
        "        net opDR_Hold = 0\n"
        "        net ipDR_Over = 1\n"
        "    }\n"
        "    void ConnectSPI(pin SClk, pin nCS, pin SDIO, pin SyncIO, pin IO_Update){\n"
        "        SClk = opSClk\n"
        "        nCS = opnCS\n"
        "        SDIO = opSDIO\n"
        "        SyncIO = opSyncIO\n"
        "        IO_Update = opIO_Update\n"
        "    }\n"
        "    void ConnectDR(pin Control, pin Hold, pin Over){\n"
        "        Control = opDR_Control\n"
        "        Hold = opDR_Hold\n"
        "        ipDR_Over = Over\n"
        "    }\n"
        "    private {\n"
        "        net RegsChanged\n"
        "    }\n"
        "    private {\n"
        "        void Send_SPI(net (8) Address, net (32) Value){\n"
        "            net (40) Data\n"
        "            opSDIO = (Data)[ 39 ]\n"
        "            fsm {\n"
        "                RegsChanged = 1\n"
        "                Data = :( Address, Value )\n"
        "                opSyncIO = 1\n"
        "                opSyncIO = 0\n"
        "                loop (40) {\n"
        "                    opSClk = 1\n"
        "                    opSClk = 0\n"
        "                    Data <<= 1\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    private {\n"
        "        void Init(){\n"
        "            fsm {\n"
        "                Send_SPI(0, 65546)\n"
        "                Send_SPI(1, 534784)\n"
        "                Send_SPI(3, 17113376)\n"
        "                opIO_Update = 1\n"
        "                opIO_Update = 0\n"
        "                loop (32768) {\n"
        "                    fence\n"
        "                }\n"
        "                Send_SPI(3, 336160)\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    private {\n"
        "        void UpdateRegisters(){\n"
        "            fsm {\n"
        "                if ((ipFreqLowerLimit) != (FreqLowerLimit)) {\n"
        "                    FreqLowerLimit = ipFreqLowerLimit\n"
        "                    Send_SPI(4, ipFreqLowerLimit)\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "                if ((ipFreqUpperLimit) != (FreqUpperLimit)) {\n"
        "                    FreqUpperLimit = ipFreqUpperLimit\n"
        "                    Send_SPI(5, ipFreqUpperLimit)\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "                if ((ipStepUp) != (StepUp)) {\n"
        "                    StepUp = ipStepUp\n"
        "                    Send_SPI(6, ipStepUp)\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "                if ((ipStepDown) != (StepDown)) {\n"
        "                    StepDown = ipStepDown\n"
        "                    Send_SPI(7, ipStepDown)\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "                if (((ipSlopeUp) != (SlopeUp)) | ((ipSlopeDown) != (SlopeDown))) {\n"
        "                    SlopeUp = ipSlopeUp\n"
        "                    SlopeDown = ipSlopeDown\n"
        "                    Send_SPI(8, :( ipSlopeDown, ipSlopeUp ))\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "                if (RegsChanged) {\n"
        "                    opIO_Update = 1\n"
        "                    opIO_Update = 0\n"
        "                    RegsChanged = 0\n"
        "                } else {\n"
        "                    fence\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    fsm (Clk, Reset){\n"
        "        Init()\n"
        "        UpdateRegisters()\n"
        "        loop {\n"
        "            if (opBusy) {\n"
        "                while (ipUpdate) {\n"
        "                    fence\n"
        "                }\n"
        "                opBusy = 0\n"
        "            }\n"
        "            while ( ~ (ipUpdate)) {\n"
        "                fence\n"
        "            }\n"
        "            opBusy = 1\n"
        "            UpdateRegisters()\n"
        "        }\n"
        "    }\n"
        "    rtl (Clk, Reset){\n"
        "        Trigger = :( (Trigger)[ 0 ], ipTrigger )\n"
        "        DR_Over = :( (DR_Over)[ 0 ], ipDR_Over )\n"
        "        if ((DR_Over) == (1)) {\n"
        "            if ((Trigger)[ 0 ]) {\n"
        "                opDR_Control =  ~ (opDR_Control)\n"
        "            } else {\n"
        "                opDR_Control = 0\n"
        "            }\n"
        "        } else {\n"
        "            if ((((DR_Over)[ 0 ]) == (1)) & ((Trigger) == (1))) {\n"
        "                opDR_Control = ( ~ (opDR_Control)) & ( ~ (opBusy))\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "class ADF4159(net Clk, net Reset, num Baud_kHz = 1000){\n"
        "    public {\n"
        "        net (4) CP_CurrentSetting = 7\n"
        "        net (12) Integer = 0\n"
        "        net (25) Fraction = 0\n"
        "        net RampOn = 0\n"
        "        net ( - (15)) DeviationWord_0 = 0\n"
        "        net (4) DeviationOffset_0 = 0\n"
        "        net (20) StepWord_0 = 0\n"
        "        net ( - (15)) DeviationWord_1 = 0\n"
        "        net (4) DeviationOffset_1 = 0\n"
        "        net (20) StepWord_1 = 0\n"
        "        net UseRefMul2 = 0\n"
        "        net (5) RefCounter = 1\n"
        "        net UseRefDiv2 = 1\n"
        "        net (12) Clk1Divider = 1\n"
        "        net (12) Clk2Divider_0 = 2\n"
        "        net (12) Clk2Divider_1 = 2\n"
        "        input {\n"
        "            net Update\n"
        "        }\n"
        "        output {\n"
        "            net Busy = 1\n"
        "        }\n"
        "        input {\n"
        "            net Trigger\n"
        "        }\n"
        "        input {\n"
        "            net (4) MuxOutControl = 15\n"
        "        }\n"
        "        input {\n"
        "            net PhaseAdjust = 0\n"
        "        }\n"
        "        input {\n"
        "            net (12) PhaseValue = 0\n"
        "        }\n"
        "        input {\n"
        "            net CSR_Enable = 0\n"
        "        }\n"
        "        input {\n"
        "            net Prescaler = 0\n"
        "        }\n"
        "        input {\n"
        "            net (3) NegBleedCurrent = 4\n"
        "        }\n"
        "        input {\n"
        "            net NegBleedEnable = 0\n"
        "        }\n"
        "        input {\n"
        "            net LossOfLock = 0\n"
        "        }\n"
        "        input {\n"
        "            net NSel = 0\n"
        "        }\n"
        "        input {\n"
        "            net SD_Reset = 0\n"
        "        }\n"
        "        input {\n"
        "            net (2) RampMode = 3\n"
        "        }\n"
        "        input {\n"
        "            net PSK = 0\n"
        "        }\n"
        "        input {\n"
        "            net FSK = 0\n"
        "        }\n"
        "        input {\n"
        "            net LDP = 1\n"
        "        }\n"
        "        input {\n"
        "            net PDPolarity = 0\n"
        "        }\n"
        "        input {\n"
        "            net PowerDown = 0\n"
        "        }\n"
        "        input {\n"
        "            net CP_ThreeState = 0\n"
        "        }\n"
        "        input {\n"
        "            net CounterReset = 0\n"
        "        }\n"
        "        input {\n"
        "            net LE_Select = 0\n"
        "        }\n"
        "        input {\n"
        "            net (5) SD_ModulatorMode = 0\n"
        "        }\n"
        "        input {\n"
        "            net (5) RampStatus = 3\n"
        "        }\n"
        "        input {\n"
        "            net (2) ClkDivMode = 3\n"
        "        }\n"
        "        input {\n"
        "            net TxDataInvert = 0\n"
        "        }\n"
        "        input {\n"
        "            net TxDataRampClk = 0\n"
        "        }\n"
        "        input {\n"
        "            net ParabolicRamp = 0\n"
        "        }\n"
        "        input {\n"
        "            net (2) Interrupt = 0\n"
        "        }\n"
        "        input {\n"
        "            net FSK_Ramp = 0\n"
        "        }\n"
        "        input {\n"
        "            net DualRamp = 0\n"
        "        }\n"
        "        input {\n"
        "            net TxDataTriggerDelay = 0\n"
        "        }\n"
        "        input {\n"
        "            net TriDelay = 0\n"
        "        }\n"
        "        input {\n"
        "            net SingleFullTriangle = 1\n"
        "        }\n"
        "        input {\n"
        "            net TxDataTrigger = 1\n"
        "        }\n"
        "        input {\n"
        "            net FastRamp = 1\n"
        "        }\n"
        "        input {\n"
        "            net RampDelayFastLock = 0\n"
        "        }\n"
        "        input {\n"
        "            net RampDelay = 0\n"
        "        }\n"
        "        input {\n"
        "            net DelClkSel = 0\n"
        "        }\n"
        "        input {\n"
        "            net DelStartEn = 0\n"
        "        }\n"
        "        input {\n"
        "            net (12) DelayStartWord = 0\n"
        "        }\n"
        "    }\n"
        "    private {\n"
        "        net SPI_Clk = 0\n"
        "        net SPI_Data\n"
        "        net SPI_LE = 1\n"
        "    }\n"
        "    void ConnectPins(pin SClk, pin Data, pin LE, pin TxData){\n"
        "        SClk = SPI_Clk\n"
        "        Data = SPI_Data\n"
        "        LE = SPI_LE\n"
        "        TxData = (Trigger) & (RampOn)\n"
        "    }\n"
        "    private {\n"
        "        num RefFreq = 0\n"
        "        num PfdFreq = 0\n"
        "        num StartFreq = 10000000000\n"
        "        bool FeedbackVcoDiv2 = 1\n"
        "        bool UseRefMul2 = 0\n"
        "        bool UseRefDiv2 = 0\n"
        "    }\n"
        "    void SetStart(num StartFreq){\n"
        "        assert {\n"
        "            (RefFreq) > (0)\n"
        "        }\n"
        "        (this) . (StartFreq) = StartFreq\n"
        "        num Frequency = (FeedbackVcoDiv2) ? (round(((16777216) * (StartFreq)) / (PfdFreq))) : (round(((33554432) * (StartFreq)) / (PfdFreq)))\n"
        "        Integer = (Frequency) >> (25)\n"
        "        Fraction = (Frequency) & (33554431)\n"
        "    }\n"
        "    private {\n"
        "        void SetRamp(bool Up, num Bandwidth, num Time){\n"
        "            assert {\n"
        "                (RefFreq) > (0)\n"
        "            }\n"
        "            Clk1Divider = 1\n"
        "            num Clk2Divider = 2\n"
        "            num StepWord = 0\n"
        "            num DeviationOffset = 0\n"
        "            num DeviationWord = 0\n"
        "            Clk2Divider = (PfdFreq) / (((1048576) - (1)) / (Time))\n"
        "            if ((Clk2Divider) < (2)) {\n"
        "                Clk2Divider = 2\n"
        "            }\n"
        "            assert {\n"
        "                (Clk2Divider) < (4096)\n"
        "            }\n"
        "            StepWord = round(((PfdFreq) / (Clk2Divider)) * (Time))\n"
        "            assert {\n"
        "                (StepWord) < (1048576)\n"
        "            }\n"
        "            DeviationOffset = 0\n"
        "            DeviationWord = round((((Bandwidth) / (StepWord)) / ((PfdFreq) / (33554432))) / ((2) ** (DeviationOffset)))\n"
        "            while ((abs(DeviationWord)) >= (32768)) {\n"
        "                DeviationOffset++\n"
        "                DeviationWord /= 2\n"
        "            }\n"
        "            assert {\n"
        "                (DeviationOffset) < (16)\n"
        "            }\n"
        "            if (Up) {\n"
        "                Clk2Divider_0 = Clk2Divider\n"
        "                StepWord_0 = StepWord\n"
        "                DeviationOffset_0 = DeviationOffset\n"
        "                DeviationWord_0 = DeviationWord\n"
        "            } else {\n"
        "                Clk2Divider_1 = Clk2Divider\n"
        "                StepWord_1 = StepWord\n"
        "                DeviationOffset_1 = DeviationOffset\n"
        "                DeviationWord_1 = DeviationWord\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    void SetRamp(num Bandwidth, num UpTime, num DownTime){\n"
        "        SetRamp(1, Bandwidth, UpTime)\n"
        "        SetRamp(0, Bandwidth, DownTime)\n"
        "    }\n"
        "    void SetRefFreq(num RefFreq, bool FeedbackVcoDiv2 = 1){\n"
        "        (this) . (RefFreq) = RefFreq\n"
        "        (this) . (FeedbackVcoDiv2) = FeedbackVcoDiv2\n"
        "        UseRefMul2 = 0\n"
        "        UseRefDiv2 = 0\n"
        "        PfdFreq = RefFreq\n"
        "        if ((RefFreq) > (110000000)) {\n"
        "            UseRefDiv2 = 1\n"
        "            PfdFreq /= 2\n"
        "        }\n"
        "        if ((RefFreq) < (55000000)) {\n"
        "            UseRefMul2 = 1\n"
        "            PfdFreq *= 2\n"
        "        }\n"
        "        num RefDiv = 1\n"
        "        while (((RefDiv) < (32)) & ((PfdFreq) > (110000000))) {\n"
        "            RefDiv++\n"
        "            PfdFreq /= 2\n"
        "        }\n"
        "        RefCounter = RefDiv\n"
        "        SetStart(StartFreq)\n"
        "    }\n"
        "    private {\n"
        "        net (32) R0 = :( RampOn, MuxOutControl, Integer, (Fraction)[ (24) .. (13) ], (0) @ (3) )\n"
        "        net (32) R1 = :( (0) @ (3), PhaseAdjust, (Fraction)[ (12) .. (0) ], PhaseValue, (1) @ (3) )\n"
        "        net (32) R2 = :( (0) @ (3), CSR_Enable, CP_CurrentSetting, (0) @ (1), Prescaler, UseRefDiv2, UseRefMul2, RefCounter, Clk1Divider, (2) @ (3) )\n"
        "        net (32) R3 = :( (0) @ (7), NegBleedCurrent, NegBleedEnable, (1) @ (4), LossOfLock, NSel, SD_Reset, (0) @ (2), RampMode, PSK, FSK, LDP, PDPolarity, PowerDown, CP_ThreeState, CounterReset, (3) @ (3) )\n"
        "        net (32) R4_0 = :( LE_Select, SD_ModulatorMode, RampStatus, ClkDivMode, Clk2Divider_0, (0) @ (1), (0) @ (3), (4) @ (3) )\n"
        "        net (32) R4_1 = :( LE_Select, SD_ModulatorMode, RampStatus, ClkDivMode, Clk2Divider_1, (1) @ (1), (0) @ (3), (4) @ (3) )\n"
        "        net (32) R5_0 = :( (0) @ (1), TxDataInvert, TxDataRampClk, ParabolicRamp, Interrupt, FSK_Ramp, DualRamp, (0) @ (1), DeviationOffset_0, DeviationWord_0, (5) @ (3) )\n"
        "        net (32) R5_1 = :( (0) @ (1), TxDataInvert, TxDataRampClk, ParabolicRamp, Interrupt, FSK_Ramp, DualRamp, (1) @ (1), DeviationOffset_1, DeviationWord_1, (5) @ (3) )\n"
        "        net (32) R6_0 = :( (0) @ (9), StepWord_0, (6) @ (3) )\n"
        "        net (32) R6_1 = :( (1) @ (9), StepWord_1, (6) @ (3) )\n"
        "        net (32) R7 = :( (0) @ (8), TxDataTriggerDelay, TriDelay, SingleFullTriangle, TxDataTrigger, FastRamp, RampDelayFastLock, RampDelay, DelClkSel, DelStartEn, DelayStartWord, (7) @ (3) )\n"
        "    }\n"
        "    private {\n"
        "        num Baud_Cycles = round(((Clk) ' (frequency)) / ((Baud_kHz) * (2)))\n"
        "        num Baud_N = ceil(log2(Baud_Cycles))\n"
        "        net (Baud_N) Baud_Count\n"
        "        rtl (Clk){\n"
        "            if (Baud_Count) {\n"
        "                Baud_Count--\n"
        "            } else {\n"
        "                Baud_Count = (Baud_Cycles) - (1)\n"
        "            }\n"
        "        }\n"
        "        net Baud_Trigger =  ~| (Baud_Count)\n"
        "    }\n"
        "    private {\n"
        "        void SendSPI(net (32) Register){\n"
        "            net (32) Data\n"
        "            SPI_Data = (Data)[ 31 ]\n"
        "            fsm {\n"
        "                Data = Register\n"
        "                SPI_LE = 0\n"
        "                loop (32) {\n"
        "                    SPI_Clk = 1\n"
        "                    SPI_Clk = 0\n"
        "                    Data <<= 1\n"
        "                }\n"
        "                SPI_LE = 1\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    fsm (Clk, Reset, Baud_Trigger){\n"
        "        loop {\n"
        "            Busy = 0\n"
        "            while ( ! (Update)) {\n"
        "                fence\n"
        "            }\n"
        "            fence\n"
        "            Busy = 1\n"
        "            SendSPI(R7)\n"
        "            SendSPI(R6_1)\n"
        "            SendSPI(R6_0)\n"
        "            SendSPI(R5_1)\n"
        "            SendSPI(R5_0)\n"
        "            SendSPI(R4_1)\n"
        "            SendSPI(R4_0)\n"
        "            SendSPI(R3)\n"
        "            SendSPI(R2)\n"
        "            SendSPI(R1)\n"
        "            SendSPI(R0)\n"
        "            while (Update) {\n"
        "                fence\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        " ' (target_vendor) = \"Altera\"",
        " ' (target_series) = \"Cyclone V\"",
        " ' (target_device) = \"5CSEMA4U23C6N\"",
        " ' (target_board) = \"DE0-Nano-SoC rev C1\"",
        " ' (standard) = \"3.3V\"",
        "group <frequency = 50000000> {\n"
        "    input {\n"
        "        pin <location = \"V11\", clock_group = \"ClockGroup1\"> Clock1\n"
        "    }\n"
        "    input {\n"
        "        pin <location = \"Y13\", clock_group = \"ClockGroup2\"> Clock2\n"
        "    }\n"
        "    input {\n"
        "        pin <location = \"E11\", clock_group = \"ClockGroup1\"> Clock3\n"
        "    }\n"
        "}",

        "input {\n"
        "    pin (4) <location = [ \"H5\", \"H6\", \"L9\", \"L10\" ]> Switches\n"
        "}",

        "input {\n"
        "    pin (2) <location = [ \"AH16\", \"AH17\" ]> Keys\n"
        "}",

        "output {\n"
        "    pin (8) <location = [ \"AA23\", \"Y16\", \"AE26\", \"AF26\", \"V15\", \"V16\", \"AA24\", \"W15\" ]> LEDs\n"
        "}",

        "pin (36) <location = [ [ \"AE12\", \"AF11\", \"AE11\", \"AD12\", \"AF10\", \"AD11\", \"AE9\", \"AD10\", \"AE8\", \"AF9\", \"AF6\", \"AE7\", \"T11\", \"T13\", \"AE4\", \"AF5\", \"AG6\", \"AF4\", \"AH2\", \"AH3\", \"AG5\", \"AH4\", \"AH6\", \"AH5\", \"T12\", \"T8\", \"U11\", \"Y5\", \"Y4\", \"W8\", \"AB4\", \"Y8\", \"AF8\", \"W12\", \"AF7\", \"V12\" ], [ \"AC22\", \"AA18\", \"AE23\", \"AD23\", \"AH18\", \"AG21\", \"AH21\", \"AH19\", \"AH22\", \"AF22\", \"AD20\", \"AE24\", \"AE20\", \"AD19\", \"AF18\", \"AE19\", \"AG23\", \"AH23\", \"AF25\", \"AG24\", \"AA19\", \"AH26\", \"AG18\", \"AC23\", \"AF20\", \"AG19\", \"AG20\", \"AF21\", \"AE22\", \"AF23\", \"AH24\", \"AG26\", \"AH27\", \"AA15\", \"AG28\", \"Y15\" ] ]> GPIO[2]",
        "group Arduino {\n"
        "    input {\n"
        "        pin <location = \"AH7\"> nReset\n"
        "    }\n"
        "    pin (16) <location = [ \"AG11\", \"AH9\", \"AH12\", \"AH11\", \"AG16\", \"AF15\", \"AE15\", \"AF17\", \"AH8\", \"AG8\", \"U13\", \"U14\", \"AG9\", \"AG10\", \"AF13\", \"AG13\" ]> IO\n"
        "}",

        "group ADC {\n"
        "    output {\n"
        "        pin <location = \"U9\"> ConvStart\n"
        "    }\n"
        "    output {\n"
        "        pin <location = \"V10\"> Clock\n"
        "    }\n"
        "    output {\n"
        "        pin <location = \"AC4\"> DataIn\n"
        "    }\n"
        "    input {\n"
        "        pin <location = \"AD4\"> DataOut\n"
        "    }\n"
        "}",

        "group HPS {\n"
        "    group <frequency = 25000000> {\n"
        "        input {\n"
        "            pin <location = \"E20\"> Clock1\n"
        "        }\n"
        "        input {\n"
        "            pin <location = \"D20\"> Clock2\n"
        "        }\n"
        "    }\n"
        "    input {\n"
        "        pin <location = \"J18\", hps_gpio = \"GPIO54\", register_bit = 25> Key\n"
        "    }\n"
        "    output {\n"
        "        pin <location = \"A20\", hps_gpio = \"GPIO53\", register_bit = 24> LED\n"
        "    }\n"
        "    group Ethernet {\n"
        "        group Tx {\n"
        "            output {\n"
        "                pin <location = \"J15\"> Clock\n"
        "            }\n"
        "            output {\n"
        "                pin (4) <location = [ \"D17\", \"A15\", \"J14\", \"A16\" ]> Data\n"
        "            }\n"
        "            output {\n"
        "                pin <location = \"A12\"> Enable\n"
        "            }\n"
        "        }\n"
        "        group Rx {\n"
        "            input {\n"
        "                pin <location = \"J12\"> Clock\n"
        "            }\n"
        "            input {\n"
        "                pin (4) <location = [ \"A9\", \"C15\", \"A11\", \"A14\" ]> Data\n"
        "            }\n"
        "            input {\n"
        "                pin <location = \"J13\"> Valid\n"
        "            }\n"
        "        }\n"
        "        output {\n"
        "            pin <location = \"B14\"> nReset\n"
        "        }\n"
        "        pin <location = \"E16\"> MDIO\n"
        "        output {\n"
        "            pin <location = \"A13\"> MDC\n"
        "        }\n"
        "        input {\n"
        "            pin <location = \"B14\"> nInterrupt\n"
        "        }\n"
        "    }\n"
        "    group UART {\n"
        "        input {\n"
        "            pin <location = \"A22\"> Rx\n"
        "        }\n"
        "        output {\n"
        "            pin <location = \"B21\"> Tx\n"
        "        }\n"
        "        input {\n"
        "            pin <location = \"C6\"> nConvUSB\n"
        "        }\n"
        "    }\n"
        "    group <standard = \"SSTL-15 Class I\"> DDR3 {\n"
        "        group <output_termination = \"SERIES 50 OHM WITH CALIBRATION\"> {\n"
        "            group <input_termination = \"PARALLEL 50 OHM WITH CALIBRATION\"> {\n"
        "                pin (32) <location = [ \"AA27\", \"Y27\", \"T24\", \"R24\", \"W26\", \"AA28\", \"R25\", \"R26\", \"V27\", \"R27\", \"N27\", \"N26\", \"U28\", \"T28\", \"N25\", \"N24\", \"N28\", \"M28\", \"M26\", \"M27\", \"J28\", \"J27\", \"L25\", \"K25\", \"F28\", \"G27\", \"K26\", \"J26\", \"D27\", \"E28\", \"J24\", \"J25\" ]> Data\n"
        "                pin (4) <location = [ \"U19-T20\", \"T19-T18\", \"R19-R18\", \"R17-R16\" ], standard = \"Differential 1.5-V SSTL Class I\"> DataStrobe\n"
        "            }\n"
        "            pin (4) <location = [ \"AB28\", \"W28\", \"P28\", \"G28\" ]> DataMask\n"
        "        }\n"
        "        group <output_termination = \"SERIES 50 OHM WITHOUT CALIBRATION\"> {\n"
        "            pin (15) <location = [ \"G23\", \"C24\", \"D24\", \"B24\", \"A24\", \"F25\", \"F26\", \"B26\", \"C26\", \"J20\", \"J21\", \"D26\", \"E26\", \"B28\", \"C28\" ]> Address\n"
        "            pin (3) <location = [ \"G25\", \"H25\", \"A27\" ]> Bank\n"
        "            pin <location = \"N21-N20\", standard = \"Differential 1.5-V SSTL Class I\"> Clock\n"
        "            pin <location = \"A26\"> nColumnAddressStrobe\n"
        "            pin <location = \"L28\"> ClockEnable\n"
        "            pin <location = \"L21\"> nChipSelect\n"
        "            pin <location = \"D28\"> OnDieTermination\n"
        "            pin <location = \"A25\"> nRowAddressStrobe\n"
        "            pin <location = \"V28\"> nReset\n"
        "            pin <location = \"E25\"> nWriteEnable\n"
        "        }\n"
        "        pin <location = \"D25\"> RZQ\n"
        "    }\n"
        "    group SD {\n"
        "        output {\n"
        "            pin <location = \"B8\"> Clock\n"
        "        }\n"
        "        output {\n"
        "            pin <location = \"D14\"> Command\n"
        "        }\n"
        "        pin (4) <location = [ \"B9\", \"B11\", \"B6\", \"C13\" ]> Data\n"
        "    }\n"
        "    group USB {\n"
        "        input {\n"
        "            pin <location = \"G4\", frequency = 60000000> ClockOut\n"
        "        }\n"
        "        pin <location = \"H12\"> Reset\n"
        "        pin (8) <location = [ \"F4\", \"C7\", \"D4\", \"C8\", \"C4\", \"C9\", \"F5\", \"C10\" ]> Data\n"
        "        input {\n"
        "            pin <location = \"E5\"> Direction\n"
        "        }\n"
        "        input {\n"
        "            pin <location = \"D5\"> Next\n"
        "        }\n"
        "        output {\n"
        "            pin <location = \"C5\"> Step\n"
        "        }\n"
        "    }\n"
        "    group I2C {\n"
        "        pin <location = \"C18\"> Clock\n"
        "        pin <location = \"A19\"> Data\n"
        "    }\n"
        "    group <i2c_address = [ 166, 167 ], i2c_bus = I2C> G_Sensor {\n"
        "        input {\n"
        "            pin <location = \"A17\"> Interrupt\n"
        "        }\n"
        "    }\n"
        "    group LTC {\n"
        "        pin <location = \"H13\"> GPIO\n"
        "        group I2C {\n"
        "            pin <location = \"B21\"> Clock\n"
        "            pin <location = \"A21\"> Data\n"
        "        }\n"
        "        group SPI {\n"
        "            output {\n"
        "                pin <location = \"C19\"> Clock\n"
        "            }\n"
        "            input {\n"
        "                pin <location = \"B19\"> MISO\n"
        "            }\n"
        "            output {\n"
        "                pin <location = \"B16\"> MOSI\n"
        "            }\n"
        "            output {\n"
        "                pin <location = \"C16\"> SlaveSelect\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}",

        "class TriggerDelay(net Clock, net Reset, net Trigger){\n"
        "    input {\n"
        "        net Enable = 1\n"
        "    }\n"
        "    input {\n"
        "        net (32) Delay = (1/1000000 (~1e-06)) * ((Clock) ' (frequency))\n"
        "    }\n"
        "    input {\n"
        "        net (32) Length = 1\n"
        "    }\n"
        "    output {\n"
        "        net Output = 0\n"
        "    }\n"
        "    private {\n"
        "        net (32) DelayCount = 0\n"
        "        net (32) LengthCount = 0\n"
        "        net Trigger_1\n"
        "    }\n"
        "    rtl (Clock, Reset){\n"
        "        Trigger_1 = Trigger\n"
        "        if ((:( Trigger_1, Trigger )) == (1)) {\n"
        "            DelayCount = Delay\n"
        "        } else {\n"
        "            if ((DelayCount) > (0)) {\n"
        "                DelayCount--\n"
        "            }\n"
        "        }\n"
        "        if (Output) {\n"
        "            if ((LengthCount) == (1)) {\n"
        "                Output = 0\n"
        "            }\n"
        "            LengthCount--\n"
        "        } else {\n"
        "            if ((DelayCount) == (1)) {\n"
        "                Output = Enable\n"
        "                LengthCount = Length\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    AddDebug(Output)\n"
        "}",

        "class TriggerGen(net Clock, net Reset){\n"
        "    net (32) Period = (1/1000 (~0.001)) * ((Clock) ' (frequency))\n"
        "    net Output = 0\n"
        "    net (32) Count = 0\n"
        "    rtl (Clock, Reset){\n"
        "        if ((Count) >= (Period)) {\n"
        "            Count = 1\n"
        "            Output = 1\n"
        "        } else {\n"
        "            Count++\n"
        "            Output = 0\n"
        "        }\n"
        "    }\n"
        "    AddDebug(Ouptut)\n"
        "}",

        "num DebugCounter = 0",
        "void AddDebug(net Signal){\n"
        "    (Debug)[ DebugCounter ] = Signal\n"
        "    print(:[ \"Debug[\", $(DebugCounter), \"] <= \", (Signal) ' (full_instance_name), \"\n" "\" ])\n"
        "    DebugCounter++\n"
        "}",

        "import \"Platform/Platform\"",
        "import \"SubSystems/SystemController\"",
        "import \"Library/Interfaces/RegistersDecoder\"",
        "RegistersDecoder ((SystemController) . (LightWeightBus)) Registers",
        "(Registers) . (CppFilename) = \"../Software/Registers/Registers\"",
        "(Registers) . (LaTeXFilename) = \"../Documentation/Registers\"",
        "import \"Library/Misc/FirmwareVersion\"",
        "FirmwareVersion (1, 0) Firmware",
        "(Registers) . (ReadOnly)((Firmware) . (Version))",
        "(Registers) . (ReadOnly)((Firmware) . (Date))",
        "(Registers) . (ReadOnly)((Firmware) . (Time))",
        "(Registers) . (ReadOnly)((Firmware) . (GitHash))",
        "import \"SubSystems/MasterTrigger\"",
        "import \"SubSystems/HardwareControl\" as Hardware",
        "import \"SubSystems/WaveformGenerator\" as Waveform",
        "import \"SubSystems/ReceiverAbstraction\" as Receiver",
        "import \"SubSystems/RadarProcessor\" as Processor",
        "import \"Library/Platforms/DE0-Nano-SoC\" as DE0",
        "alias HPS_Pins = (DE0) . (HPS)",
        "alias LEDs = (DE0) . (LEDs)",
        "alias DipSwitch = (DE0) . (Switches)",
        "group Synth {\n"
        "    group SPI {\n"
        "        alias SClk = (((DE0) . (Arduino)) . (IO))[ 0 ]\n"
        "        alias nCS = (((DE0) . (Arduino)) . (IO))[ 1 ]\n"
        "        alias SDIO = (((DE0) . (Arduino)) . (IO))[ 2 ]\n"
        "        alias SyncIO = (((DE0) . (Arduino)) . (IO))[ 3 ]\n"
        "        alias IO_Update = (((DE0) . (Arduino)) . (IO))[ 4 ]\n"
        "    }\n"
        "    alias DR_Control = (((DE0) . (Arduino)) . (IO))[ 9 ]\n"
        "    alias DR_Hold = (((DE0) . (Arduino)) . (IO))[ 10 ]\n"
        "    alias DR_Over = (((DE0) . (Arduino)) . (IO))[ 11 ]\n"
        "}",

        "group ADC {\n"
        "    alias nCS = (((DE0) . (Arduino)) . (IO))[ 5 ]\n"
        "    alias SClk = (((DE0) . (Arduino)) . (IO))[ 6 ]\n"
        "    alias Data = (((DE0) . (Arduino)) . (IO))[ 7 ]\n"
        "    (SClk) ' (frequency) = 60000000\n"
        "    ((nCS) ' (set_output_delay)) ' (clock) = SClk\n"
        "    ((nCS) ' (set_output_delay)) ' (delay) = 0\n"
        "    ((nCS) ' (set_output_delay)) ' (uncertainty) = 1/20000000000 (~5e-11)\n"
        "    ((Data) ' (set_input_delay)) ' (clock) = SClk\n"
        "    ((Data) ' (set_input_delay)) ' (delay) = (1/10 (~0.1)) / (200000000)\n"
        "    ((Data) ' (set_input_delay)) ' (uncertainty) = 1/20000000000 (~5e-11)\n"
        "}",

        "alias TxEnable = (((DE0) . (Arduino)) . (IO))[ 8 ]",
        "net (36) Debug",
        "((DE0) . (GPIO))[ 0 ] = Debug",
        "group I2C {\n"
        "    alias SClk = (((DE0) . (Arduino)) . (IO))[ 15 ]\n"
        "    alias Data = (((DE0) . (Arduino)) . (IO))[ 14 ]\n"
        "    (SClk) ' (pullup) = 1\n"
        "    (SClk) . (driver) = 0\n"
        "    (SClk) . (enable) = 0\n"
        "    (Data) ' (pullup) = 1\n"
        "    (Data) . (driver) = 0\n"
        "    (Data) . (enable) = 0\n"
        "}",

        "net ResetKey =  ~ (((DE0) . (Keys))[ 0 ])",
        "import \"Library/Clocking_and_Reset/PLL_CycloneV\"",
        "PLL_CycloneV ((DE0) . (Clock1), ResetKey, [ 120, 5/2 (~2.5) ]) PLL",
        "alias DspClock = ((PLL) . (Output))[ 0 ]",
        "alias ControlClock = ((PLL) . (Output))[ 1 ]",
        "import \"Library/Clocking_and_Reset/DelayedReset\"",
        "net HPS_Reset = DelayedReset(ControlClock, (ResetKey) | ( ~ ((PLL) . (Locked))), 1)",
        "net HPS_FPGA_Reset = 0",
        "net MasterReset = DelayedReset(ControlClock, ((HPS_FPGA_Reset) | (ResetKey)) | ( ~ ((PLL) . (Locked))), 100)",
        "alias Buffer = (SystemController) . (StreamBuffer)",
        "net (((Buffer) . (Address)) ' (width)) WrAddress = 0",
        "net (((Buffer) . (Address)) ' (width)) RdAddress = 0",
        "(Registers) . (ReadOnly)(WrAddress)",
        "(Registers) . (Writeable)(RdAddress)",
        "(Buffer) . (Address) = 0",

        "fsm (DspClock, MasterReset){\n"
        "    loop {\n"
        "        loop {\n"
        "            if ((Buffer) . (Write)) {\n"
        "                (Buffer) . (Address)++\n"
        "            } else {\n"
        "                WrAddress = (Buffer) . (Address)\n"
        "            }\n"
        "            (Buffer) . (WriteData) := :( (Packet) . (Data), (0) @ (2) )\n"
        "            if (((Packet) . (Valid)) & ((Packet) . (SoP))) {\n"
        "                (Buffer) . (Write) = 1\n"
        "                break\n"
        "            } else {\n"
        "                (Buffer) . (Write) = 0\n"
        "            }\n"
        "        }\n"
        "        loop {\n"
        "            (Buffer) . (WriteData) := :( (Packet) . (Data), (0) @ (2) )\n"
        "            (Buffer) . (Write) = (Packet) . (Valid)\n"
        "            if ((Packet) . (Valid)) {\n"
        "                (Buffer) . (Address)++\n"
        "                if ((Packet) . (EoP)) {\n"
        "                    break\n"
        "                }\n"
        "            }\n"
        "            fence\n"
        "        }\n"
        "    }\n"
        "}",

        "import \"Library/Misc/MutEx\"",
        "import \"Library/Timing/TriggerDelay\"",
        "import \"Library/Peripherals/ADC_and_BIM/LTC2991\"",
        "MutEx (ControlClock, MasterReset) I2C_MutEx",
        "LTC2991 (ControlClock, MasterReset, 0) TxBIM",
        "(TxBIM) . (SetType)(0, ((LTC2991) . (TYPE)) . (Voltage))",
        "(TxBIM) . (SetType)(1, ((LTC2991) . (TYPE)) . (Voltage))",
        "(TxBIM) . (SetType)(2, ((LTC2991) . (TYPE)) . (Voltage))",
        "(TxBIM) . (SetType)(3, ((LTC2991) . (TYPE)) . (Temperature))",
        "(TxBIM) . (Quiet) =  ! (TxEnable)",
        "(TxBIM) . (MakeRegisters)(Registers)",
        "(TxBIM) . (ConnectBus)((I2C) . (SClk), (I2C) . (Data))",
        "(I2C_MutEx) . (Add)(TxBIM)",
        "LTC2991 (ControlClock, MasterReset, 1) RxBIM",
        "(RxBIM) . (SetType)(0, ((LTC2991) . (TYPE)) . (Voltage))",
        "(RxBIM) . (SetType)(1, ((LTC2991) . (TYPE)) . (Differential))",
        "(RxBIM) . (SetType)(2, ((LTC2991) . (TYPE)) . (Voltage))",
        "(RxBIM) . (SetType)(3, ((LTC2991) . (TYPE)) . (Temperature))",
        "(RxBIM) . (Quiet) =  ! (TxEnable)",
        "(RxBIM) . (MakeRegisters)(Registers)",
        "(RxBIM) . (ConnectBus)((I2C) . (SClk), (I2C) . (Data))",
        "(I2C_MutEx) . (Add)(RxBIM)",
        "TriggerDelay (ControlClock, MasterReset, MasterTrigger) TriggerDelay_PA",
        "(Registers) . (Writeable)((TriggerDelay_PA) . (Enable))",
        "(Registers) . (Writeable)((TriggerDelay_PA) . (Delay))",
        "(Registers) . (Writeable)((TriggerDelay_PA) . (Length))",
        "TxEnable = (TriggerDelay_PA) . (Output)",
        "import \"Library/Timing/TriggerGen\"",
        "TriggerGen (ControlClock, MasterTrigger) MasterTriggerGen",
        "(Registers) . (Writable)((MasterTriggerGen) . (Period))",
        "alias MasterTrigger = (MasterTriggerGen) . (Output)",
        "import \"Library/DSP/Streams\"",
        "import \"Library/DSP/FIFO\"",
        "import \"Library/DSP/Window\"",
        "import \"Library/DSP/FFT\"",
        "import \"Library/DSP/Transpose\"",
        "import \"Library/DSP/AlphaFilter\"",
        "FIFO (DspClock, MasterReset, 8192) Queue",
        "Window (DspClock, MasterReset, 2500, 16, 32, \"Hann\") RangeWindow",
        "FFT (DspClock, MasterReset, 4096) RangeFFT",
        "Transpose (DspClock, MasterReset, 2048, 128) CornerTurn",
        "Window (DspClock, MasterReset, 128, 32, 32, \"Hann\") DopplerWindow",
        "FFT (DspClock, MasterReset, 128) DopplerFFT",
        "AlphaFilter (DspClock, MasterReset, 128, 2048) Filter",
        "(Registers) . (ReadOnly)((Queue) . (NumItems))",
        "(Registers) . (Writeable)((Filter) . (Alpha))",
        "(Registers) . (ReadOnly)((Filter) . (WrAddress))",
        "DSP = ((((((((Receiver) . (Packet)) | (Queue)) | (RangeWindow)) | (RangeFFT)) | (CornerTurn)) | (DopplerWindow)) | (DopplerFFT)) | (Filter)",
        "import \"Library/Timing/TriggerDelay\"",
        "import \"Library/Peripherals/ADC_and_BIM/ADS7056\"",
        "ADS7056 (DspClock, MasterReset) ADC",
        "TriggerDelay (ControlClock, MasterReset, MasterTrigger) PacketTrigger",
        "(Registers) . (Writeable)((PacketTrigger) . (Enable))",
        "(Registers) . (Writeable)((PacketTrigger) . (Delay))",
        "(Registers) . (Writeable)((PacketTrigger) . (Length))",
        "net PacketTrigger_DSP",
        "rtl (DspClock){\n"
        "    PacketTrigger_DSP = (PacketTrigger) . (Output)\n"
        "}",

        "group Packet {\n"
        "    net SoP\n"
        "    net EoP\n"
        "    net (13,  - (1)) Data\n"
        "    net Valid = 0\n"
        "}",

        "AddDebug((Packet) . (SoP))",
        "AddDebug((Packet) . (EoP))",
        "AddDebug((Packet) . (Valid))",
        "private {\n"
        "    net (12) n\n"
        "}",

        "private {\n"
        "    num N = 2500\n"
        "}",

        "fsm (DspClock, MasterReset){\n"
        "    loop {\n"
        "        n = 0\n"
        "        (Packet) . (Valid) = 0\n"
        "        while ( ! (PacketTrigger_DSP)) {\n"
        "            fence\n"
        "        }\n"
        "        fence\n"
        "        while ((n) < (N)) {\n"
        "            (Packet) . (SoP) = (n) == (0)\n"
        "            (Packet) . (EoP) = (n) == ((N) - (1))\n"
        "            (Packet) . (Data) := :(  ~ (((ADC) . (Output)) . (Data)(13)), ((ADC) . (Output)) . (Data)((12) .. (0)) )\n"
        "            (Packet) . (Valid) = ((ADC) . (Output)) . (Valid)\n"
        "            if (((ADC) . (Output)) . (Valid)) {\n"
        "                n++\n"
        "            }\n"
        "            fence\n"
        "        }\n"
        "    }\n"
        "}",

        "import \"DebugStreamer\" as DebugStreamer",
        "import \"Library/Interfaces/AvalonMaster\"",
        "import \"Library/Interfaces/AvalonSlave\"",

        "class SystemController_Class{\n"
        "    private {\n"
        "        hdl SoC_System_HDL(\n"
        "        ){\n"
        "            output {\n"
        "                net hps_clk_out_clk\n"
        "            }\n"
        "            output {\n"
        "                net hps_reset_out_reset\n"
        "            }\n"
        "            input {\n"
        "                net hps_cold_reset_req_reset_n\n"
        "            }\n"
        "            input {\n"
        "                net hps_warm_reset_req_reset_n\n"
        "            }\n"
        "            input {\n"
        "                net hps_debug_reset_req_reset_n\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TX_CLK\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TXD0\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TXD1\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TXD2\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TXD3\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RXD0\n"
        "            }\n"
        "            net hps_io_hps_io_emac1_inst_MDIO\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_MDC\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RX_CTL\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_emac1_inst_TX_CTL\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RX_CLK\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RXD1\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RXD2\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_emac1_inst_RXD3\n"
        "            }\n"
        "            net hps_io_hps_io_sdio_inst_CMD\n"
        "            net hps_io_hps_io_sdio_inst_D0\n"
        "            net hps_io_hps_io_sdio_inst_D1\n"
        "            output {\n"
        "                net hps_io_hps_io_sdio_inst_CLK\n"
        "            }\n"
        "            net hps_io_hps_io_sdio_inst_D2\n"
        "            net hps_io_hps_io_sdio_inst_D3\n"
        "            net hps_io_hps_io_usb1_inst_D0\n"
        "            net hps_io_hps_io_usb1_inst_D1\n"
        "            net hps_io_hps_io_usb1_inst_D2\n"
        "            net hps_io_hps_io_usb1_inst_D3\n"
        "            net hps_io_hps_io_usb1_inst_D4\n"
        "            net hps_io_hps_io_usb1_inst_D5\n"
        "            net hps_io_hps_io_usb1_inst_D6\n"
        "            net hps_io_hps_io_usb1_inst_D7\n"
        "            input {\n"
        "                net hps_io_hps_io_usb1_inst_CLK\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_usb1_inst_STP\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_usb1_inst_DIR\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_usb1_inst_NXT\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_spim1_inst_CLK\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_spim1_inst_MOSI\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_spim1_inst_MISO\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_spim1_inst_SS0\n"
        "            }\n"
        "            input {\n"
        "                net hps_io_hps_io_uart0_inst_RX\n"
        "            }\n"
        "            output {\n"
        "                net hps_io_hps_io_uart0_inst_TX\n"
        "            }\n"
        "            net hps_io_hps_io_i2c0_inst_SDA\n"
        "            net hps_io_hps_io_i2c0_inst_SCL\n"
        "            net hps_io_hps_io_i2c1_inst_SDA\n"
        "            net hps_io_hps_io_i2c1_inst_SCL\n"
        "            net hps_io_hps_io_gpio_inst_GPIO09\n"
        "            net hps_io_hps_io_gpio_inst_GPIO35\n"
        "            net hps_io_hps_io_gpio_inst_GPIO40\n"
        "            net hps_io_hps_io_gpio_inst_GPIO53\n"
        "            net hps_io_hps_io_gpio_inst_GPIO54\n"
        "            net hps_io_hps_io_gpio_inst_GPIO61\n"
        "            output {\n"
        "                net (15) memory_mem_a\n"
        "            }\n"
        "            output {\n"
        "                net (3) memory_mem_ba\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_ck\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_ck_n\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_cke\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_cs_n\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_ras_n\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_cas_n\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_we_n\n"
        "            }\n"
        "            output {\n"
        "                net memory_mem_reset_n\n"
        "            }\n"
        "            net (32) memory_mem_dq\n"
        "            net (4) memory_mem_dqs\n"
        "            net (4) memory_mem_dqs_n\n"
        "            output {\n"
        "                net memory_mem_odt\n"
        "            }\n"
        "            output {\n"
        "                net (4) memory_mem_dm\n"
        "            }\n"
        "            input {\n"
        "                net memory_oct_rzqin\n"
        "            }\n"
        "            input {\n"
        "                net light_weight_bus_clk_clk\n"
        "            }\n"
        "            input {\n"
        "                net light_weight_bus_reset_reset\n"
        "            }\n"
        "            output {\n"
        "                net (12) light_weight_bus_address\n"
        "            }\n"
        "            output {\n"
        "                net (4) light_weight_bus_byteenable\n"
        "            }\n"
        "            output {\n"
        "                net (1) light_weight_bus_burstcount\n"
        "            }\n"
        "            input {\n"
        "                net light_weight_bus_waitrequest\n"
        "            }\n"
        "            output {\n"
        "                net (32) light_weight_bus_writedata\n"
        "            }\n"
        "            output {\n"
        "                net light_weight_bus_write\n"
        "            }\n"
        "            output {\n"
        "                net light_weight_bus_read\n"
        "            }\n"
        "            input {\n"
        "                net (32) light_weight_bus_readdata\n"
        "            }\n"
        "            input {\n"
        "                net light_weight_bus_readdatavalid\n"
        "            }\n"
        "            output {\n"
        "                net light_weight_bus_debugaccess\n"
        "            }\n"
        "            input {\n"
        "                net (27) sdram_address\n"
        "            }\n"
        "            input {\n"
        "                net (32) sdram_byteenable\n"
        "            }\n"
        "            input {\n"
        "                net (8) sdram_burstcount\n"
        "            }\n"
        "            output {\n"
        "                net sdram_waitrequest\n"
        "            }\n"
        "            input {\n"
        "                net (256) sdram_writedata\n"
        "            }\n"
        "            input {\n"
        "                net sdram_write\n"
        "            }\n"
        "            input {\n"
        "                net sdram_read\n"
        "            }\n"
        "            output {\n"
        "                net (256) sdram_readdata\n"
        "            }\n"
        "            output {\n"
        "                net sdram_readdatavalid\n"
        "            }\n"
        "            input {\n"
        "                net streambuffer_clk_clk\n"
        "            }\n"
        "            input {\n"
        "                net streambuffer_clken\n"
        "            }\n"
        "            input {\n"
        "                net streambuffer_reset_reset\n"
        "            }\n"
        "            input {\n"
        "                net streambuffer_chipselect\n"
        "            }\n"
        "            input {\n"
        "                net (13) streambuffer_address\n"
        "            }\n"
        "            input {\n"
        "                net (2) streambuffer_byteenable\n"
        "            }\n"
        "            input {\n"
        "                net (16) streambuffer_writedata\n"
        "            }\n"
        "            input {\n"
        "                net streambuffer_write\n"
        "            }\n"
        "            output {\n"
        "                net (16) streambuffer_readdata\n"
        "            }\n"
        "        }\n"
        "        SoC_System_HDL SoC_System\n"
        "    }\n"
        "    private {\n"
        "        ((((HPS_Pins) . (Ethernet)) . (Tx)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TX_CLK))\n"
        "        ((((HPS_Pins) . (Ethernet)) . (Tx)) . (Enable)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TX_CTL))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Tx)) . (Data))[ 0 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TXD0))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Tx)) . (Data))[ 1 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TXD1))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Tx)) . (Data))[ 2 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TXD2))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Tx)) . (Data))[ 3 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_TXD3))\n"
        "        ((((HPS_Pins) . (Ethernet)) . (Rx)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RX_CLK))\n"
        "        ((((HPS_Pins) . (Ethernet)) . (Rx)) . (Valid)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RX_CTL))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Rx)) . (Data))[ 0 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RXD0))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Rx)) . (Data))[ 1 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RXD1))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Rx)) . (Data))[ 2 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RXD2))\n"
        "        (((((HPS_Pins) . (Ethernet)) . (Rx)) . (Data))[ 3 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_RXD3))\n"
        "        (((HPS_Pins) . (Ethernet)) . (MDC)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_MDC))\n"
        "        (((HPS_Pins) . (Ethernet)) . (MDIO)) . (hdl_map)((SoC_System) . (hps_io_hps_io_emac1_inst_MDIO))\n"
        "        (((HPS_Pins) . (Ethernet)) . (nInterrupt)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO35))\n"
        "        (((HPS_Pins) . (SD)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_CLK))\n"
        "        (((HPS_Pins) . (SD)) . (Command)) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_CMD))\n"
        "        ((((HPS_Pins) . (SD)) . (Data))[ 0 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_D0))\n"
        "        ((((HPS_Pins) . (SD)) . (Data))[ 1 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_D1))\n"
        "        ((((HPS_Pins) . (SD)) . (Data))[ 2 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_D2))\n"
        "        ((((HPS_Pins) . (SD)) . (Data))[ 3 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_sdio_inst_D3))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 0 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D0))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 1 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D1))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 2 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D2))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 3 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D3))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 4 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D4))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 5 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D5))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 6 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D6))\n"
        "        ((((HPS_Pins) . (USB)) . (Data))[ 7 ]) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_D7))\n"
        "        (((HPS_Pins) . (USB)) . (ClockOut)) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_CLK))\n"
        "        (((HPS_Pins) . (USB)) . (Step)) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_STP))\n"
        "        (((HPS_Pins) . (USB)) . (Direction)) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_DIR))\n"
        "        (((HPS_Pins) . (USB)) . (Next)) . (hdl_map)((SoC_System) . (hps_io_hps_io_usb1_inst_NXT))\n"
        "        (((HPS_Pins) . (LTC)) . (GPIO)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO40))\n"
        "        ((((HPS_Pins) . (LTC)) . (I2C)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_i2c1_inst_SCL))\n"
        "        ((((HPS_Pins) . (LTC)) . (I2C)) . (Data)) . (hdl_map)((SoC_System) . (hps_io_hps_io_i2c1_inst_SDA))\n"
        "        ((((HPS_Pins) . (LTC)) . (SPI)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_spim1_inst_CLK))\n"
        "        ((((HPS_Pins) . (LTC)) . (SPI)) . (MOSI)) . (hdl_map)((SoC_System) . (hps_io_hps_io_spim1_inst_MOSI))\n"
        "        ((((HPS_Pins) . (LTC)) . (SPI)) . (MISO)) . (hdl_map)((SoC_System) . (hps_io_hps_io_spim1_inst_MISO))\n"
        "        ((((HPS_Pins) . (LTC)) . (SPI)) . (SlaveSelect)) . (hdl_map)((SoC_System) . (hps_io_hps_io_spim1_inst_SS0))\n"
        "        (((HPS_Pins) . (UART)) . (Rx)) . (hdl_map)((SoC_System) . (hps_io_hps_io_uart0_inst_RX))\n"
        "        (((HPS_Pins) . (UART)) . (Tx)) . (hdl_map)((SoC_System) . (hps_io_hps_io_uart0_inst_TX))\n"
        "        (((HPS_Pins) . (UART)) . (nConvUSB)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO09))\n"
        "        (((HPS_Pins) . (I2C)) . (Clock)) . (hdl_map)((SoC_System) . (hps_io_hps_io_i2c0_inst_SCL))\n"
        "        (((HPS_Pins) . (I2C)) . (Data)) . (hdl_map)((SoC_System) . (hps_io_hps_io_i2c0_inst_SDA))\n"
        "        ((HPS_Pins) . (LED)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO53))\n"
        "        ((HPS_Pins) . (Key)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO54))\n"
        "        (((HPS_Pins) . (G_Sensor)) . (Interrupt)) . (hdl_map)((SoC_System) . (hps_io_hps_io_gpio_inst_GPIO61))\n"
        "        (((HPS_Pins) . (DDR3)) . (nReset)) . (hdl_map)((SoC_System) . (memory_mem_reset_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (Clock)) . (hdl_map)((SoC_System) . (memory_mem_ck), (SoC_System) . (memory_mem_ck_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (ClockEnable)) . (hdl_map)((SoC_System) . (memory_mem_cke))\n"
        "        (((HPS_Pins) . (DDR3)) . (nChipSelect)) . (hdl_map)((SoC_System) . (memory_mem_cs_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (Bank)) . (hdl_map)((SoC_System) . (memory_mem_ba))\n"
        "        (((HPS_Pins) . (DDR3)) . (Address)) . (hdl_map)((SoC_System) . (memory_mem_a))\n"
        "        (((HPS_Pins) . (DDR3)) . (DataMask)) . (hdl_map)((SoC_System) . (memory_mem_dm))\n"
        "        (((HPS_Pins) . (DDR3)) . (nRowAddressStrobe)) . (hdl_map)((SoC_System) . (memory_mem_ras_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (nColumnAddressStrobe)) . (hdl_map)((SoC_System) . (memory_mem_cas_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (nWriteEnable)) . (hdl_map)((SoC_System) . (memory_mem_we_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (Data)) . (hdl_map)((SoC_System) . (memory_mem_dq))\n"
        "        (((HPS_Pins) . (DDR3)) . (DataStrobe)) . (hdl_map)((SoC_System) . (memory_mem_dqs), (SoC_System) . (memory_mem_dqs_n))\n"
        "        (((HPS_Pins) . (DDR3)) . (OnDieTermination)) . (hdl_map)((SoC_System) . (memory_mem_odt))\n"
        "        (((HPS_Pins) . (DDR3)) . (RZQ)) . (hdl_map)((SoC_System) . (memory_oct_rzqin))\n"
        "    }\n"
        "    public {\n"
        "        (SoC_System) . (hps_cold_reset_req_reset_n) =  ! (HPS_Reset)\n"
        "        (SoC_System) . (hps_warm_reset_req_reset_n) =  ! (HPS_Reset)\n"
        "        (SoC_System) . (hps_debug_reset_req_reset_n) =  ! (HPS_Reset)\n"
        "        HPS_FPGA_Reset = (SoC_System) . (hps_reset_out_reset)\n"
        "        AvalonMaster (ControllerClock, MasterReset, 32, 65536) LightWeightBus\n"
        "        (LightWeightBus) ' (BaseAddress) = 3221225472\n"
        "        (SoC_System) . (light_weight_bus_clk_clk) = ControllerClock\n"
        "        (SoC_System) . (light_weight_bus_reset_reset) = MasterReset\n"
        "        (LightWeightBus) . (Address) = (SoC_System) . (light_weight_bus_address)\n"
        "        (LightWeightBus) . (ByteEnable) = (SoC_System) . (light_weight_bus_byteenable)\n"
        "        (SoC_System) . (light_weight_bus_waitrequest) = (LightWeightBus) . (WaitRequest)\n"
        "        (LightWeightBus) . (WriteData) = (SoC_System) . (light_weight_bus_writedata)\n"
        "        (LightWeightBus) . (Write) = (SoC_System) . (light_weight_bus_write)\n"
        "        (LightWeightBus) . (Read) = (SoC_System) . (light_weight_bus_read)\n"
        "        (SoC_System) . (light_weight_bus_readdata) = (LightWeightBus) . (ReadData)\n"
        "        (SoC_System) . (light_weight_bus_readdatavalid) = (LightWeightBus) . (ReadValid)\n"
        "        AvalonSlave ((SoC_System) . (hps_clk_out_clk), (SoC_System) . (hps_reset_out_reset), 256, 33554432, 128) SDRAM\n"
        "        ((SDRAM) . (Clock)) ' (frequency) = 100000000\n"
        "        (SoC_System) . (sdram_address) = (SDRAM) . (Address)\n"
        "        (SoC_System) . (sdram_byteenable) = (SDRAM) . (ByteEnable)\n"
        "        (SoC_System) . (sdram_burstcount) = (SDRAM) . (BurstCount)\n"
        "        (SDRAM) . (WaitRequest) = (SoC_System) . (sdram_waitrequest)\n"
        "        (SoC_System) . (sdram_writedata) = (SDRAM) . (WriteData)\n"
        "        (SoC_System) . (sdram_write) = (SDRAM) . (Write)\n"
        "        (SoC_System) . (sdram_read) = (SDRAM) . (Read)\n"
        "        (SDRAM) . (ReadData) = (SoC_System) . (sdram_readdata)\n"
        "        (SDRAM) . (ReadValid) = (SoC_System) . (sdram_readdatavalid)\n"
        "        AvalonInterface (16, 8192) StreamBuffer\n"
        "        (StreamBuffer) ' (BaseAddress) = 4280287232\n"
        "        (SoC_System) . (streambuffer_clk_clk) = DspClock\n"
        "        (SoC_System) . (streambuffer_clken) = 1\n"
        "        (SoC_System) . (streambuffer_reset_reset) = MasterReset\n"
        "        (SoC_System) . (streambuffer_chipselect) = 1\n"
        "        (SoC_System) . (streambuffer_address) = (StreamBuffer) . (Address)\n"
        "        (SoC_System) . (streambuffer_byteenable) = 3\n"
        "        (SoC_System) . (streambuffer_writedata) = (StreamBuffer) . (WriteData)\n"
        "        (SoC_System) . (streambuffer_write) = (StreamBuffer) . (Write)\n"
        "        (StreamBuffer) . (ReadData) = (SoC_System) . (streambuffer_readdata)\n"
        "        (StreamBuffer) . (ReadValid) = 1\n"
        "    }\n"
        "}",

        "SystemController_Class SystemController",
        "import \"Library/Timing/TriggerDelay\"",
        "import \"Library/Peripherals/Synth_and_DDS/AD9915\"",
        "TriggerDelay (ControlClock, MasterReset, MasterTrigger) SynthTrigger",
        "(Registers) . (Writeable)((SynthTrigger) . (Enable))",
        "(Registers) . (Writeable)((SynthTrigger) . (Delay))",
        "(Registers) . (Writeable)((SynthTrigger) . (Length))",
        "AD9915 (ControlClock, MasterReset) SynthInst",
        "(Registers) . (Writeable)((SynthInst) . (ipUpdate))",
        "(Registers) . (ReadOnly)((SynthInst) . (opBusy))",
        "(SynthInst) . (ipFreqLowerLimit) = round((500000000) * ((4294967296) / (2500000000)))",
        "(SynthInst) . (ipFreqUpperLimit) = round((650000000) * ((4294967296) / (2500000000)))",
        "(SynthInst) . (ipStepUp) = round(((150000000) / (((1/1000 (~0.001)) * (2500000000)) / (24))) * ((4294967296) / (2500000000)))",
        "(SynthInst) . (ipStepDown) = round(((150000000) / (((1/20000 (~5e-05)) * (2500000000)) / (24))) * ((4294967296) / (2500000000)))",
        "(Registers) . (Writeable)((SynthInst) . (ipFreqLowerLimit))",
        "(Registers) . (Writeable)((SynthInst) . (ipFreqUpperLimit))",
        "(Registers) . (Writeable)((SynthInst) . (ipStepUp))",
        "(Registers) . (Writeable)((SynthInst) . (ipStepDown))",
        "(SynthInst) . (Trigger) = (SynthTrigger) . (Output)",
        "(SynthInst) . (ConnectSPI)(SClk = ((Synth) . (SPI)) . (SClk), nCS = ((Synth) . (SPI)) . (nCS), SDIO = ((Synth) . (SPI)) . (SDIO), SyncIO = ((Synth) . (SPI)) . (SyncIO), IP_Update = ((Synth) . (SPI)) . (IO_Update))",
        "(SynthInst) . (ConnectDR)(Control = (Synth) . (DR_Control), Hold = (Synth) . (DR_Hold), Over = (Synth) . (DR_Over))",
        0
    };
    if(!runTest(expected)) return false;

    endTest();
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    setupTerminal();

    printf("\n");

    if(!testLiterals    ()) goto MainError;
    if(!testIdentifiers ()) goto MainError;
    if(!testExpressions ()) goto MainError;
    if(!testModules     ()) goto MainError;
    if(!testAutogen     ()) goto MainError;
    if(!testVerification()) goto MainError;
    if(!testParser      ()) goto MainError;

    if(!testAlchaCaseStudyCombined()) goto MainError;

    endTest("All OK");
    if(ast) delete ast;
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        if(ast) delete ast;
        return -1;
}
//------------------------------------------------------------------------------

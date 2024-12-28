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
               expected,
               got.c_str());
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
        }
        if(!test(n, node, expected[n])) return false;
        n++;
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
        "(((a)[5 (~5)])[6 (~6)])[(7 (~7)) .. (9 (~9)), 8 (~8), (1 (~1)) .. (10 (~10)):(2 (~2))] = ((((b) . (c)) . (d)) . (e)) ' (attribute)",

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
    if(!testParser() ) goto MainError;

    endTest("All OK");
    if(ast) delete ast;
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        if(ast) delete ast;
        return -1;
}
//------------------------------------------------------------------------------

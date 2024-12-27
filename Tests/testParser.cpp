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
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s...\n", name); \

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
                   ANSI_RESET         "    More AST nodes than expected\n");
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
        "num c",
        "num d",
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

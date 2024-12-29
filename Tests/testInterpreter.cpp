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
#include "Interpreter.h"
//------------------------------------------------------------------------------

#include <string>
using std::string;

AST::AST*    ast = 0;
Parser*      parser;
Interpreter* interpreter;
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s...\n", name); \

    string filename;
    filename  = "testInterpreter/";
    filename += name;
    filename += ".alc";

    if(ast) delete ast;
    ast = parser->parse(filename.c_str());

    if(!ast){
        error("Cannot parse file %s", filename.c_str());
        return false;
    }
    if(!interpreter->run(ast)){
        error("Cannot interpret AST of file %s", filename.c_str());
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------

bool test(int testIndex, const char* name, const char* expected)
{
    auto symbol = interpreter->global.symbols.find(name);
    if(symbol == interpreter->global.symbols.end()){
        printf(ANSI_FG_BRIGHT_RED "FAILED:\n"
               ANSI_RESET         "    Cannot find \"%s\" in symbol table\n",
               name);
        return false;
    }
    string got = symbol->second->print();

    if(got != expected){
        printf(ANSI_FG_BRIGHT_RED "FAILED: "
               ANSI_RESET         "Test %d; "
               ANSI_FG_YELLOW     "%s\n"
               ANSI_FG_GREEN      "    Expected: %s\n"
               ANSI_FG_BRIGHT_RED "    Got:      %s\n"
               ANSI_RESET,
               testIndex+1,
               name,
               expected,
               got.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool runTest(const char** expected)
{
    for(int n = 0; expected[n]; n += 2){
        if(!expected[n]){
            printf(ANSI_FG_BRIGHT_RED "FAILED:\n"
                   ANSI_RESET         "    More AST nodes than expected\n");
            return false;
        }
        if(!test(n/2, expected[n], expected[n+1])) return false;
    }
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
        "a", "0 (~0)",
        "b", "0 (~0)",
        "c", "0 (~0)",
        "d", "0 (~0)",
        0, 0
    };
    if(!runTest(expected)) return false;

    // TODO Test that the netlist table is correct

    endTest();
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    Parser _parser;
    parser = &_parser;

    Interpreter _interpreter;
    interpreter = &_interpreter;

    setupTerminal();

    printf("\n");
    if(!testParser()) goto MainError;

    endTest("All OK");
    if(ast) delete ast;
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        if(ast) delete ast;
        return -1;
}
//------------------------------------------------------------------------------

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

Parser* parser;
//------------------------------------------------------------------------------

bool startTest(const char* name)
{
    printf(ANSI_FG_CYAN "Starting test: " ANSI_RESET "%s...\n", name); \

    string filenmae;
    filenmae  = "testParser/";
    filenmae += name;
    filenmae += ".alc";

    if(!parser->run(filenmae.c_str())){
        error("Cannot parse file %s", filenmae.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool testParser()
{
    if(!startTest("Parser")) return false;

    printf(ANSI_FG_GREEN "PASS\n" ANSI_RESET);
    return true;
}
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    Parser _parser;
    parser = &_parser;
    setupTerminal();

    printf("\n\n");
    if(!testParser() ) goto MainError;

    printf(ANSI_FG_GREEN "All OK\n\n" ANSI_RESET);
    return 0;

    MainError:
        printf(ANSI_FG_BRIGHT_RED "There were errors\n\n" ANSI_RESET);
        return -1;
}
//------------------------------------------------------------------------------

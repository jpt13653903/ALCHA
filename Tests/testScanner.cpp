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

#include "General.h"
#include "Scanner.h"
//------------------------------------------------------------------------------

int main(int argc, const char** argv)
{
    setupTerminal();

    const char*  inputFile = "../TestCases/FrontEnd/Scanner.alc";
    if(argc > 1) inputFile = argv[1];

    info("inputFile = %s", inputFile);

    Scanner scanner;
    if(!scanner.open(inputFile)){
        error("Cannot open \"%s\"", inputFile);
        return -1;
    }
    Token token;
    while(scanner.getToken(&token)) token.display();

    return 0;
}
//------------------------------------------------------------------------------

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
#include "Engine.h"
#include "BackEnd.h"
//------------------------------------------------------------------------------

void Pause()
{
    printf("\nPress Enter to continue\n");
    fflush(stdout);
    getchar();
}
//------------------------------------------------------------------------------

void PrintUsage()
{
    printf(
        "\n"
        "ALCHA Compiler, Version %d.%d\n"
        "Built on " __DATE__ " at " __TIME__ "\n"
        "\n"
        "Copyright (C) John-Philip Taylor\n"
        "jpt13653903@gmail.com\n"
        "\n"
        "This program is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU General Public License as published by\n"
        "the Free Software Foundation, either version 3 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU General Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU General Public License\n"
        "along with this program.  If not, see <http://www.gnu.org/licenses/>\n"
        "\n"
        "Usage: ALCHA main_source.alc output_folder\n"
        "Example: ALCHA main.alc \"My Project Output\"\n"
        "\n",
        MAJOR_VERSION, MINOR_VERSION // These are defined in the Makefile
    );
    Pause();
}
//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    setupTerminal();

    if(argc < 3){
        PrintUsage();
        return 0;
    }

    ENGINE Engine;
    if(!Engine.Run(argv[1])) return 1;

    // Remove path and extension from input filename
    int n = strlen(argv[1]);
    while(n > 0 && argv[1][n] != '.') n--;
    if(n > 0) argv[1][n] = 0;
    while(n > 0 && argv[1][n] != '\\' && argv[1][n] != '/') n--;
    if(n > 0) argv[1] += n+1;

    BACK_END BackEnd;
    if(!BackEnd.BuildAltera(argv[2], argv[1])) return 2;

    return 0;
}
//------------------------------------------------------------------------------


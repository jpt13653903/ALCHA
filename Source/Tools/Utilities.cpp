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

#include "Utilities.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

bool error = false;
//------------------------------------------------------------------------------

static void header(int line, const std::string& filename)
{
    if(line) printf(ANSI_FG_BRIGHT_BLACK "Line "
                    ANSI_FG_CYAN         "%05d "
                    ANSI_FG_BRIGHT_BLACK "of "
                    ANSI_FG_YELLOW       "%s\n"
                    ANSI_RESET,
                    line,
                    filename.c_str());
    else     printf(ANSI_FG_BRIGHT_BLACK "Global\n");
}
//------------------------------------------------------------------------------

void printError(int line, const std::string& filename, const char* message)
{
    if(error) return;
    error = true;

    header(line, filename);
    printf(ANSI_FG_BRIGHT_RED "  Error: " ANSI_RESET);

    if(message) printf("%s\n", message);
}
//------------------------------------------------------------------------------

void printWarning(int line, const std::string& filename, const char* message)
{
    header(line, filename);
    printf(ANSI_FG_MAGENTA "  Warning: " ANSI_RESET);

    if(message) printf("%s\n", message);
}
//------------------------------------------------------------------------------

void simplifyFilename(string& filename)
{
    int  lastSlash;
    int  n, c;
    bool changes;

    do{
        changes   = false;
        lastSlash = -1;

        for(n = 0; filename[n]; n++){
            if(filename[n] == '/'
                #ifdef WINVER
                    || filename[n] == '\\'
                #endif
            ){
                if(
                    filename[n+1] == '.' &&
                    filename[n+2] == '.' && (
                        filename[n+3] == '/'
                        #ifdef WINVER
                            || filename[n+3] == '\\'
                        #endif
                    ) && (
                        n > lastSlash+3 ||
                        filename[lastSlash+1] != '.' ||
                        filename[lastSlash+2] != '.'
                    )
                ){
                    n += 4;
                    for(c = lastSlash+1; filename[n]; c++, n++) filename[c] = filename[n];
                    filename[c] = 0;
                    changes = true;
                    break;
                }else{
                    lastSlash = n;
                }
            }
        }
    }while(changes);

    filename.resize(n);
}
//------------------------------------------------------------------------------

void align(string& body, int column)
{
    body += ' ';

    int n = body.length()-1;
    while(n >= 0){
        if(body[n] == '\n') break;
        n--;
    }

    n = column - (body.length() - n);
    while(n > 0){
        body += ' ';
        n--;
    }
}
//------------------------------------------------------------------------------


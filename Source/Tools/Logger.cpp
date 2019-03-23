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

#include "Logger.h"
//------------------------------------------------------------------------------

using namespace std;
//------------------------------------------------------------------------------

LOGGER Debug("Debug.log");
//------------------------------------------------------------------------------

LOGGER::LOGGER(const char* Filename){
  File = fopen(Filename, "wb");
}
//------------------------------------------------------------------------------

LOGGER::~LOGGER(){
  fclose(File);
}
//------------------------------------------------------------------------------

void LOGGER::print(const char* String){
  fprintf(File, "%s", String);
}
//------------------------------------------------------------------------------

void LOGGER::print(const std::string& String){
  fprintf(File, "%s", String.c_str());
}
//------------------------------------------------------------------------------

void LOGGER::print(const char* Format, int i){
  fprintf(File, Format, i);
}
//------------------------------------------------------------------------------

void LOGGER::print(const char* Format, unsigned u){
  fprintf(File, Format, u);
}
//------------------------------------------------------------------------------

void LOGGER::print(const char* Format, const char* s){
  fprintf(File, Format, s);
}
//------------------------------------------------------------------------------

void LOGGER::print(const char* Format, const string& s){
  fprintf(File, Format, s.c_str());
}
//------------------------------------------------------------------------------


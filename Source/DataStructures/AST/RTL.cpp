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

#include "RTL.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

RTL::RTL(int Line, std::string& Filename): RTL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

RTL::RTL(int Line, const char* Filename): BASE(Line, Filename, TYPE::RTL){
  Statements = 0;
}
//------------------------------------------------------------------------------

RTL::~RTL(){
  if(Statements) delete Statements;

  foreach(Parameter, Parameters){
    if(*Parameter) delete *Parameter;
  }
}
//------------------------------------------------------------------------------

BASE* RTL::Copy(bool CopyNext){
  RTL* Copy = new RTL(Source.Line, Source.Filename.c_str());

  if(Statements) Copy->Statements = (decltype(Statements))Statements->Copy(CopyNext);

  foreach(Parameter, Parameters){
    if(*Parameter) Copy->Parameters.push_back((*Parameter)->Copy(CopyNext));
  }

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

BASE* RTL::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool RTL::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void RTL::Display(){
  DisplayInfo();
  Debug.print("rtl(");
    bool isFirst = true;
    foreach(Parameter, Parameters){
      if(isFirst) Debug.print(", ");
      isFirst = false;
      if(*Parameter) (*Parameter)->Display();
    }
  Debug.print("){\n");
    if(Statements) Statements->Display();
  Debug.print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

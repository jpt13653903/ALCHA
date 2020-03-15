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

#include "FSM.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FSM::FSM(int Line, std::string& Filename): FSM(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

FSM::FSM(int Line, const char* Filename): BASE(Line, Filename, TYPE::FSM){
  Parameters = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

FSM::~FSM(){
  if(Parameters) delete Parameters;
  if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* FSM::Copy(bool CopyNext){
  FSM* Copy = new FSM(Line, Filename.c_str());

  if(Parameters) Copy->Parameters = (decltype(Parameters))Parameters->Copy(CopyNext);
  if(Statements) Copy->Statements = (decltype(Statements))Statements->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool FSM::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool FSM::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void FSM::Display(){
  DisplayInfo();
  Debug.print("fsm(");
    if(Parameters) Parameters->Display();
  Debug.print("){\n");
    if(Statements) Statements->Display();
  Debug.print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

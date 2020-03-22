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

#include "HDL.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

HDL::HDL(int Line, std::string& Filename): HDL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

HDL::HDL(int Line, const char* Filename): BASE(Line, Filename, TYPE::HDL){
  Files      = 0;
  Ports      = 0;
  Parameters = 0;
}
//------------------------------------------------------------------------------

HDL::~HDL(){
  if(Files     ) delete Files;
  if(Ports     ) delete Ports;
  if(Parameters) delete Parameters;
}
//------------------------------------------------------------------------------

BASE* HDL::Copy(bool CopyNext){
  HDL* Copy = new HDL(Source.Line, Source.Filename.c_str());

  Copy->Identifier = Identifier;

  if(Files     ) Copy->Files      = (decltype(Files     ))Files     ->Copy(CopyNext);
  if(Ports     ) Copy->Ports      = (decltype(Ports     ))Ports     ->Copy(CopyNext);
  if(Parameters) Copy->Parameters = (decltype(Parameters))Parameters->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool HDL::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool HDL::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void HDL::Display(){
  DisplayInfo();
  Debug.print("hdl (%s):\n", Identifier.c_str());
  Debug.print(" Files:\n  "     ); if(Files     ) Files     ->Display(); Debug.print("\n");
  Debug.print(" Parameters:\n  "); if(Parameters) Parameters->Display();
  Debug.print(" Ports:\n  "     ); if(Ports     ) Ports     ->Display();

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void HDL::ValidateMembers(){
  assert(Type == TYPE::HDL);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------


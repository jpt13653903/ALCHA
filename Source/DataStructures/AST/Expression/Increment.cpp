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

#include "Increment.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

INCREMENT::INCREMENT(int Line, const string& Filename): INCREMENT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

INCREMENT::INCREMENT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Increment){
}
//------------------------------------------------------------------------------

INCREMENT::~INCREMENT(){
}
//------------------------------------------------------------------------------

BASE* INCREMENT::Copy(bool CopyNext){
  INCREMENT* Copy = new INCREMENT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool INCREMENT::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool INCREMENT::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* INCREMENT::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* INCREMENT::Simplify(bool GenWire){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void INCREMENT::Display(){
  DisplayStart();

  Debug.print("++");

  DisplayEnd();
}
//------------------------------------------------------------------------------

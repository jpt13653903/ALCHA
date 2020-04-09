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

#include "Parameter.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

PARAMETER::PARAMETER(
  int             Line,
  std::string&    Filename,
  DEFINITION_TYPE DefinitionType
): PARAMETER(Line, Filename.c_str(), DefinitionType){}
//------------------------------------------------------------------------------

PARAMETER::PARAMETER(
  int             Line,
  const char*     Filename,
  DEFINITION_TYPE DefinitionType
): BASE(Line, Filename, TYPE::Parameter){
  this->DefinitionType = DefinitionType;

  ClassName       = 0;
  ArrayDimensions = 0;
}
//------------------------------------------------------------------------------

PARAMETER::~PARAMETER(){
  if(ClassName) delete ClassName;
}
//------------------------------------------------------------------------------

BASE* PARAMETER::Copy(bool CopyNext){
  PARAMETER* Copy = new PARAMETER(Source.Line, Source.Filename.c_str(), DefinitionType);

  Copy->ArrayDimensions = ArrayDimensions;
  Copy->Identifier      = Identifier;

  if(ClassName) Copy->ClassName = (decltype(ClassName))ClassName->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool PARAMETER::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool PARAMETER::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void PARAMETER::Display(){
  DisplayInfo();
  Debug.Print("Parameter (");

  switch(DefinitionType){
    case DEFINITION_TYPE::Auto   : Debug.Print("Auto):"     ); break;
    case DEFINITION_TYPE::Pin    : Debug.Print("Pin):"      ); break;
    case DEFINITION_TYPE::Net    : Debug.Print("Net):"      ); break;
    case DEFINITION_TYPE::Byte   : Debug.Print("Byte):"     ); break;
    case DEFINITION_TYPE::Char   : Debug.Print("Char):"     ); break;
    case DEFINITION_TYPE::Number : Debug.Print("Number):"   ); break;
    case DEFINITION_TYPE::Func   : Debug.Print("Function):" ); break;

    case DEFINITION_TYPE::ClassInstance:
      if(ClassName) ClassName->Display();
      else          Debug.Print("Class instance with no class name");
      Debug.Print("):");
      break;

    default: Debug.Print("Invalid definition type:\n");
  }

  Debug.Print(" %s", Identifier.c_str());

  int j;
  for(j = 0; j < ArrayDimensions; j++) Debug.Print("[]");

  Debug.Print("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------


void PARAMETER::ValidateMembers(){
  assert(Type == TYPE::Parameter);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------


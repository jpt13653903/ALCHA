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

PARSER::PARSER(){
 error   = false;
 Scanner = 0;
}
//------------------------------------------------------------------------------

PARSER::~PARSER(){
 SCANNER* Temp;
 while(Scanner){
  Temp    = Scanner;
  Scanner = Scanner->Next;
  delete Temp;
 }
}
//------------------------------------------------------------------------------

void PARSER::Error(const char* Message){
 if(error) return;
 error = true;
 printf(
  "Line %05d of %s\n  Error: %s\n",
  Token.Line,
  Scanner->Filename.String(),
  Message
 );
}
//------------------------------------------------------------------------------

void PARSER::GetToken(){
 Scanner->GetToken(&Token);
 printf("%3d Token = %s (%d)\n", Token.Line, Token.Data.String(), Token.Type);
}
//------------------------------------------------------------------------------

bool PARSER::AttributeAssignment(){
 if(Token.Type != TOKEN::Identifier){
  Error("Attribute expected");
  return false;
 }
 GetToken();

 if(Token.Type != TOKEN::Assign){
  Error("'=' expected");
  return false;
 }
 GetToken();

 if(Token.Type != TOKEN::String){
  Error("Attribute value expected");
  return false;
 }
 GetToken();

 return true;
}
//------------------------------------------------------------------------------

bool PARSER::AttributeList(){
 if(Token.Type != TOKEN::OpenAngle) return false;
 GetToken();

 while(Token.Type){
  if(!AttributeAssignment()){ // At least one
   Error("Attribute assignment expected");
   return false;
  }
  if(Token.Type == TOKEN::CloseAngle){
   GetToken();
   return true;
  }
  if(Token.Type != TOKEN::Comma){
   Error("',' expected");
   return false;
  }
  GetToken();
 }
 Error("Incomplete attribute list");
 return false;
}
//------------------------------------------------------------------------------

bool PARSER::TargetDefinition(){
 if(Token.Type != TOKEN::Target) return false;
 GetToken();

 if(!AttributeList()){
  Error("Attribute list expected");
  return false;
 }
 if(Token.Type != TOKEN::Semicolon){
  Error("';' expected");
  return false;
 }
 GetToken();
 return true;
}
//------------------------------------------------------------------------------

bool PARSER::Run(const byte* Filename){
 SCANNER* Temp;
 while(Scanner){
  Temp    = Scanner;
  Scanner = Scanner->Next;
  delete Temp;
 }

 error   = false;
 Scanner = new SCANNER;

 if(!Scanner->Open(Filename)) return false;
 GetToken();

 while(
  TargetDefinition() //|
//  IfStatement     ()
 );

 return true;
}
//------------------------------------------------------------------------------

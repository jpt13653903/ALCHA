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
 if(Token.Type) printf("  Token: %s\n", Token.Data.String());
}
//------------------------------------------------------------------------------

void PARSER::GetToken(){
 Scanner->GetToken(&Token);

 #ifdef Verbose
  printf("%s\tline %d  \t", Scanner->Filename.String(), Token.Line);
  switch(Token.Type){
   case TOKEN::Identifier: printf("Identifier\t"            ); break;
   case TOKEN::Literal   : printf("Literal   \t"            ); break;
   case TOKEN::String    : printf("String    \t\""          ); break;
   default               : printf("Token %d  \t", Token.Type); break;
  }
  printf("%s", Token.Data.String());
  switch(Token.Type){
   case TOKEN::Literal: printf(" = "); Token.Value.Display(); break;
   case TOKEN::String : printf("\""); break;
   default            : break;
  }
  printf("\n");
 #endif
}
//------------------------------------------------------------------------------

bool PARSER::AttributeAssignment(DICTIONARY* Attributes){
 if(Token.Type != TOKEN::Identifier){
  Error("Attribute expected");
  return false;
 }
 STRING Name = Token.Data;
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
 STRING* Data = new STRING;
 *Data = Token.Data;
 Attributes->Insert(Name.String(), Data);
 GetToken();

 return true;
}
//------------------------------------------------------------------------------

bool PARSER::AttributeList(DICTIONARY* Attributes){
 if(Token.Type != TOKEN::OpenAngle) return false;
 GetToken();

 while(Token.Type){
  if(!AttributeAssignment(Attributes)){ // At least one
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

AST_TargetDefinition* PARSER::TargetDefinition(){
 if(Token.Type != TOKEN::Target) return 0;
 GetToken();

 AST_TargetDefinition* Node = new AST_TargetDefinition(Token.Line);

 if(!AttributeList(&Node->Attributes)){
  Error("Attribute list expected");
  delete Node;
  return 0;
 }
 if(Token.Type != TOKEN::Semicolon){
  Error("';' expected");
  delete Node;
  return 0;
 }
 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Literal(){
 if(Token.Type != TOKEN::Literal) return 0;

 AST_Expression* Node = new AST_Expression(Token.Line, AST_Expression::Literal);

 Node->Value = new NUMBER(Token.Value);

 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Identifier(){
 if(Token.Type != TOKEN::Identifier) return 0;

 AST_Expression* Node =
  new AST_Expression(Token.Line, AST_Expression::Identifier);
 Node->Name = new STRING(Token.Data);

 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::DotExpression(){
 AST_Expression* Left = Identifier();
 if(!Left) return 0;

 while(Token.Type == TOKEN::Dot){
  GetToken();
  AST_Expression* Node = new AST_Expression(Token.Line, AST_Expression::Dot);
  Node->Left  = Left;
  Node->Right = Identifier();

  if(!Node->Right){
   Error("Identifier expected");
   delete Node;
   return 0;
  }
  Left = Node;
 }
 return Left;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Primary(){
 AST_Expression* Node;

           Node = Literal();
 if(!Node) Node = DotExpression();
// if(!Node) Node = Bracketed(); // (...) -- see Expressions.ebnf
// if(!Node) Node = ArrayConcatenation(); // @{...}

 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Expression(){
 return Primary();
}
//------------------------------------------------------------------------------

AST_Definition::ARRAY* PARSER::ArrayDefinition(){
 AST_Definition::ARRAY* Array;

 if(Token.Type != TOKEN::OpenSquare) return 0;
 GetToken();

 Array = new AST_Definition::ARRAY;

 Array->Size = Expression();

 if(Token.Type != TOKEN::CloseSquare){
  Error("']' expected");
  delete Array;
  return 0;
 }
 GetToken();
 return Array;
}
//------------------------------------------------------------------------------

AST_Definition* PARSER::IdentifierList(AST_Definition* Node){
 AST_Definition::IDENTIFIER* Identifier;
 AST_Definition::ARRAY     * Array;

 Identifier = new AST_Definition::IDENTIFIER;
 Node->Identifiers = Identifier;

 while(Token.Type){
  if(Token.Type != TOKEN::Identifier){
   Error("Identifier expected");
   delete Node;
   return 0;
  }
  Identifier->Identifier = Token.Data;
  GetToken();

  Array = ArrayDefinition();
  Identifier->Array = Array;
  while(Array){
   Array->Next = ArrayDefinition();
   Array = Array->Next;
  }
  /// @todo Put function definition in here.....................................

  if(Token.Type == TOKEN::Semicolon){
   GetToken();
   return Node;
  }

  if(Token.Type != TOKEN::Comma){
   Error("Comma expected");
   delete Node;
   return 0;
  }
  GetToken();

  Identifier->Next = new AST_Definition::IDENTIFIER;
  Identifier = Identifier->Next;
 }
 Error("Incomplete pin or signal definition");
 delete Node;
 return 0;
}
//------------------------------------------------------------------------------

AST_Definition* PARSER::Definition(){
 bool Signed = false;

 AST_Definition::DIRECTION Direction =
  AST_Definition::Bidirectional;

 if(Token.Type == TOKEN::Signed){
  Signed = true;
  GetToken();
 }

 if(Token.Type == TOKEN::In){
  Direction = AST_Definition::In;
  GetToken();

 }else if(Token.Type == TOKEN::Out){
  Direction = AST_Definition::Out;
  GetToken();
 }

 if(Token.Type == TOKEN::Signed){
  Signed = true;
  GetToken();
 }

 AST_Definition* Node = 0;
 switch(Token.Type){
  case TOKEN::Pin:
   Node = new AST_Definition(Token.Line, AST_Definition::Pin); break;
  case TOKEN::Sig:
   Node = new AST_Definition(Token.Line, AST_Definition::Sig); break;
  case TOKEN::Clk:
   Node = new AST_Definition(Token.Line, AST_Definition::Clk); break;
  case TOKEN::Int:
   Node = new AST_Definition(Token.Line, AST_Definition::Int); break;
  case TOKEN::Rat:
   Node = new AST_Definition(Token.Line, AST_Definition::Rat); break;
  case TOKEN::Float:
   Node = new AST_Definition(Token.Line, AST_Definition::Float); break;
  case TOKEN::Complex:
   Node = new AST_Definition(Token.Line, AST_Definition::Complex); break;
  default:
   if(Signed || Direction) Error("Type name expected");
   return 0;
 }
 Node->Signed    = Signed;
 Node->Direction = Direction;
 GetToken();

 if(Token.Type == TOKEN::FP_Cast){
  GetToken();
  if(Token.Type == TOKEN::OpenRound){
   GetToken();

   Node->IntegerBits = Expression();
   if(!Node->IntegerBits){
    Error("Expression expected for format specifier");
    delete Node;
    return 0;
   }
   if(Token.Type == TOKEN::Comma){
    GetToken();
    Node->FractionBits = Expression();
    if(!Node->FractionBits){
     Error("Expression expected for format specifier");
     delete Node;
     return 0;
    }
   }
   if(Token.Type != TOKEN::CloseRound){
    Error(") expected");
    delete Node;
    return 0;
   }
   GetToken();

  }else{
   Node->IntegerBits = Expression();
   if(!Node->IntegerBits){
    Error("Expression expected for format specifier");
    delete Node;
    return 0;
   }
  }
 }

 AttributeList(&Node->Attributes);
 if(error){
  delete Node;
  return 0;
 }

 return IdentifierList(Node);
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Run(const byte* Filename){
 printf("Building AST for %s...\n", Filename);

 SCANNER* Temp;
 while(Scanner){
  Temp    = Scanner;
  Scanner = Scanner->Next;
  delete Temp;
 }

 AST_Base* AST = 0;

 error   = false;
 Scanner = new SCANNER;

 if(!Scanner->Open(Filename)) return 0;
 GetToken();

 AST_Base* Node = 0;
 AST_Base* Last = 0;
 while(!error && Token.Type){
            Node = TargetDefinition();
  if(!Node) Node = Definition      ();

  if(!Node){
   Error("Unexpected token");
   if(AST) delete AST;
   return 0;
  }

  if(Last) Last->Next = Node;
  else     AST        = Node;
  Last = Node;
 }

 #ifdef Verbose
  if(AST) AST->Display();
 #endif
 return AST;
}
//------------------------------------------------------------------------------

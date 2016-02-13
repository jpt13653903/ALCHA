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
 if(error) return;

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
 AST_TargetDefinition* Node = new AST_TargetDefinition(Token.Line);
 GetToken();

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

AST_ClassDefinition* PARSER::ClassDefinition(){
 if(Token.Type != TOKEN::Class) return 0;
 AST_ClassDefinition* Node = new AST_ClassDefinition(Token.Line);
 GetToken();

 AttributeList(&Node->Attributes);

 if(Token.Type != TOKEN::Identifier){
  Error("Identifier expected");
  delete Node;
  return 0;
 }
 Node->Identifier = Token.ID;
 GetToken();

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  Node->Parameters = DefParameterList();

  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
 }

 if(Token.Type == TOKEN::Colon){
  GetToken();

  if(Token.Type != TOKEN::Identifier){
   Error("Identifier expected");
   delete Node;
   return 0;
  }
  Node->Parent = Token.ID;
  GetToken();

  if(Token.Type == TOKEN::OpenRound){
   GetToken();

   Node->ParentParameters = ParameterList();

   if(Token.Type != TOKEN::CloseRound){
    Error(") expected");
    delete Node;
    return 0;
   }
   GetToken();
  }
 }
 if(Token.Type != TOKEN::OpenCurly){
  Error("{ expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Body = Statements();

 if(Token.Type != TOKEN::CloseCurly){
  Error("} expected");
  delete Node;
  return 0;
 }
 GetToken();

 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::String(){
 if(Token.Type != TOKEN::String) return 0;

 AST_Expression* Node = new AST_Expression(Token.Line, AST_Expression::String);

 Node->StrValue = new STRING(Token.Data);

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

 AST_Expression* Node = new AST_Expression(
  Token.Line, AST_Expression::Identifier
 );
 Node->Name = Token.ID;

 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::ParameterList(){
 AST_Expression* Head;
 AST_Expression* Node;

 Head = Node = Expression();
 if(!Node) return 0;

 while(Token.Type == TOKEN::Comma){
  GetToken();
  Node->Next = Expression();
  Node       = (AST_Expression*)Node->Next;
  if(!Node){
   Error("Expression expected");
   delete Head;
   return 0;
  }
 }
 return Head;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::SliceList(){
 if(Token.Type == TOKEN::SliceAll){
  GetToken();
  return new AST_Expression(Token.Line, AST_Expression::SliceAll);
 }

 AST_Expression* Head;
 AST_Expression* Node;

 Head = Node = Array();
 if(!Node) return 0;

 while(Token.Type == TOKEN::Comma){
  GetToken();
  Node->Next = Array();
  Node       = (AST_Expression*)Node->Next;
  if(!Node){
   Error("Array expected");
   delete Head;
   return 0;
  }
 }
 return Head;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Primary(){
 AST_Expression* Node;

 Node = Identifier(); if(Node) return Node;
 Node = Literal   (); if(Node) return Node;
 Node = String    (); if(Node) return Node;

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  Node = Expression();
  if(!Node){
   Error("Expression expected");
   return 0;
  }

  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
  return Node;
 }
 return 0;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Postfix(){
 AST_Expression* Node = Primary();
 if(!Node) return 0;

 AST_Expression* Temp;

 while(Token.Type){
  if(Token.Type == TOKEN::OpenSquare){ // Array slice
   Temp = new AST_Expression(Token.Line, AST_Expression::Slice);
   GetToken();

   Temp->Left  = Node;
   Temp->Right = SliceList();
   Node = Temp;

   if(!Node->Right){
    Error("Slice list expected");
    delete Node;
    return 0;
   }

   if(Token.Type != TOKEN::CloseSquare){
    Error("] expected");
    delete Node;
    return 0;
   }
   GetToken();

  }else if(Token.Type == TOKEN::OpenRound){ // Function call
   Temp = new AST_Expression(Token.Line, AST_Expression::FunctionCall);
   GetToken();

   Temp->Left  = Node;
   Temp->Right = ParameterList();
   Node = Temp;

   if(Token.Type != TOKEN::CloseRound){
    Error(") expected");
    delete Node;
    return 0;
   }
   GetToken();

  }else if(Token.Type == TOKEN::Dot){
   Temp = new AST_Expression(Token.Line, AST_Expression::Dot);
   GetToken();

   Temp->Left  = Node;
   Temp->Right = Identifier();
   Node = Temp;

   if(!Node->Right){
    Error("Identifier expected");
    delete Node;
    return 0;
   }

  }else if(Token.Type == TOKEN::Increment){
   Temp = new AST_Expression(Token.Line, AST_Expression::Increment);
   GetToken();

   Temp->Left = Node;
   Node = Temp;

  }else if(Token.Type == TOKEN::Decrement){
   Temp = new AST_Expression(Token.Line, AST_Expression::Decrement);
   GetToken();

   Temp->Left = Node;
   Node = Temp;

  }else{
   return Node;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Unary(){
 AST_Expression* Node;
 AST_Expression* Head = 0;
 AST_Expression* Tail = 0;

 while(Token.Type){
  if(Token.Type == TOKEN::Negate){
   Node = new AST_Expression(Token.Line, AST_Expression::Negate);

  }else if(Token.Type == TOKEN::Bit_NOT){
   Node = new AST_Expression(Token.Line, AST_Expression::Bit_NOT);

  }else{
   break;
  }
  GetToken();
  if(Tail) Tail->Right = Node;
  else     Head        = Node;
  Tail = Node;
 }
 Node = Postfix();
 if(Tail) Tail->Right = Node;
 else     Head        = Node;
 Tail = Node;

 if(!Node){
  if(Head){
   Error("Postfix expected");
   delete Head;
  }
  return 0;
 }
 return Head;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Array(){
 AST_Expression* Node;
 AST_Expression* Temp;

 if(Token.Type == TOKEN::Array_Concatenate){
  Node = new AST_Expression(Token.Line, AST_Expression::ArrayConcatenate);
  GetToken();

  Temp = Node->Right = Expression();
  while(Temp){
   if(Token.Type == TOKEN::CloseCurly){
    GetToken();
    return Node;
   }
   if(Token.Type != TOKEN::Comma){
    Error("Comma expected");
    delete Node;
    return 0;
   }
   GetToken();
   Temp->Next = Expression();
   Temp = (AST_Expression*)Temp->Next;
  }
  Error("Incomplete array concatenation");
  delete Node;
  return 0;
 }
 Node = Unary();
 if(!Node) return 0;

 if(Token.Type == TOKEN::To){
  Temp = new AST_Expression(Token.Line, AST_Expression::Range);
  GetToken();

  Temp->Left  = Node;
  Temp->Right = Unary();
  Node = Temp;
  if(!Node->Right){
   Error("Range end expected");
   delete Node;
   return 0;
  }
  if(Token.Type == TOKEN::Step){
   GetToken();
   Node->Right->Next = Unary();
   if(!Node->Right->Next){
    Error("Range step expected");
    delete Node;
    return 0;
   }
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Reduction(){
 AST_Expression* Node = 0;
 switch(Token.Type){
  case TOKEN::Bit_AND:
   Node = new AST_Expression(Token.Line, AST_Expression::AND_Reduce);
   GetToken();
   break;

  case TOKEN::Bit_NAND:
   Node = new AST_Expression(Token.Line, AST_Expression::NAND_Reduce);
   GetToken();
   break;

  case TOKEN::Bit_OR:
   Node = new AST_Expression(Token.Line, AST_Expression::OR_Reduce);
   GetToken();
   break;

  case TOKEN::Bit_NOR:
   Node = new AST_Expression(Token.Line, AST_Expression::NOR_Reduce);
   GetToken();
   break;

  case TOKEN::Bit_XOR:
   Node = new AST_Expression(Token.Line, AST_Expression::XOR_Reduce);
   GetToken();
   break;

  case TOKEN::Bit_XNOR:
   Node = new AST_Expression(Token.Line, AST_Expression::XNOR_Reduce);
   GetToken();
   break;

  case TOKEN::Logical_NOT:
   Node = new AST_Expression(Token.Line, AST_Expression::Logical_NOT);
   GetToken();
   break;

  default:
   break;
 }
 if(Node){
  Node->Right = Array();
  if(!Node->Right){
   Error("Array expected");
   delete Node;
   return 0;
  }
 }else{
  Node = Array();
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::FP_Cast(AST_Expression* Node){
 AST_Expression* Temp;

 if(Token.Type == TOKEN::FP_Cast){
  Temp = new AST_Expression(Token.Line, AST_Expression::FP_Cast);
  GetToken();
  Temp->Left = Node;
  Node = Temp;

  Node->Right = Identifier(); if(Node->Right) return Node;
  Node->Right = Literal   (); if(Node->Right) return Node;

  if(Token.Type != TOKEN::OpenRound){
   Error("Format specifier expected");
   delete Node;
   return 0;
  }
  GetToken();

  Node->Right = Primary();
  if(!Node->Right){
   Error("Primary expression expected");
   delete Node;
   return 0;
  }
  if(Token.Type == TOKEN::Comma){
   GetToken();
   Node->Right->Next = Primary();
   if(!Node->Right->Next){
    Error("Primary expression expected");
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
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Cast(){
 AST_Expression* Node;
 Node = Reduction();
 if(!Node) return 0;

 return FP_Cast(Node);
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Concatenation(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Cast();
 if(!Node) return 0;

 while(Token.Type == TOKEN::Bit_Concatenate){
  Temp = new AST_Expression(Token.Line, AST_Expression::Concatenate);
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Cast();
  if(!Node->Right){
   Error("Cast expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Replication(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Concatenation();
 if(!Node) return 0;

 if(Token.Type == TOKEN::Bit_Repeat){
  Temp = new AST_Expression(Token.Line, AST_Expression::Replicate);
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Primary();
  if(!Node->Right){
   Error("Primary expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Multiplicative(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Replication();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Multiply:
    Temp = new AST_Expression(Token.Line, AST_Expression::Multiply);
    break;
   case TOKEN::Divide:
    Temp = new AST_Expression(Token.Line, AST_Expression::Divide);
    break;
   case TOKEN::Modulus:
    Temp = new AST_Expression(Token.Line, AST_Expression::Modulus);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Replication();
  if(!Node->Right){
   Error("Replication expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Additive(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Multiplicative();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Add:
    Temp = new AST_Expression(Token.Line, AST_Expression::Add);
    break;
   case TOKEN::Subtract:
    Temp = new AST_Expression(Token.Line, AST_Expression::Subtract);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Multiplicative();
  if(!Node->Right){
   Error("Multiplicative expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Shift(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Additive();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Shift_Left:
    Temp = new AST_Expression(Token.Line, AST_Expression::Shift_Left);
    break;
   case TOKEN::Shift_Right:
    Temp = new AST_Expression(Token.Line, AST_Expression::Shift_Right);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Additive();
  if(!Node->Right){
   Error("Additive expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Relational(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Shift();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Less:
    Temp = new AST_Expression(Token.Line, AST_Expression::Less);
    break;
   case TOKEN::Greater:
    Temp = new AST_Expression(Token.Line, AST_Expression::Greater);
    break;
   case TOKEN::Less_Equal:
    Temp = new AST_Expression(Token.Line, AST_Expression::Less_Equal);
    break;
   case TOKEN::Greater_Equal:
    Temp = new AST_Expression(Token.Line, AST_Expression::Greater_Equal);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Shift();
  if(!Node->Right){
   Error("Shift expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Equality(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Relational();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Equal:
    Temp = new AST_Expression(Token.Line, AST_Expression::Equal);
    break;
   case TOKEN::Not_Equal:
    Temp = new AST_Expression(Token.Line, AST_Expression::Not_Equal);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Relational();
  if(!Node->Right){
   Error("Relational expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::BitwiseAND(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = Equality();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Bit_AND:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_AND);
    break;
   case TOKEN::Bit_NAND:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_NAND);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Equality();
  if(!Node->Right){
   Error("Equality expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::BitwiseXOR(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = BitwiseAND();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Bit_XOR:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_XOR);
    break;
   case TOKEN::Bit_XNOR:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_XNOR);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = BitwiseAND();
  if(!Node->Right){
   Error("BitwiseAND expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::BitwiseOR(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = BitwiseXOR();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Bit_OR:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_OR);
    break;
   case TOKEN::Bit_NOR:
    Temp = new AST_Expression(Token.Line, AST_Expression::Bit_NOR);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = BitwiseXOR();
  if(!Node->Right){
   Error("BitwiseXOR expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::LogicalAND(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = BitwiseOR();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Logical_AND:
    Temp = new AST_Expression(Token.Line, AST_Expression::Logical_AND);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = BitwiseOR();
  if(!Node->Right){
   Error("BitwiseOR expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::LogicalOR(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = LogicalAND();
 if(!Node) return 0;

 while(Token.Type){
  switch(Token.Type){
   case TOKEN::Logical_OR:
    Temp = new AST_Expression(Token.Line, AST_Expression::Logical_OR);
    break;
   default:
    return Node;
  }
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = LogicalAND();
  if(!Node->Right){
   Error("LogicalAND expected");
   delete Node;
   return 0;
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Expression(){
 AST_Expression* Temp;
 AST_Expression* Node;

 Node = LogicalOR();
 if(!Node) return 0;

 if(Token.Type == TOKEN::Conditional){
  Temp = new AST_Expression(Token.Line, AST_Expression::Conditional);
  GetToken();

  Temp->Left = Node;
  Node = Temp;

  Node->Right = Primary();
  if(!Node->Right){
   Error("Primary expected");
   delete Node;
   return 0;
  }

  if(Token.Type != TOKEN::Colon){
   Error(": expected");
   delete Node;
   return 0;
  }
  GetToken();

  Node->Right->Next = Primary();
  if(!Node->Right->Next){
   Error("Primary expected");
   delete Node;
   return 0;
  }
 }
 return Node;
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

AST_Expression* PARSER::DefParameterList(){
 AST_Expression* Head;
 AST_Expression* Node;

 Head = Node = Identifier();
 if(!Node) return 0;

 while(Token.Type == TOKEN::Comma){
  GetToken();
  Node->Next = Identifier();
  Node       = (AST_Expression*)Node->Next;
  if(!Node){
   Error("Identifier expected");
   delete Head;
   return 0;
  }
 }
 return Head;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER* PARSER::IdentifierList(){
 AST_Definition::IDENTIFIER* Head;
 AST_Definition::IDENTIFIER* Node;
 AST_Definition::ARRAY     * Array;

 if(Token.Type != TOKEN::Identifier) return 0;

 Head = Node = new AST_Definition::IDENTIFIER;
 Node->Identifier = Token.ID;
 GetToken();

 Node->Array = Array = ArrayDefinition();
 while(Array){
  Array->Next = ArrayDefinition();
  Array = Array->Next;
 }

 if(Token.Type == TOKEN::OpenRound){ // Function definition
  GetToken();

  Node->Function   = true;
  Node->Parameters = DefParameterList();
  if(error){
   delete Head;
   return 0;
  }
  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Head;
   return 0;
  }
  GetToken();
  if(Token.Type != TOKEN::OpenCurly){
   Error("{ expected");
   delete Head;
   return 0;
  }
  GetToken();
  Node->FunctionBody = Statements();
  if(Token.Type != TOKEN::CloseCurly){
   Error("} expected");
   delete Head;
   return 0;
  }
  GetToken();
  return Head;
 }

 while(Token.Type == TOKEN::Comma){
  GetToken();

  if(Token.Type != TOKEN::Identifier){
   Error("Identifier expected");
   delete Head;
   return 0;
  }
  Node->Next = new AST_Definition::IDENTIFIER;
  Node = Node->Next;
  Node->Identifier = Token.ID;
  GetToken();

  Node->Array = Array = ArrayDefinition();
  while(Array){
   Array->Next = ArrayDefinition();
   Array = Array->Next;
  }
 }
 if(Token.Type != TOKEN::Semicolon){
  Error("; expected");
  delete Head;
  return 0;
 }
 GetToken();
 return Head;
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
  case TOKEN::Void:
   Node = new AST_Definition(Token.Line, AST_Definition::Void); break;
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

 Node->Format = FP_Cast(0);
 if(Node->Format){
  if(Node->DefinitionType == AST_Definition::Void){
   Error("Void cannot have a format");
   delete Node;
   return 0;
  }

  AST_Expression* Temp = Node->Format;
  Node->Format = Temp->Right;
  Temp->Right = 0;
  delete Temp;
 }

 AttributeList(&Node->Attributes);
 if(error){
  delete Node;
  return 0;
 }

 Node->Identifiers = IdentifierList();
 if(!Node->Identifiers){
  Error("IdentifierList expected");
  delete Node;
  return 0;
 }
 if(Node->DefinitionType == AST_Definition::Void){
  if(!Node->Identifiers->Function){
   Error("Only functions can have \"void\" type");
  }
 }
 return Node;
}
//------------------------------------------------------------------------------

bool PARSER::ValidNamespaceSpecifier(AST_Expression* Node){
 // Only a few operations are invalid:
 switch(Node->ExpressionType){
  case AST_Expression::Dot:
  case AST_Expression::Identifier:
   break;

  default:
   return false;
 }

 if(Node->Left){
  if(!ValidNamespaceSpecifier(Node->Left)) return false;
 }
 if(Node->Right){
  if(!ValidNamespaceSpecifier(Node->Right)) return false;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidTypeSpecifier(AST_Expression* Node){
 // Only a few operations are invalid:
 switch(Node->ExpressionType){
  case AST_Expression::Dot:
  case AST_Expression::Identifier:
   break;

  case AST_Expression::FunctionCall:
   if(Node->Left){ // Don't test the parameter list
    if(!ValidTypeSpecifier(Node->Left)) return false;
   }
   return true;

  default:
   return false;
 }

 // Only the root may be a function call, and test recursively
 if(Node->Left){
  if(Node->Left->ExpressionType == AST_Expression::FunctionCall) return false;
  if(!ValidTypeSpecifier(Node->Left)) return false;
 }
 if(Node->Right){
  if(Node->Right->ExpressionType == AST_Expression::FunctionCall) return false;
  if(!ValidTypeSpecifier(Node->Right)) return false;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidLHS(AST_Expression* Node){
 switch(Node->ExpressionType){
  case AST_Expression::Dot:
  case AST_Expression::Identifier:
  case AST_Expression::Concatenate:
  case AST_Expression::ArrayConcatenate:
   break;

  case AST_Expression::Slice:
   if(Node->Left){ // Don't test the slice list
    if(!ValidLHS(Node->Left)) return false;
   }
   return true;

  default:
   return false;
 }

 if(Node->Left){
  if(!ValidLHS(Node->Left)) return false;
 }
 if(Node->Right){
  if(!ValidLHS(Node->Right)) return false;
 }
 if(Node->Next){ // In the case of an array concatenation
  if(!ValidLHS((AST_Expression*)Node->Next)) return false;
 }
 return true;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Other(){
 AST_Expression* Expr = Concatenation();
 if(!Expr) return 0;

 if(Token.Type == TOKEN::Semicolon){
  if(
   Expr->ExpressionType != AST_Expression::FunctionCall &&
   Expr->ExpressionType != AST_Expression::Increment    &&
   Expr->ExpressionType != AST_Expression::Decrement
  ){
   Error("Unexpected ;");
   delete Expr;
   return 0;
  }
  GetToken();
  return Expr;
 }

 if(Token.Type == TOKEN::Dot_Curly){ // Namespace push
  if(!ValidNamespaceSpecifier(Expr)){
   Error("Invalid Name-space specifier expression");
   delete Expr;
   return 0;
  }
  AST_NamespacePush* Namespace = new AST_NamespacePush(Token.Line);
  GetToken();

  Namespace->Namespace  = Expr;
  Namespace->Statements = Statements();

  if(Token.Type != TOKEN::CloseCurly){
   Error("} expected");
   delete Namespace;
   return 0;
  }
  GetToken();
  return Namespace;
 }

 if(Token.Type == TOKEN::OpenAngle || Token.Type == TOKEN::Identifier){
  if(!ValidTypeSpecifier(Expr)){
   Error("Invalid type specifier expression");
   delete Expr;
   return 0;
  }

  AST_Definition* Def = new AST_Definition(
   Token.Line, AST_Definition::ClassInstance
  );
  Def->ClassName = Expr;
  AttributeList(&Def->Attributes);
  Def->Identifiers = IdentifierList();
  if(!Def->Identifiers){
   Error("Identifier list expected");
   delete Def;
   return 0;
  }
  return Def;
 }

 if(!ValidLHS(Expr)){
  Error("Invalid left-hand side expression");
  delete Expr;
  return 0;
 }
 AST_Assignment* Assign;
 switch(Token.Type){
  case TOKEN::Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Assign);
   break;
  case TOKEN::Raw_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Raw_Assign);
   break;
  case TOKEN::Append_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Append_Assign);
   break;
  case TOKEN::Add_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Add_Assign);
   break;
  case TOKEN::Subtract_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Subtract_Assign);
   break;
  case TOKEN::Multiply_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Multiply_Assign);
   break;
  case TOKEN::Divide_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Divide_Assign);
   break;
  case TOKEN::Modulus_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Modulus_Assign);
   break;
  case TOKEN::AND_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::AND_Assign);
   break;
  case TOKEN::OR_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::OR_Assign);
   break;
  case TOKEN::XOR_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::XOR_Assign);
   break;
  case TOKEN::Shift_Left_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Shift_Left_Assign);
   break;
  case TOKEN::Shift_Right_Assign:
   Assign = new AST_Assignment(Token.Line, AST_Assignment::Shift_Right_Assign);
   break;

  default:
   Error("Assignment operator expected");
   delete Expr;
   return 0;
 }
 GetToken();

 Assign->Left  = Expr;
 Assign->Right = Expression();

 if(!Assign->Right){
  Error("Expression expected");
  delete Assign;
  return 0;
 }
 if(Token.Type != TOKEN::Semicolon){
  Error("; expected");
  delete Assign;
  return 0;
 }
 GetToken();
 return Assign;
}
//------------------------------------------------------------------------------

AST_IfStatement* PARSER::IfStatement(){
 if(Token.Type != TOKEN::If) return 0;
 AST_IfStatement* Node = new AST_IfStatement(Token.Line);
 GetToken();

 if(Token.Type != TOKEN::OpenRound){
  Error("( expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Condition = Expression();
 if(!Node->Condition){
  Error("Expression expected");
  delete Node;
  return 0;
 }

 if(Token.Type != TOKEN::CloseRound){
  Error(") expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->TrueStatements = StatementBlock();

 if(Token.Type == TOKEN::Else){
  GetToken();
  Node->FalseStatements = StatementBlock();
 }

 return Node;
}
//------------------------------------------------------------------------------

AST_WhileLoop* PARSER::WhileLoop(){
 if(Token.Type != TOKEN::While) return 0;
 AST_WhileLoop* Node = new AST_WhileLoop(Token.Line);
 GetToken();

 if(Token.Type != TOKEN::OpenRound){
  Error("( expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Condition = Expression();
 if(!Node->Condition){
  Error("Expression expected");
  delete Node;
  return 0;
 }

 if(Token.Type != TOKEN::CloseRound){
  Error(") expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Statements = StatementBlock();
 return Node;
}
//------------------------------------------------------------------------------

AST_LoopLoop* PARSER::LoopLoop(){
 if(Token.Type != TOKEN::Loop) return 0;
 AST_LoopLoop* Node = new AST_LoopLoop(Token.Line);
 GetToken();

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  Node->Count = Expression();
  if(!Node->Count){
   Error("Expression expected");
   delete Node;
   return 0;
  }

  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
 }

 Node->Statements = StatementBlock();
 return Node;
}
//------------------------------------------------------------------------------

AST_ForLoop* PARSER::ForLoop(){
 if(Token.Type != TOKEN::For) return 0;
 AST_ForLoop* Node = new AST_ForLoop(Token.Line);
 GetToken();

 if(Token.Type != TOKEN::OpenRound){
  Error("( expected");
  delete Node;
  return 0;
 }
 GetToken();

 if(Token.Type != TOKEN::Identifier){
  Error("Identifier expected");
  delete Node;
  return 0;
 }
 Node->Identifier = Token.ID;
 GetToken();

 if(Token.Type != TOKEN::In){
  Error("\"in\" expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Array = Array();
 if(!Node->Array){
  Error("Array expected");
  delete Node;
  return 0;
 }

 if(Token.Type != TOKEN::CloseRound){
  Error(") expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Statements = StatementBlock();
 return Node;
}
//------------------------------------------------------------------------------

AST_Switch* PARSER::Switch(){
 if(Token.Type != TOKEN::Switch) return 0;
 AST_Switch* Node = new AST_Switch(Token.Line);
 GetToken();

 if(Token.Type != TOKEN::OpenRound){
  Error("( expected");
  delete Node;
  return 0;
 }
 GetToken();

 Node->Expression = Expression();
 if(!Node->Expression){
  Error("Expression expected");
  delete Node;
  return 0;
 }
 if(Token.Type != TOKEN::CloseRound){
  Error(") expected");
  delete Node;
  return 0;
 }
 GetToken();
 if(Token.Type != TOKEN::OpenCurly){
  Error("{ expected");
  delete Node;
  return 0;
 }
 GetToken();

 AST_Switch::CASE* Case;
 AST_Switch::CASE* Tail = 0;

 while(Token.Type == TOKEN::Case){
  Case = new AST_Switch::CASE;
  if(Tail) Tail->Next  = Case;
  else     Node->Cases = Case;
  Tail = Case;
  GetToken();

  if(Token.Type != TOKEN::OpenRound){
   Error("( expected");
   delete Node;
   return 0;
  }
  GetToken();

  Case->Expressions = ParameterList();
  if(!Case->Expressions){
   Error("Expression list expected");
   delete Node;
   return 0;
  }
  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
  Case->Statements = StatementBlock();
 }

 if(Token.Type == TOKEN::Default){
  GetToken();
  Node->Default = StatementBlock();
 }

 if(Token.Type != TOKEN::CloseCurly){
  Error("} expected");
  delete Node;
  return 0;
 }
 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_RTL* PARSER::RTL(){
 if(Token.Type != TOKEN::RTL) return 0;
 AST_RTL* Node = new AST_RTL(Token.Line);
 GetToken();

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  Node->Parameters = ParameterList();

  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
 }
 Node->Statements = StatementBlock();
 return Node;
}
//------------------------------------------------------------------------------

AST_FSM* PARSER::FSM(){
 if(Token.Type != TOKEN::FSM) return 0;
 AST_FSM* Node = new AST_FSM(Token.Line);
 GetToken();

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  Node->Parameters = ParameterList();

  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
 }
 Node->Statements = StatementBlock();
 return Node;
 return 0;
}
//------------------------------------------------------------------------------

AST_HDL* PARSER::HDL(){
 if(Token.Type != TOKEN::HDL) return 0;
 AST_HDL* Node = new AST_HDL(Token.Line);
 GetToken();

 if(Token.Type != TOKEN::OpenRound){
  Error("( expected");
  delete Node;
  return 0;
 }
 GetToken();

 AST_Expression* File = Node->Files = String();
 if(!File){
  Error("File name string expected");
  delete Node;
  return 0;
 }
 while(Token.Type == TOKEN::Comma){
  GetToken();
  File->Next = String();
  File = (AST_Expression*)File->Next;
  if(!File){
   Error("File name string expected");
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

 if(Token.Type != TOKEN::Identifier){
  Error("Identifier expected");
  delete Node;
  return 0;
 }
 Node->Identifier = Token.ID;
 GetToken();

 if(Token.Type == TOKEN::OpenRound){
  GetToken();

  AST_Base* Tail = 0;
  AST_Base* Temp = Other(); // Also returns assignments
  while(Temp){
   if(Temp->Type != AST_Base::Assignment){
    Error("Assignment expected");
    delete Temp;
    delete Node;
    return 0;
   }
   if(Tail) Tail->Next       = Temp;
   else     Node->Parameters = (AST_Assignment*)Temp;
   Tail = Temp;
   Temp = Other();
  }
  if(Token.Type != TOKEN::CloseRound){
   Error(") expected");
   delete Node;
   return 0;
  }
  GetToken();
 }
 if(Token.Type != TOKEN::OpenCurly){
  Error("{ expected");
  delete Node;
  return 0;
 }
 GetToken();

 AST_Definition* Tail = 0;
 AST_Definition* Temp = Definition();
 while(Temp){
  if(Tail) Tail->Next  = Temp;
  else     Node->Ports = Temp;
  Tail = Temp;
  Temp = Definition();
 }

 if(Token.Type != TOKEN::CloseCurly){
  Error("} expected");
  delete Node;
  return 0;
 }
 GetToken();
 return Node;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Statement(){
 AST_Base* Node;

 Node = TargetDefinition(); if(Node) return Node;
 Node = ClassDefinition (); if(Node) return Node;
 Node = Definition      (); if(Node) return Node;
 Node = IfStatement     (); if(Node) return Node;
 Node = WhileLoop       (); if(Node) return Node;
 Node = LoopLoop        (); if(Node) return Node;
 Node = ForLoop         (); if(Node) return Node;
 Node = Switch          (); if(Node) return Node;
 Node = RTL             (); if(Node) return Node;
 Node = FSM             (); if(Node) return Node;
 Node = HDL             (); if(Node) return Node;
 Node = Other           (); if(Node) return Node;

 if(Token.Type == TOKEN::Semicolon){
  Node = new AST_Fence(Token.Line);
  GetToken();
  return Node;
 }
 return 0;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Statements(){
 AST_Base* Head = 0;
 AST_Base* Tail = 0;
 AST_Base* Node = 0;

 while(!error && Token.Type){
  Node = Statement();
  if(!Node) return Head;

  if(Tail) Tail->Next = Node;
  else     Head       = Node;
  Tail = Node;
 }
 if(error){
  if(Head) delete Head;
  return 0;
 }
 return Head;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::StatementBlock(){
 AST_Base* Node;

 if(Token.Type == TOKEN::OpenCurly){
  GetToken();
  Node = Statements();
  if(Token.Type != TOKEN::CloseCurly){
   Error("} expected");
   if(Node) delete Node;
   return 0;
  }
  GetToken();
 }else{
  Node = Statement();
  if(!Node) Error("Statement expected");
 }
 return Node;
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

 error   = false;
 Scanner = new SCANNER;

 if(!Scanner->Open(Filename)) return 0;
 GetToken();

 AST_Base* AST = Statements();
 if(Token.Type){
  Error("Unexpected token");
  if(AST) delete AST;
  return 0;
 }

 #ifdef Verbose
  if(AST) AST->Display();
 #endif
 return AST;
}
//------------------------------------------------------------------------------

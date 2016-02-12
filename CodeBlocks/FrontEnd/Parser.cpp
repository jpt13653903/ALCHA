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
 Node = Postfix();
 if(!Node) return 0;

 if(Token.Type == TOKEN::To){
  Temp = new AST_Expression(Token.Line, AST_Expression::Range);
  GetToken();

  Temp->Left  = Node;
  Temp->Right = Postfix();
  Node = Temp;
  if(!Node->Right){
   Error("Range end expected");
   delete Node;
   return 0;
  }
  if(Token.Type == TOKEN::Step){
   GetToken();
   Node->Right->Next = Postfix();
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
 Node = Array();
 if(Tail) Tail->Right = Node;
 else     Head        = Node;
 Tail = Node;

 if(!Node){
  Error("Array expected");
  delete Head;
  return 0;
 }
 return Head;
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
  Node->Right = Unary();
  if(!Node->Right){
   Error("Unary expected");
   delete Node;
   return 0;
  }
 }else{
  Node = Unary();
 }
 return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Cast(){
 AST_Expression* Node;
 Node = Reduction();
 if(!Node) return 0;

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
  Temp = new AST_Expression(Token.Line, AST_Expression::Logical_OR);
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
 if(Token.Type != TOKEN::OpenRound) return 0;
 GetToken();

 if(Token.Type == TOKEN::CloseRound){
  GetToken();
  return 0;
 }

 AST_Expression* Head;
 AST_Expression* Node;

 Head = Node = Identifier();

 while(Node && Token.Type){
  if(Token.Type == TOKEN::CloseRound){
   GetToken();
   return Head;
  }
  if(Token.Type != TOKEN::Comma){
   Error("Comma expected");
   delete Head;
   return 0;
  }
  GetToken();
  Node->Next = Identifier();
  Node = (AST_Expression*)Node->Next;
 }
 Error("Incomplete parameter list");
 if(Head) delete Head;
 return 0;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER* PARSER::IdentifierList(){
 AST_Definition::IDENTIFIER* Head;
 AST_Definition::IDENTIFIER* Node;
 AST_Definition::ARRAY     * Array;

 Head = Node = new AST_Definition::IDENTIFIER;

 while(Token.Type){
  if(Token.Type != TOKEN::Identifier){
   Error("Identifier expected");
   delete Head;
   return 0;
  }
  Node->Identifier = Token.ID;
  GetToken();

  Array = ArrayDefinition();
  Node->Array = Array;
  while(Array){
   Array->Next = ArrayDefinition();
   Array = Array->Next;
  }

  if(Token.Type == TOKEN::OpenRound){ // Function definition
   if(Head != Node){
    Error("Identifier list not allowed with function definitions");
    delete Head;
    return 0;
   }
   Node->Function   = true;
   Node->Parameters = DefParameterList();
   if(error){
    delete Head;
    return 0;
   }
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

  if(Token.Type == TOKEN::Semicolon){
   GetToken();
   return Head;
  }

  if(Token.Type != TOKEN::Comma){
   Error("Comma expected");
   delete Head;
   return 0;
  }
  GetToken();

  Node->Next = new AST_Definition::IDENTIFIER;
  Node = Node->Next;
 }
 Error("Incomplete pin or signal definition");
 delete Head;
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

 if(Token.Type == TOKEN::FP_Cast){
  if(Node->DefinitionType == AST_Definition::Void){
   Error("Void cannot have a format");
   delete Node;
   return 0;
  }
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

 Node->Identifiers = IdentifierList();
 if(!Node->Identifiers){
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

AST_Base* PARSER::Other(){
 if(Token.Type != TOKEN::Identifier) return 0;

 AST_Definition* Def  = 0;
 AST_Expression* Expr = Concatenation();

 /// @todo Check for validity at this point...

 if(Token.Type == TOKEN::OpenAngle){ // Attributes
  Def = new AST_Definition(Token.Line, AST_Definition::ClassInstance);
  Def->ClassName = Expr;
  if(!AttributeList(&Def->Attributes)){
   delete Def;
   return 0;
  }
 }

 if(Token.Type == TOKEN::Identifier){ // Instance definition
  if(!Def) Def = new AST_Definition(Token.Line, AST_Definition::ClassInstance);
  Def->ClassName   = Expr;
  Def->Identifiers = IdentifierList();
  return Def;
 }
 if(Def){
  Error("Identifier expected in object definition");
  delete Def;
  return 0;
 }
 // At this point, it is not an object definition, but could be a function call
 // or assignment statement.

 if(Expr->ExpressionType == AST_Expression::FunctionCall){
  if(Token.Type != TOKEN::Semicolon){
   Error("; expected");
   delete Expr;
   return 0;
  }
  GetToken();
  return Expr;
 }
 // At this point it can only be an assignment statement, optionally with
 // left-hand side concatenation

 AST_Assignment* Assignment = 0;
 switch(Token.Type){
  case TOKEN::Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Assign);
   break;
  case TOKEN::Raw_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Raw_Assign);
   break;
  case TOKEN::Append_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Append_Assign);
   break;
  case TOKEN::Add_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Add_Assign);
   break;
  case TOKEN::Subtract_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Subtract_Assign);
   break;
  case TOKEN::Multiply_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Multiply_Assign);
   break;
  case TOKEN::Divide_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Divide_Assign);
   break;
  case TOKEN::Modulus_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::Modulus_Assign);
   break;
  case TOKEN::AND_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::AND_Assign);
   break;
  case TOKEN::OR_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::OR_Assign);
   break;
  case TOKEN::XOR_Assign:
   Assignment = new AST_Assignment(Token.Line, AST_Assignment::XOR_Assign);
   break;
  case TOKEN::Shift_Left_Assign:
   Assignment = new AST_Assignment(
    Token.Line, AST_Assignment::Shift_Left_Assign
   );
   break;
  case TOKEN::Shift_Right_Assign:
   Assignment = new AST_Assignment(
    Token.Line, AST_Assignment::Shift_Right_Assign
   );
   break;

  default: return Expr;
 }
 GetToken();

 Assignment->Left  = Expr;
 Assignment->Right = Expression();
 if(!Assignment->Right){
  Error("Right-hand side of assignment expected");
  delete Assignment;
  return 0;
 }
 if(Token.Type != TOKEN::Semicolon){
  Error("; expected");
  delete Assignment;
  return 0;
 }
 GetToken();
 return Assignment;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Statement(){
 AST_Base* Node;

 Node = TargetDefinition(); if(Node) return Node;
 Node = Definition      (); if(Node) return Node;
 Node = Other           (); if(Node) return Node;

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

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

using namespace std;
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
    ANSI_FG_BRIGHT_BLACK "Line %05d of %s\n"
    ANSI_FG_BRIGHT_RED "  Error:"
    ANSI_FG_YELLOW " %s\n" ANSI_RESET,
    Token.Line,
    Scanner->Filename.c_str(),
    Message
  );
  if(Token.Type) printf(
    ANSI_FG_CYAN "  Token: "
    ANSI_RESET "%s\n", Token.Data.c_str()
  );
}
//------------------------------------------------------------------------------

void PARSER::GetToken(){
  if(error) return;

  if(!Scanner->GetToken(&Token)) return;

  #ifdef DEBUG
    printf(ANSI_FG_BRIGHT_BLACK "%s:" ANSI_FG_CYAN "%05d  \t" ANSI_RESET, Scanner->Filename.c_str(), Token.Line);
    switch(Token.Type){
      case TOKEN::Identifier: printf("Identifier\t"            ); break;
      case TOKEN::Literal   : printf("Literal   \t"            ); break;
      case TOKEN::String    : printf("String    \t\""          ); break;
      default               : printf("Token %d  \t", Token.Type); break;
    }
    printf("%s", Token.Data.c_str());
    switch(Token.Type){
      case TOKEN::Literal: printf(" = "); Token.Value.Display(); break;
      case TOKEN::String : printf("\""); break;
      default            : break;
    }
    printf("\n");
  #endif
}
//------------------------------------------------------------------------------

AST_Assignment* PARSER::AttributeAssignment(){
  AST_Assignment* Node;

  if(Token.Type != TOKEN::Identifier){
    Error("Attribute expected");
    return 0;
  }
  Node = new AST_Assignment(
    Token.Line, Scanner->Filename.c_str(), AST_Assignment::Assign
  );
  Node->Left = Identifier();
  if(!Node->Left){
    Error("Identifier expected");
    delete Node;
    return 0;
  }

  if(Token.Type != TOKEN::Assign){
    Error("'=' expected");
    delete Node;
    return 0;
  }
  GetToken();

  Node->Right = Primary();
  if(!Node->Right){
    Error("Attribute value expected");
    delete Node;
    return 0;
  }

  return Node;
}
//------------------------------------------------------------------------------

AST_Assignment* PARSER::AttributeList(){
  AST_Assignment* Head = 0;
  AST_Assignment* Tail = 0;
  AST_Assignment* Node;
  
  if(Token.Type != TOKEN::OpenAngle) return 0;
  GetToken();

  while(Token.Type){
    Node = AttributeAssignment();
    if(!Node){
      Error("Attribute assignment expected");
      if(Head) delete Head;
      return 0;
    }
    if(Tail) Tail->Next = Node;
    else     Head       = Node;
    Tail = Node;

    if(Token.Type == TOKEN::CloseAngle){
      GetToken();
      return Head;
    }
    if(Token.Type != TOKEN::Comma){
      Error("',' or '>' expected");
      delete Head;
      return 0;
    }
    GetToken();
  }
  Error("Incomplete attribute list");
  if(Head) delete Head;
  return 0;
}
//------------------------------------------------------------------------------

AST_ClassDefinition* PARSER::ClassDefinition(){
  if(Token.Type != TOKEN::Class) return 0;
  AST_ClassDefinition* Node = new AST_ClassDefinition(
    Token.Line, Scanner->Filename.c_str()
  );
  GetToken();

  Node->Attributes = AttributeList();

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
    AST_ClassDefinition::PARENT* Parent;
    AST_ClassDefinition::PARENT* LastParent = 0;
    do{
      GetToken();

      Parent = new AST_ClassDefinition::PARENT;
      if(LastParent) LastParent->Next = Parent;
      else           Node->Parents    = Parent;
      LastParent = Parent;

      Parent->ClassName = TypeIdentifier();
      if(!Parent->ClassName){
        Error("Type identifier expected");
        delete Node;
        return 0;
      }

      Parent->Parameters = ParameterList();
    }while(Token.Type == TOKEN::Comma);
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

AST_EnumDefinition* PARSER::EnumDefinition(){
  if(Token.Type != TOKEN::Enum) return 0;
  AST_EnumDefinition* Node = new AST_EnumDefinition(
    Token.Line, Scanner->Filename.c_str()
  );
  GetToken();

  if(Token.Type != TOKEN::Identifier){
    Error("Identifier expected");
    delete Node;
    return 0;
  }
  Node->Identifier = Token.ID;
  GetToken();

  if(Token.Type != TOKEN::OpenCurly){
    Error("{ expected");
    delete Node;
    return 0;
  }

  AST_EnumDefinition::VALUE* Value;
  AST_EnumDefinition::VALUE* LastValue = 0;

  do{
    GetToken();

    Value = new AST_EnumDefinition::VALUE;
    if(LastValue) LastValue->Next = Value;
    else          Node->Values    = Value;
    LastValue = Value;

    if(Token.Type != TOKEN::Identifier){
      Error("Identifier expected");
      delete Node;
      return 0;
    }
    Value->Identifier = Token.ID;
    GetToken();
  }while(Token.Type == TOKEN::Comma);

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

  AST_Expression* Node = new AST_Expression(
    Token.Line, Scanner->Filename.c_str(), AST_Expression::String
  );

  Node->StrValue = new string(Token.Data);

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Literal(){
  if(Token.Type != TOKEN::Literal) return 0;

  AST_Expression* Node = new AST_Expression(
    Token.Line, Scanner->Filename.c_str(), AST_Expression::Literal
  );

  Node->Value = new NUMBER(Token.Value);

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Identifier(){
  if(Token.Type != TOKEN::Identifier) return 0;

  AST_Expression* Node = new AST_Expression(
    Token.Line, Scanner->Filename.c_str(), AST_Expression::Identifier
  );
  Node->Name = Token.ID;

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::ExpressionList(){
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

AST_Base* PARSER::Parameter(){
  AST_Assignment* Node;
  AST_Expression* Expr = Expression();
  if(!Expr) return 0;

  if(Expr->ExpressionType == AST_Expression::Identifier){
    switch(Token.Type){
      case TOKEN::Assign:
        Node = new AST_Assignment(
          Token.Line, Scanner->Filename.c_str(), AST_Assignment::Assign
        );
        break;

      case TOKEN::Raw_Assign:
        Node = new AST_Assignment(
          Token.Line, Scanner->Filename.c_str(), AST_Assignment::Raw_Assign
        );
        break;

      default:
        return Expr;
    }
    GetToken();

    Node->Left  = Expr;
    Node->Right = Expression();
    
    if(!Node->Right){
      Error("Expression expected");
      delete Node;
      return 0;
    }
    return Node;
  }
  return Expr;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::ParameterList(){
  AST_Base* Head = 0;
  AST_Base* Tail = 0;
  AST_Base* Node;
  
  if(Token.Type != TOKEN::OpenRound) return 0;
  GetToken();
  if(Token.Type == TOKEN::CloseRound){
    GetToken();
    return 0; // The caller checks for OpenRound if required
  }

  while(Token.Type){
    Node = Parameter();
    if(!Node){
      Error("Parameter assignment or expression expected");
      if(Head) delete Head;
      return 0;
    }
    if(Tail) Tail->Next = Node;
    else     Head       = Node;
    Tail = Node;

    if(Token.Type == TOKEN::CloseRound){
      GetToken();
      return Head;
    }
    if(Token.Type != TOKEN::Comma){
      Error("',' or ')' expected");
      delete Head;
      return 0;
    }
    GetToken();
  }
  Error("Incomplete parameter list");
  if(Head) delete Head;
  return 0;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Array(){
  AST_Expression* Node;

  if(Token.Type == TOKEN::OpenSquare){
    GetToken();

    Node = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::Array
    );
    Node->Right = ExpressionList();

    if(Token.Type != TOKEN::CloseSquare){
      Error("] expected");
      delete Node;
      return 0;
    }
    GetToken();
    return Node;
  }
  return 0;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::ArrayConcat(){
  AST_Expression* Node;

  if(Token.Type == TOKEN::ArrayConcatenate){
    GetToken();

    Node = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::ArrayConcatenate
    );
    Node->Right = ExpressionList();

    if(Token.Type != TOKEN::CloseSquare){
      Error("] expected");
      delete Node;
      return 0;
    }
    GetToken();
    return Node;
  }
  return 0;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::VectorConcat(){
  AST_Expression* Node;

  if(Token.Type == TOKEN::Concatenate){
    GetToken();

    Node = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::VectorConcatenate
    );
    Node->Right = ExpressionList();

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

AST_Expression* PARSER::Primary(){
  AST_Expression* Node;
  bool GlobalAttribute = false;

  if(Token.Type == TOKEN::AccessAttribute){
    GlobalAttribute = true;
    GetToken();
  }

  Node = Identifier();
  if(Node){
    if(GlobalAttribute){
      AST_Expression* Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AccessAttribute
      );
      Temp->Right = Node;
      Node = Temp;
    }
    return Node;
  }
  if(GlobalAttribute){
    Error("Identifier expected");
    return 0;
  }

  Node = Literal     (); if(Node) return Node;
  Node = Array       (); if(Node) return Node;
  Node = String      (); if(Node) return Node;
  Node = VectorConcat(); if(Node) return Node;
  Node = ArrayConcat (); if(Node) return Node;

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

AST_Expression* PARSER::CastEpr(AST_Expression* Node){
  AST_Expression* Temp;

  if(Token.Type == TOKEN::CastOp){
    Temp = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::Cast
    );
    GetToken();
    Temp->Left = Node;
    Node = Temp;

    Node->Right = Identifier(); if(Node->Right) return Node;
    Node->Right = Literal   (); if(Node->Right) return Node;

    if(Token.Type != TOKEN::OpenRound){
      Error("Cast expression specifier expected");
      delete Node;
      return 0;
    }
    GetToken();

    Node->Right = Expression();
    if(!Node->Right){
      Error("Expression expected");
      delete Node;
      return 0;
    }
    if(Token.Type == TOKEN::Comma){
      GetToken();
      Node->Right->Next = Expression();
      if(!Node->Right->Next){
        Error("Expression expected");
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
  Node = Primary();
  if(!Node) return 0;

  return CastEpr(Node);
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Postfix(){
  AST_Expression* Node = Cast();
  if(!Node) return 0;

  AST_Expression* Temp;

  while(Token.Type){
    if(Token.Type == TOKEN::OpenSquare){ // Array slice
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Slice
      );

      Temp->Left  = Node;
      Temp->Right = Array();
      Node = Temp;

      if(!Node->Right){
        Error("Slice array expected");
        delete Node;
        return 0;
      }

    }else if(Token.Type == TOKEN::OpenRound){ // Function call
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::FunctionCall
      );

      Temp->Left  = Node;
      Temp->Right = ParameterList();
      Node = Temp;

    }else if(Token.Type == TOKEN::AccessMember){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AccessMember
      );
      GetToken();

      Temp->Left  = Node;
      Temp->Right = Identifier();
      Node = Temp;

      if(!Node->Right){
        Error("Identifier expected");
        delete Node;
        return 0;
      }

    }else if(Token.Type == TOKEN::AccessMemberSafe){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AccessMemberSafe
      );
      GetToken();

      Temp->Left  = Node;
      Temp->Right = Identifier();
      Node = Temp;

      if(!Node->Right){
        Error("Identifier expected");
        delete Node;
        return 0;
      }

    }else if(Token.Type == TOKEN::AccessAttribute){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AccessAttribute
      );
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
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Increment
      );
      GetToken();

      Temp->Left = Node;
      Node = Temp;

    }else if(Token.Type == TOKEN::Decrement){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Decrement
      );
      GetToken();

      Temp->Left = Node;
      Node = Temp;

    }else if(Token.Type == TOKEN::Factorial){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Factorial
      );
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
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Negate
      );

    }else if(Token.Type == TOKEN::Bit_NOT){
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_NOT
      );

    }else if(Token.Type == TOKEN::Colon){
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Raw
      );

    }else if(Token.Type == TOKEN::Increment){
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Increment
      );

    }else if(Token.Type == TOKEN::Decrement){
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Decrement
      );

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

AST_Expression* PARSER::Range(){
  AST_Expression* Node;
  AST_Expression* Temp;

  Node = Unary();
  if(!Node) return 0;

  if(Token.Type == TOKEN::To){
    Temp = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::Range
    );
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
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AND_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_NAND:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::NAND_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_OR:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::OR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_NOR:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::NOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_XOR:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::XOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_XNOR:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::XNOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Logical_NOT:
      Node = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::Logical_NOT
      );
      GetToken();
      break;

    default:
      break;
  }
  if(Node){
    Node->Right = Range();
    if(!Node->Right){
      Error("Range expected");
      delete Node;
      return 0;
    }
  }else{
    Node = Range();
  }
  return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Stringification(){
  // TODO
  return Reduction();
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::Replication(){
  AST_Expression* Temp;
  AST_Expression* Node;

  Node = Stringification();
  if(!Node) return 0;

  if(Token.Type == TOKEN::Replicate){
    Temp = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::Replicate
    );
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

AST_Expression* PARSER::Exponential(){
  AST_Expression* Temp;
  AST_Expression* Node;

  Node = Replication();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Exponential:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Exponential
        );
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

AST_Expression* PARSER::Multiplicative(){
  AST_Expression* Temp;
  AST_Expression* Node;

  Node = Exponential();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Multiply:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Multiply
        );
        break;
      case TOKEN::Divide:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Divide
        );
        break;
      case TOKEN::Modulus:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Modulus
        );
        break;
      default:
        return Node;
    }
    GetToken();

    Temp->Left = Node;
    Node = Temp;

    Node->Right = Exponential();
    if(!Node->Right){
      Error("Exponential expected");
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Add
        );
        break;
      case TOKEN::Subtract:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Subtract
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Shift_Left
        );
        break;
      case TOKEN::Shift_Right:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Shift_Right
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Less
        );
        break;
      case TOKEN::Greater:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Greater
        );
        break;
      case TOKEN::Less_Equal:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Less_Equal
        );
        break;
      case TOKEN::Greater_Equal:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Greater_Equal
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Equal
        );
        break;
      case TOKEN::Not_Equal:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Not_Equal
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_AND
        );
        break;
      case TOKEN::Bit_NAND:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_NAND
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_XOR
        );
        break;
      case TOKEN::Bit_XNOR:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_XNOR
        );
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
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_OR
        );
        break;
      case TOKEN::Bit_NOR:
        Temp = new AST_Expression(
          Token.Line, Scanner->Filename.c_str(), AST_Expression::Bit_NOR
        );
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

AST_Expression* PARSER::Expression(){
  AST_Expression* Temp;
  AST_Expression* Node;

  Node = BitwiseOR();
  if(!Node) return 0;

  if(Token.Type == TOKEN::TernaryIf){
    Temp = new AST_Expression(
      Token.Line, Scanner->Filename.c_str(), AST_Expression::Conditional
    );
    GetToken();

    Temp->Left = Node;
    Node = Temp;

    Node->Right = Cast();
    if(!Node->Right){
      Error("Cast expected");
      delete Node;
      return 0;
    }

    if(Token.Type != TOKEN::TernaryElse){
      Error(": expected");
      delete Node;
      return 0;
    }
    GetToken();

    Node->Right->Next = Cast();
    if(!Node->Right->Next){
      Error("Cast expected");
      delete Node;
      return 0;
    }
  }
  return Node;
}
//------------------------------------------------------------------------------

AST_Expression* PARSER::TypeIdentifier(){
  AST_Expression* Node = Identifier();
  if(!Node) return 0;

  AST_Expression* Temp;

  while(Token.Type){
    if(Token.Type == TOKEN::AccessMember){
      Temp = new AST_Expression(
        Token.Line, Scanner->Filename.c_str(), AST_Expression::AccessMember
      );
      GetToken();

      Temp->Left  = Node;
      Temp->Right = Identifier();
      Node = Temp;

      if(!Node->Right){
        Error("Identifier expected");
        delete Node;
        return 0;
      }
    }else{
      return Node;
    }
  }
  return Node;
}
//------------------------------------------------------------------------------

AST_Assignment* PARSER::Initialiser(byte* Identifier){
  AST_Assignment* Node;

  switch(Token.Type){
    case TOKEN::Assign:
      Node = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Assign
      );
      break;
    case TOKEN::Raw_Assign:
      Node = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Raw_Assign
      );
      break;
    default:
      return 0;
  }
  GetToken();

  Node->Right = Expression();
  if(!Node->Right){
    Error("Expression expected");
    delete Node;
    return 0;
  }
  Node->Left       = new AST_Expression(
    Node->Line, Scanner->Filename.c_str(), AST_Expression::Identifier
  );
  Node->Left->Name = Identifier;
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

AST_Parameter* PARSER::DefParameter(){
  AST_Parameter* Node;

  switch(Token.Type){
    case TOKEN::Pin:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Pin
      ); GetToken();
      break;
    case TOKEN::Net:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Net
      ); GetToken();
      break;
    case TOKEN::Byte:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Byte
      ); GetToken();
      break;
    case TOKEN::Char:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Char
      ); GetToken();
      break;
    case TOKEN::Num:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Number
      ); GetToken();
      break;
    case TOKEN::Func:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::Func
      ); GetToken();
      break;
    case TOKEN::Identifier:
      Node = new AST_Parameter(
        Token.Line, Scanner->Filename.c_str(), AST_Parameter::ClassInstance
      );
      Node->ClassName = TypeIdentifier();
      if(!Node->ClassName){
        Error("Invalid class name specifier");
        delete Node;
        return 0;
      }
      break;

    default:
      return 0;
  }

  if(Token.Type == TOKEN::Identifier){
    Node->Identifier = Token.ID;
    GetToken();

  }else{
    if(
      Node->ClassName &&
      Node->ClassName->ExpressionType == AST_Expression::Identifier
    ){
      Node->Identifier = Node->ClassName->Name;
      delete Node->ClassName;
      Node->ClassName      = 0;
      Node->DefinitionType = AST_Parameter::Auto;

    }else{
      Error("Identifier expected");
      delete Node;
      return 0;
    }
  }
  while(Token.Type == TOKEN::OpenSquare){
    GetToken();
    if(Token.Type != TOKEN::CloseSquare){
      Error("] expected");
      delete Node;
      return 0;
    }
    GetToken();
    Node->ArrayDimensions++;
  }
  return Node;
}
//------------------------------------------------------------------------------

AST_Parameter* PARSER::DefParameterList(){
  AST_Parameter* Head;
  AST_Parameter* Node;

  Head = Node = DefParameter();
  if(!Node) return 0;

  while(Token.Type == TOKEN::Comma){
    GetToken();
    Node->Next = DefParameter();
    Node       = (AST_Parameter*)Node->Next;
    if(!Node){
      Error("DefParameter expected");
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

  Node->Initialiser = Initialiser(Node->Identifier);

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

    Node->Initialiser = Initialiser(Node->Identifier);
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
  AST_Definition::DIRECTION Direction =
    AST_Definition::Inferred;

  if(Token.Type == TOKEN::Input){
    Direction = AST_Definition::Input;
    GetToken();

  }else if(Token.Type == TOKEN::Output){
    Direction = AST_Definition::Output;
    GetToken();
  }

  AST_Definition* Node = 0;
  switch(Token.Type){
    case TOKEN::Pin:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Pin
      ); break;
    case TOKEN::Net:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Net
      ); break;
    case TOKEN::Void:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Void
      ); break;
    case TOKEN::Auto:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Auto
      ); break;
    case TOKEN::Byte:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Byte
      ); break;
    case TOKEN::Char:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Char
      ); break;
    case TOKEN::Num:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Num
      ); break;
    case TOKEN::Func:
      Node = new AST_Definition(
        Token.Line, Scanner->Filename.c_str(), AST_Definition::Func
      ); break;
    default:
      if(Direction) Error("Type name expected");
      return 0;
  }
  Node->Direction = Direction;
  GetToken();

  Node->Parameters = ParameterList();

  if(Node->Parameters){
    if(Node->DefinitionType == AST_Definition::Void){
      Error("Void type does not take parameters");
      delete Node;
      return 0;
    }
    if(Node->DefinitionType == AST_Definition::Auto){
      Error("Auto type does not take parameters");
      delete Node;
      return 0;
    }
    if(Node->DefinitionType == AST_Definition::Func){
      Error("Func type does not take parameters");
      delete Node;
      return 0;
    }
  }

  Node->Attributes = AttributeList();
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
  if(
    Node->DefinitionType == AST_Definition::Void ||
    Node->DefinitionType == AST_Definition::Auto
  ){
    if(!Node->Identifiers->Function){
      Error(
        "Only functions can have \"void\" or \"auto\" types.\n         "
        "For auto variables and nets, don't declare them at all."
      );
    }
  }
  return Node;
}
//------------------------------------------------------------------------------

bool PARSER::ValidNamespaceSpecifier(AST_Expression* Node){
  // Only a few operations are invalid:
  switch(Node->ExpressionType){
    case AST_Expression::AccessMember:
    case AST_Expression::Identifier:
      break;

    default:
      return false;
  }

  if(Node->Left){
    if(!ValidNamespaceSpecifier(Node->Left)) return false;
  }
  if(Node->Right){
    if(Node->Right->Type == AST_Base::Expression){
      if(!ValidNamespaceSpecifier((AST_Expression*)Node->Right)) return false;
    }else{
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidTypeSpecifier(AST_Expression* Node){
  // Only a few operations are invalid:
  switch(Node->ExpressionType){
    case AST_Expression::AccessMember:
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
    if(Node->Right->Type == AST_Base::Expression){
      AST_Expression* Right = (AST_Expression*)Node->Right;
      if(Right->ExpressionType == AST_Expression::FunctionCall) return false;
      if(!ValidTypeSpecifier(Right)) return false;
    }else{
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidLHS(AST_Expression* Node){
  switch(Node->ExpressionType){
    case AST_Expression::AccessMember:
    case AST_Expression::AccessMemberSafe:
    case AST_Expression::AccessAttribute:
    case AST_Expression::Identifier:
    case AST_Expression::VectorConcatenate:
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
    if(Node->Right->Type == AST_Base::Expression){
      if(!ValidLHS((AST_Expression*)Node->Right)) return false;
    }else{
      return false;
    }
  }
  if(Node->Next){ // In the case of an array concatenation
    if(!ValidLHS((AST_Expression*)Node->Next)) return false;
  }
  return true;
}
//------------------------------------------------------------------------------

AST_Base* PARSER::Other(){
  AST_Expression* Expr = Postfix();
  if(!Expr) return 0;

  if(Token.Type == TOKEN::Comma || Token.Type == TOKEN::Semicolon){
    if(
      Expr->ExpressionType != AST_Expression::FunctionCall &&
      Expr->ExpressionType != AST_Expression::Increment    &&
      Expr->ExpressionType != AST_Expression::Decrement
    ){
      Error("Unexpected end-of-statement");
      delete Expr;
      return 0;
    }
    if(Token.Type == TOKEN::Semicolon){
      Expr->Next = new AST_Fence(
        Token.Line, Scanner->Filename.c_str()
      );
    }
    GetToken();
    return Expr;
  }

  if(Token.Type == TOKEN::AccessMemberPush){ // Namespace push
    if(!ValidNamespaceSpecifier(Expr)){
      Error("Invalid Name-space specifier expression");
      delete Expr;
      return 0;
    }
    AST_NamespacePush* Namespace = new AST_NamespacePush(
      Token.Line, Scanner->Filename.c_str()
    );
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
      Token.Line, Scanner->Filename.c_str(), AST_Definition::ClassInstance
    );
    if(Expr->ExpressionType == AST_Expression::FunctionCall){
      Def->ClassName  = Expr->Left;
      Def->Parameters = Expr->Right;
      Expr->Left  = 0;
      Expr->Right = 0;
      delete Expr;
    }else{
      Def->ClassName = Expr;
    }
    Def->Attributes  = AttributeList ();
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
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Assign
      );
      break;
    case TOKEN::Raw_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Raw_Assign
      );
      break;
    case TOKEN::Append_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Append_Assign
      );
      break;
    case TOKEN::Add_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Add_Assign
      );
      break;
    case TOKEN::Subtract_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Subtract_Assign
      );
      break;
    case TOKEN::Multiply_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Multiply_Assign
      );
      break;
    case TOKEN::Divide_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Divide_Assign
      );
      break;
    case TOKEN::Modulus_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Modulus_Assign
      );
      break;
    case TOKEN::Exponential_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Exponential_Assign
      );
      break;
    case TOKEN::AND_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::AND_Assign
      );
      break;
    case TOKEN::OR_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::OR_Assign
      );
      break;
    case TOKEN::XOR_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::XOR_Assign
      );
      break;
    case TOKEN::Shift_Left_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Shift_Left_Assign
      );
      break;
    case TOKEN::Shift_Right_Assign:
      Assign = new AST_Assignment(
        Token.Line, Scanner->Filename.c_str(), AST_Assignment::Shift_Right_Assign
      );
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
  switch(Token.Type){
    case TOKEN::Comma    : Assign->Fence = false; break;
    case TOKEN::Semicolon: Assign->Fence = true ; break;
    default:
      Error(", or ; expected");
      delete Assign;
      return 0;
  }
  GetToken();
  return Assign;
}
//------------------------------------------------------------------------------

AST_IfStatement* PARSER::IfStatement(){
  if(Token.Type != TOKEN::If) return 0;
  AST_IfStatement* Node = new AST_IfStatement(
    Token.Line, Scanner->Filename.c_str()
  );
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
  AST_WhileLoop* Node = new AST_WhileLoop(Token.Line, Scanner->Filename.c_str());
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
  AST_LoopLoop* Node = new AST_LoopLoop(Token.Line, Scanner->Filename.c_str());
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
  AST_ForLoop* Node = new AST_ForLoop(Token.Line, Scanner->Filename.c_str());
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

  Node->Range = Range();
  if(!Node->Range){
    Error("Range expected");
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

AST_Import* PARSER::Import(){
  if(Token.Type != TOKEN::Import) return 0;
  AST_Import* Node = new AST_Import(Token.Line, Scanner->Filename.c_str());
  GetToken();

  if(Token.Type != TOKEN::String){
    Error("File name expected");
    delete Node;
    return 0;
  }
  Node->File = Token.Data;
  GetToken();

  if(Token.Type == TOKEN::As){
    GetToken();

    if(Token.Type != TOKEN::Identifier){
      Error("Identifier expected");
      delete Node;
      return 0;
    }
    Node->Namespace = Token.ID;
    GetToken();
  }

  if(Token.Type != TOKEN::Semicolon){
    Error("; expected");
    delete Node;
    return 0;
  }
  GetToken();

  return Node;
}
//------------------------------------------------------------------------------

AST_Switch* PARSER::Switch(){
  if(Token.Type != TOKEN::Switch) return 0;
  AST_Switch* Node = new AST_Switch(Token.Line, Scanner->Filename.c_str());
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
      Error("'(' expected");
      delete Node;
      return 0;
    }
    GetToken();

    Case->Expressions = ExpressionList();
    if(!Case->Expressions){
      Error("Expression list expected");
      delete Node;
      return 0;
    }
    if(Token.Type != TOKEN::CloseRound){
      Error("')' expected");
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

AST_Alias* PARSER::Alias(){
  if(Token.Type != TOKEN::Alias) return 0;
  GetToken();

  AST_Alias* Node = new AST_Alias(Token.Line, Scanner->Filename.c_str());

  if(Token.Type != TOKEN::Identifier){
    Error("Identifier expected");
    delete Node;
    return 0;
  }
  Node->Identifier = Token.ID;
  GetToken();

  if(Token.Type != TOKEN::Assign){
    Error("= expected");
    delete Node;
    return 0;
  }
  GetToken();

  Node->Expression = Expression();
  if(!Node->Expression){
    Error("Identifier expected");
    delete Node;
    return 0;
  }

  if(Token.Type != TOKEN::Semicolon){
    Error("; expected");
    delete Node;
    return 0;
  }
  GetToken();

  return Node;
}
//------------------------------------------------------------------------------

AST_Group* PARSER::Group(){
  if(Token.Type != TOKEN::Group) return 0;
  GetToken();

  AST_Group* Node = new AST_Group(Token.Line, Scanner->Filename.c_str());
  if(Token.Type == TOKEN::OpenAngle){
    Node->Attributes = AttributeList();
  }

  if(Token.Type == TOKEN::Identifier){
    Node->Identifier = Token.ID;
    GetToken();
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

AST_Jump* PARSER::Jump(){
  AST_Jump* Node;

  switch(Token.Type){
    case TOKEN::Return:
      Node = new AST_Jump(
        Token.Line, Scanner->Filename.c_str(), AST_Jump::Return
      );
      break;
    case TOKEN::Break:
      Node = new AST_Jump(
        Token.Line, Scanner->Filename.c_str(), AST_Jump::Break
      );
      break;
    case TOKEN::Continue:
      Node = new AST_Jump(
        Token.Line, Scanner->Filename.c_str(), AST_Jump::Continue
      );
      break;
    default:
      return 0;
  }
  GetToken();

  Node->Expression = Expression();

  if(Token.Type != TOKEN::Semicolon){
    Error("; expected");
    delete Node;
    return 0;
  }
  GetToken();

  return Node;
}
//------------------------------------------------------------------------------

AST_RTL* PARSER::RTL(){
  if(Token.Type != TOKEN::RTL) return 0;
  AST_RTL* Node = new AST_RTL(Token.Line, Scanner->Filename.c_str());
  GetToken();

  Node->Parameters = ParameterList ();
  Node->Statements = StatementBlock();
  return Node;
}
//------------------------------------------------------------------------------

AST_FSM* PARSER::FSM(){
  if(Token.Type != TOKEN::FSM) return 0;
  AST_FSM* Node = new AST_FSM(Token.Line, Scanner->Filename.c_str());
  GetToken();

  Node->Parameters = ParameterList ();
  Node->Statements = StatementBlock();
  return Node;
}
//------------------------------------------------------------------------------

AST_HDL* PARSER::HDL(){
  if(Token.Type != TOKEN::HDL) return 0;
  AST_HDL* Node = new AST_HDL(Token.Line, Scanner->Filename.c_str());
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
      while(Tail->Next) Tail = Tail->Next;
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

  Node = ClassDefinition (); if(Node) return Node;
  Node = EnumDefinition  (); if(Node) return Node;
  Node = Definition      (); if(Node) return Node;
  Node = IfStatement     (); if(Node) return Node;
  Node = WhileLoop       (); if(Node) return Node;
  Node = LoopLoop        (); if(Node) return Node;
  Node = ForLoop         (); if(Node) return Node;
  Node = Import          (); if(Node) return Node;
  Node = Switch          (); if(Node) return Node;
  Node = Group           (); if(Node) return Node;
  Node = Alias           (); if(Node) return Node;
  Node = Jump            (); if(Node) return Node;
  Node = RTL             (); if(Node) return Node;
  Node = FSM             (); if(Node) return Node;
  Node = HDL             (); if(Node) return Node;
  Node = Other           (); if(Node) return Node;

  if(Token.Type == TOKEN::Semicolon){
    Node = new AST_Fence(Token.Line, Scanner->Filename.c_str());
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
    while(Tail->Next) Tail = Tail->Next;
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

AST_Base* PARSER::Run(const char* Filename){
  debug("Building AST for %s...", Filename);

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

  #ifdef DEBUG
    if(AST) AST->Display();
  #endif
  return AST;
}
//------------------------------------------------------------------------------

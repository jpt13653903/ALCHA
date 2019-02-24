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
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_BRIGHT_RED   "  Error:"
    ANSI_RESET           " %s\n",
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

AST::ASSIGNMENT* PARSER::AttributeAssignment(){
  AST::ASSIGNMENT* Node;

  if(Token.Type != TOKEN::Identifier){
    Error("Attribute expected");
    return 0;
  }
  Node = new AST::ASSIGNMENT(
    Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Assign
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

AST::ASSIGNMENT* PARSER::AttributeList(){
  AST::ASSIGNMENT* Head = 0;
  AST::ASSIGNMENT* Tail = 0;
  AST::ASSIGNMENT* Node;
  
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

AST::CLASS_DEFINITION* PARSER::ClassDefinition(){
  if(Token.Type != TOKEN::Class) return 0;
  AST::CLASS_DEFINITION* Node = new AST::CLASS_DEFINITION(
    Token.Line, Scanner->Filename.c_str()
  );
  GetToken();

  Node->Attributes = AttributeList();

  if(Token.Type != TOKEN::Identifier){
    Error("Identifier expected");
    delete Node;
    return 0;
  }
  Node->Identifier = Token.Data;
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
    AST::CLASS_DEFINITION::PARENT* Parent;
    AST::CLASS_DEFINITION::PARENT* LastParent = 0;
    do{
      GetToken();

      Parent = new AST::CLASS_DEFINITION::PARENT;
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

AST::ENUM_DEFINITION* PARSER::EnumDefinition(){
  if(Token.Type != TOKEN::Enum) return 0;
  AST::ENUM_DEFINITION* Node = new AST::ENUM_DEFINITION(
    Token.Line, Scanner->Filename.c_str()
  );
  GetToken();

  if(Token.Type != TOKEN::Identifier){
    Error("Identifier expected");
    delete Node;
    return 0;
  }
  Node->Identifier = Token.Data;
  GetToken();

  if(Token.Type != TOKEN::OpenCurly){
    Error("{ expected");
    delete Node;
    return 0;
  }

  AST::ENUM_DEFINITION::VALUE* Value;
  AST::ENUM_DEFINITION::VALUE* LastValue = 0;

  do{
    GetToken();

    Value = new AST::ENUM_DEFINITION::VALUE;
    if(LastValue) LastValue->Next = Value;
    else          Node->Values    = Value;
    LastValue = Value;

    if(Token.Type != TOKEN::Identifier){
      Error("Identifier expected");
      delete Node;
      return 0;
    }
    Value->Identifier = Token.Data;
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

AST::EXPRESSION* PARSER::String(){
  if(Token.Type != TOKEN::String) return 0;

  AST::EXPRESSION* Node = new AST::EXPRESSION(
    Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::String
  );

  Node->StrValue = new string(Token.Data);

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Literal(){
  if(Token.Type != TOKEN::Literal) return 0;

  AST::EXPRESSION* Node = new AST::EXPRESSION(
    Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Literal
  );

  Node->Value = new NUMBER(Token.Value);

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Identifier(){
  if(Token.Type != TOKEN::Identifier) return 0;

  AST::EXPRESSION* Node = new AST::EXPRESSION(
    Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Identifier
  );
  Node->Name = Token.Data;

  GetToken();
  return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::ExpressionList(){
  AST::EXPRESSION* Head;
  AST::EXPRESSION* Node;

  Head = Node = Expression();
  if(!Node) return 0;

  while(Token.Type == TOKEN::Comma){
    GetToken();
    Node->Next = Expression();
    Node       = (AST::EXPRESSION*)Node->Next;
    if(!Node){
      Error("Expression expected");
      delete Head;
      return 0;
    }
  }
  return Head;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Parameter(){
  AST::ASSIGNMENT* Node;
  AST::EXPRESSION* Expr = Expression();
  if(!Expr) return 0;

  if(Expr->ExpressionType == AST::EXPRESSION::Identifier){
    switch(Token.Type){
      case TOKEN::Assign:
        Node = new AST::ASSIGNMENT(
          Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Assign
        );
        break;

      case TOKEN::Raw_Assign:
        Node = new AST::ASSIGNMENT(
          Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Raw_Assign
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

AST::BASE* PARSER::ParameterList(){
  AST::BASE* Head = 0;
  AST::BASE* Tail = 0;
  AST::BASE* Node;
  
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

AST::EXPRESSION* PARSER::Array(){
  AST::EXPRESSION* Node;

  if(Token.Type == TOKEN::OpenSquare){
    GetToken();

    Node = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Array
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

AST::EXPRESSION* PARSER::ArrayConcat(){
  AST::EXPRESSION* Node;

  if(Token.Type == TOKEN::ArrayConcatenate){
    GetToken();

    Node = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::ArrayConcatenate
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

AST::EXPRESSION* PARSER::VectorConcat(){
  AST::EXPRESSION* Node;

  if(Token.Type == TOKEN::Concatenate){
    GetToken();

    Node = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::VectorConcatenate
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

AST::EXPRESSION* PARSER::Primary(){
  AST::EXPRESSION* Node;
  bool GlobalAttribute = false;

  if(Token.Type == TOKEN::AccessAttribute){
    GlobalAttribute = true;
    GetToken();
  }

  Node = Identifier();
  if(Node){
    if(GlobalAttribute){
      AST::EXPRESSION* Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AccessAttribute
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

AST::EXPRESSION* PARSER::CastEpr(AST::EXPRESSION* Node){
  AST::EXPRESSION* Temp;

  if(Token.Type == TOKEN::CastOp){
    Temp = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Cast
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

AST::EXPRESSION* PARSER::Cast(){
  AST::EXPRESSION* Node;
  Node = Primary();
  if(!Node) return 0;

  return CastEpr(Node);
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Postfix(){
  AST::EXPRESSION* Node = Cast();
  if(!Node) return 0;

  AST::EXPRESSION* Temp;

  while(Token.Type){
    if(Token.Type == TOKEN::OpenSquare){ // Array slice
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Slice
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
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::FunctionCall
      );

      Temp->Left  = Node;
      Temp->Right = ParameterList();
      Node = Temp;

    }else if(Token.Type == TOKEN::AccessMember){
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AccessMember
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
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AccessMemberSafe
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
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AccessAttribute
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
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Increment
      );
      GetToken();

      Temp->Left = Node;
      Node = Temp;

    }else if(Token.Type == TOKEN::Decrement){
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Decrement
      );
      GetToken();

      Temp->Left = Node;
      Node = Temp;

    }else if(Token.Type == TOKEN::Factorial){
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Factorial
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

AST::EXPRESSION* PARSER::Unary(){
  AST::EXPRESSION* Node;
  AST::EXPRESSION* Head = 0;
  AST::EXPRESSION* Tail = 0;

  while(Token.Type){
    if(Token.Type == TOKEN::Negate){
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Negate
      );

    }else if(Token.Type == TOKEN::Bit_NOT){
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_NOT
      );

    }else if(Token.Type == TOKEN::Colon){
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Raw
      );

    }else if(Token.Type == TOKEN::Increment){
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Increment
      );

    }else if(Token.Type == TOKEN::Decrement){
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Decrement
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

AST::EXPRESSION* PARSER::Range(){
  AST::EXPRESSION* Node;
  AST::EXPRESSION* Temp;

  Node = Unary();
  if(!Node) return 0;

  if(Token.Type == TOKEN::To){
    Temp = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Range
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

AST::EXPRESSION* PARSER::Reduction(){
  AST::EXPRESSION* Node = 0;
  switch(Token.Type){
    case TOKEN::Bit_AND:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AND_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_NAND:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::NAND_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_OR:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::OR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_NOR:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::NOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_XOR:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::XOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Bit_XNOR:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::XNOR_Reduce
      );
      GetToken();
      break;

    case TOKEN::Logical_NOT:
      Node = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Logical_NOT
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

AST::EXPRESSION* PARSER::Stringification(){
  // TODO
  return Reduction();
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Replication(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Stringification();
  if(!Node) return 0;

  if(Token.Type == TOKEN::Replicate){
    Temp = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Replicate
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

AST::EXPRESSION* PARSER::Exponential(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Replication();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Exponential:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Exponential
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

AST::EXPRESSION* PARSER::Multiplicative(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Exponential();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Multiply:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Multiply
        );
        break;
      case TOKEN::Divide:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Divide
        );
        break;
      case TOKEN::Modulus:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Modulus
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

AST::EXPRESSION* PARSER::Additive(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Multiplicative();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Add:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Add
        );
        break;
      case TOKEN::Subtract:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Subtract
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

AST::EXPRESSION* PARSER::Shift(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Additive();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Shift_Left:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Shift_Left
        );
        break;
      case TOKEN::Shift_Right:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Shift_Right
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

AST::EXPRESSION* PARSER::Relational(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Shift();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Less:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Less
        );
        break;
      case TOKEN::Greater:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Greater
        );
        break;
      case TOKEN::Less_Equal:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Less_Equal
        );
        break;
      case TOKEN::Greater_Equal:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Greater_Equal
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

AST::EXPRESSION* PARSER::Equality(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Relational();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Equal:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Equal
        );
        break;
      case TOKEN::Not_Equal:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Not_Equal
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

AST::EXPRESSION* PARSER::BitwiseAND(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = Equality();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Bit_AND:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_AND
        );
        break;
      case TOKEN::Bit_NAND:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_NAND
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

AST::EXPRESSION* PARSER::BitwiseXOR(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = BitwiseAND();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Bit_XOR:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_XOR
        );
        break;
      case TOKEN::Bit_XNOR:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_XNOR
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

AST::EXPRESSION* PARSER::BitwiseOR(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = BitwiseXOR();
  if(!Node) return 0;

  while(Token.Type){
    switch(Token.Type){
      case TOKEN::Bit_OR:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_OR
        );
        break;
      case TOKEN::Bit_NOR:
        Temp = new AST::EXPRESSION(
          Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Bit_NOR
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

AST::EXPRESSION* PARSER::Expression(){
  AST::EXPRESSION* Temp;
  AST::EXPRESSION* Node;

  Node = BitwiseOR();
  if(!Node) return 0;

  if(Token.Type == TOKEN::TernaryIf){
    Temp = new AST::EXPRESSION(
      Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::Conditional
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

AST::EXPRESSION* PARSER::TypeIdentifier(){
  AST::EXPRESSION* Node = Identifier();
  if(!Node) return 0;

  AST::EXPRESSION* Temp;

  while(Token.Type){
    if(Token.Type == TOKEN::AccessMember){
      Temp = new AST::EXPRESSION(
        Token.Line, Scanner->Filename.c_str(), AST::EXPRESSION::AccessMember
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

AST::ASSIGNMENT* PARSER::Initialiser(std::string& Identifier){
  AST::ASSIGNMENT* Node;

  switch(Token.Type){
    case TOKEN::Assign:
      Node = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Assign
      );
      break;
    case TOKEN::Raw_Assign:
      Node = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Raw_Assign
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
  Node->Left       = new AST::EXPRESSION(
    Node->Line, Scanner->Filename.c_str(), AST::EXPRESSION::Identifier
  );
  Node->Left->Name = Identifier;
  return Node;
}
//------------------------------------------------------------------------------

AST::DEFINITION::ARRAY* PARSER::ArrayDefinition(){
  AST::DEFINITION::ARRAY* Array;

  if(Token.Type != TOKEN::OpenSquare) return 0;
  GetToken();

  Array = new AST::DEFINITION::ARRAY;

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

AST::PARAMETER* PARSER::DefParameter(){
  AST::PARAMETER* Node;

  switch(Token.Type){
    case TOKEN::Pin:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Pin
      ); GetToken();
      break;
    case TOKEN::Net:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Net
      ); GetToken();
      break;
    case TOKEN::Byte:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Byte
      ); GetToken();
      break;
    case TOKEN::Char:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Char
      ); GetToken();
      break;
    case TOKEN::Num:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Number
      ); GetToken();
      break;
    case TOKEN::Func:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::Func
      ); GetToken();
      break;
    case TOKEN::Identifier:
      Node = new AST::PARAMETER(
        Token.Line, Scanner->Filename.c_str(), AST::PARAMETER::ClassInstance
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
    Node->Identifier = Token.Data;
    GetToken();

  }else{
    if(
      Node->ClassName &&
      Node->ClassName->ExpressionType == AST::EXPRESSION::Identifier
    ){
      Node->Identifier = Node->ClassName->Name;
      delete Node->ClassName;
      Node->ClassName      = 0;
      Node->DefinitionType = AST::PARAMETER::Auto;

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

AST::PARAMETER* PARSER::DefParameterList(){
  AST::PARAMETER* Head;
  AST::PARAMETER* Node;

  Head = Node = DefParameter();
  if(!Node) return 0;

  while(Token.Type == TOKEN::Comma){
    GetToken();
    Node->Next = DefParameter();
    Node       = (AST::PARAMETER*)Node->Next;
    if(!Node){
      Error("DefParameter expected");
      delete Head;
      return 0;
    }
  }
  return Head;
}
//------------------------------------------------------------------------------

AST::DEFINITION::IDENTIFIER* PARSER::IdentifierList(){
  AST::DEFINITION::IDENTIFIER* Head;
  AST::DEFINITION::IDENTIFIER* Node;
  AST::DEFINITION::ARRAY     * Array;

  if(Token.Type != TOKEN::Identifier) return 0;

  Head = Node = new AST::DEFINITION::IDENTIFIER;
  Node->Identifier = Token.Data;
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
    Node->Next = new AST::DEFINITION::IDENTIFIER;
    Node = Node->Next;
    Node->Identifier = Token.Data;
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

AST::DEFINITION* PARSER::Definition(){
  AST::DEFINITION::DIRECTION Direction =
    AST::DEFINITION::Inferred;

  if(Token.Type == TOKEN::Input){
    Direction = AST::DEFINITION::Input;
    GetToken();

  }else if(Token.Type == TOKEN::Output){
    Direction = AST::DEFINITION::Output;
    GetToken();
  }

  AST::DEFINITION* Node = 0;
  switch(Token.Type){
    case TOKEN::Pin:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Pin
      ); break;
    case TOKEN::Net:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Net
      ); break;
    case TOKEN::Void:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Void
      ); break;
    case TOKEN::Auto:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Auto
      ); break;
    case TOKEN::Byte:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Byte
      ); break;
    case TOKEN::Char:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Char
      ); break;
    case TOKEN::Num:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Num
      ); break;
    case TOKEN::Func:
      Node = new AST::DEFINITION(
        Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::Func
      ); break;
    default:
      if(Direction) Error("Type name expected");
      return 0;
  }
  Node->Direction = Direction;
  GetToken();

  Node->Parameters = ParameterList();

  if(Node->Parameters){
    if(Node->DefinitionType == AST::DEFINITION::Void){
      Error("Void type does not take parameters");
      delete Node;
      return 0;
    }
    if(Node->DefinitionType == AST::DEFINITION::Auto){
      Error("Auto type does not take parameters");
      delete Node;
      return 0;
    }
    if(Node->DefinitionType == AST::DEFINITION::Func){
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
    Node->DefinitionType == AST::DEFINITION::Void ||
    Node->DefinitionType == AST::DEFINITION::Auto
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

bool PARSER::ValidNamespaceSpecifier(AST::EXPRESSION* Node){
  // Only a few operations are invalid:
  switch(Node->ExpressionType){
    case AST::EXPRESSION::AccessMember:
    case AST::EXPRESSION::Identifier:
      break;

    default:
      return false;
  }

  if(Node->Left){
    if(!ValidNamespaceSpecifier(Node->Left)) return false;
  }
  if(Node->Right){
    if(Node->Right->Type == AST::BASE::TYPE::Expression){
      if(!ValidNamespaceSpecifier((AST::EXPRESSION*)Node->Right)) return false;
    }else{
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidTypeSpecifier(AST::EXPRESSION* Node){
  // Only a few operations are invalid:
  switch(Node->ExpressionType){
    case AST::EXPRESSION::AccessMember:
    case AST::EXPRESSION::Identifier:
      break;

    case AST::EXPRESSION::FunctionCall:
      if(Node->Left){ // Don't test the parameter list
        if(!ValidTypeSpecifier(Node->Left)) return false;
      }
      return true;

    default:
      return false;
  }

  // Only the root may be a function call, and test recursively
  if(Node->Left){
    if(Node->Left->ExpressionType == AST::EXPRESSION::FunctionCall) return false;
    if(!ValidTypeSpecifier(Node->Left)) return false;
  }
  if(Node->Right){
    if(Node->Right->Type == AST::BASE::TYPE::Expression){
      AST::EXPRESSION* Right = (AST::EXPRESSION*)Node->Right;
      if(Right->ExpressionType == AST::EXPRESSION::FunctionCall) return false;
      if(!ValidTypeSpecifier(Right)) return false;
    }else{
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidLHS(AST::EXPRESSION* Node){
  switch(Node->ExpressionType){
    case AST::EXPRESSION::AccessMember:
    case AST::EXPRESSION::AccessMemberSafe:
    case AST::EXPRESSION::AccessAttribute:
    case AST::EXPRESSION::Identifier:
    case AST::EXPRESSION::VectorConcatenate:
    case AST::EXPRESSION::ArrayConcatenate:
      break;

    case AST::EXPRESSION::Slice:
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
    if(Node->Right->Type == AST::BASE::TYPE::Expression){
      if(!ValidLHS((AST::EXPRESSION*)Node->Right)) return false;
    }else{
      return false;
    }
  }
  if(Node->Next){ // In the case of an array concatenation
    if(!ValidLHS((AST::EXPRESSION*)Node->Next)) return false;
  }
  return true;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Other(){
  AST::EXPRESSION* Expr = Postfix();
  if(!Expr) return 0;

  if(Token.Type == TOKEN::Comma || Token.Type == TOKEN::Semicolon){
    if(
      Expr->ExpressionType != AST::EXPRESSION::FunctionCall &&
      Expr->ExpressionType != AST::EXPRESSION::Increment    &&
      Expr->ExpressionType != AST::EXPRESSION::Decrement
    ){
      Error("Unexpected end-of-statement");
      delete Expr;
      return 0;
    }
    if(Token.Type == TOKEN::Semicolon){
      Expr->Next = new AST::FENCE(
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
    AST::NAMESPACE_PUSH* Namespace = new AST::NAMESPACE_PUSH(
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

    AST::DEFINITION* Def = new AST::DEFINITION(
      Token.Line, Scanner->Filename.c_str(), AST::DEFINITION::ClassInstance
    );
    if(Expr->ExpressionType == AST::EXPRESSION::FunctionCall){
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
  AST::ASSIGNMENT* Assign;
  switch(Token.Type){
    case TOKEN::Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Assign
      );
      break;
    case TOKEN::Raw_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Raw_Assign
      );
      break;
    case TOKEN::Append_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Append_Assign
      );
      break;
    case TOKEN::Add_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Add_Assign
      );
      break;
    case TOKEN::Subtract_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Subtract_Assign
      );
      break;
    case TOKEN::Multiply_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Multiply_Assign
      );
      break;
    case TOKEN::Divide_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Divide_Assign
      );
      break;
    case TOKEN::Modulus_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Modulus_Assign
      );
      break;
    case TOKEN::Exponential_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Exponential_Assign
      );
      break;
    case TOKEN::AND_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::AND_Assign
      );
      break;
    case TOKEN::OR_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::OR_Assign
      );
      break;
    case TOKEN::XOR_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::XOR_Assign
      );
      break;
    case TOKEN::Shift_Left_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Shift_Left_Assign
      );
      break;
    case TOKEN::Shift_Right_Assign:
      Assign = new AST::ASSIGNMENT(
        Token.Line, Scanner->Filename.c_str(), AST::ASSIGNMENT::Shift_Right_Assign
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

AST::IF_STATEMENT* PARSER::IfStatement(){
  if(Token.Type != TOKEN::If) return 0;
  AST::IF_STATEMENT* Node = new AST::IF_STATEMENT(
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

AST::WHILE_LOOP* PARSER::WhileLoop(){
  if(Token.Type != TOKEN::While) return 0;
  AST::WHILE_LOOP* Node = new AST::WHILE_LOOP(Token.Line, Scanner->Filename.c_str());
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

AST::LOOP_LOOP* PARSER::LoopLoop(){
  if(Token.Type != TOKEN::Loop) return 0;
  AST::LOOP_LOOP* Node = new AST::LOOP_LOOP(Token.Line, Scanner->Filename.c_str());
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

AST::FOR_LOOP* PARSER::ForLoop(){
  if(Token.Type != TOKEN::For) return 0;
  AST::FOR_LOOP* Node = new AST::FOR_LOOP(Token.Line, Scanner->Filename.c_str());
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
  Node->Identifier = Token.Data;
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

AST::IMPORT* PARSER::Import(){
  if(Token.Type != TOKEN::Import) return 0;
  AST::IMPORT* Node = new AST::IMPORT(Token.Line, Scanner->Filename.c_str());
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
    Node->Namespace = Token.Data;
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

AST::SWITCH* PARSER::Switch(){
  if(Token.Type != TOKEN::Switch) return 0;
  AST::SWITCH* Node = new AST::SWITCH(Token.Line, Scanner->Filename.c_str());
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

  AST::SWITCH::CASE* Case;
  AST::SWITCH::CASE* Tail = 0;

  while(Token.Type == TOKEN::Case){
    Case = new AST::SWITCH::CASE;
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

AST::ALIAS* PARSER::Alias(){
  if(Token.Type != TOKEN::Alias) return 0;
  GetToken();

  AST::ALIAS* Node = new AST::ALIAS(Token.Line, Scanner->Filename.c_str());

  if(Token.Type != TOKEN::Identifier){
    Error("Identifier expected");
    delete Node;
    return 0;
  }
  Node->Identifier = Token.Data;
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

AST::GROUP* PARSER::Group(){
  if(Token.Type != TOKEN::Group) return 0;
  GetToken();

  AST::GROUP* Node = new AST::GROUP(Token.Line, Scanner->Filename.c_str());
  if(Token.Type == TOKEN::OpenAngle){
    Node->Attributes = AttributeList();
  }

  if(Token.Type == TOKEN::Identifier){
    Node->Identifier = Token.Data;
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

AST::JUMP* PARSER::Jump(){
  AST::JUMP* Node;

  switch(Token.Type){
    case TOKEN::Return:
      Node = new AST::JUMP(
        Token.Line, Scanner->Filename.c_str(), AST::JUMP::Return
      );
      break;
    case TOKEN::Break:
      Node = new AST::JUMP(
        Token.Line, Scanner->Filename.c_str(), AST::JUMP::Break
      );
      break;
    case TOKEN::Continue:
      Node = new AST::JUMP(
        Token.Line, Scanner->Filename.c_str(), AST::JUMP::Continue
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

AST::RTL* PARSER::RTL(){
  if(Token.Type != TOKEN::RTL) return 0;
  AST::RTL* Node = new AST::RTL(Token.Line, Scanner->Filename.c_str());
  GetToken();

  Node->Parameters = ParameterList ();
  Node->Statements = StatementBlock();
  return Node;
}
//------------------------------------------------------------------------------

AST::FSM* PARSER::FSM(){
  if(Token.Type != TOKEN::FSM) return 0;
  AST::FSM* Node = new AST::FSM(Token.Line, Scanner->Filename.c_str());
  GetToken();

  Node->Parameters = ParameterList ();
  Node->Statements = StatementBlock();
  return Node;
}
//------------------------------------------------------------------------------

AST::HDL* PARSER::HDL(){
  if(Token.Type != TOKEN::HDL) return 0;
  AST::HDL* Node = new AST::HDL(Token.Line, Scanner->Filename.c_str());
  GetToken();

  if(Token.Type != TOKEN::OpenRound){
    Error("( expected");
    delete Node;
    return 0;
  }
  GetToken();

  AST::EXPRESSION* File = Node->Files = String();
  if(!File){
    Error("File name string expected");
    delete Node;
    return 0;
  }
  while(Token.Type == TOKEN::Comma){
    GetToken();
    File->Next = String();
    File = (AST::EXPRESSION*)File->Next;
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
  Node->Identifier = Token.Data;
  GetToken();

  if(Token.Type == TOKEN::OpenRound){
    GetToken();

    AST::BASE* Tail = 0;
    AST::BASE* Temp = Other(); // Also returns assignments
    while(Temp){
      if(Temp->Type != AST::BASE::TYPE::Assignment){
        Error("Assignment expected");
        delete Temp;
        delete Node;
        return 0;
      }
      if(Tail) Tail->Next       = Temp;
      else     Node->Parameters = (AST::ASSIGNMENT*)Temp;
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

  AST::DEFINITION* Tail = 0;
  AST::DEFINITION* Temp = Definition();
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

AST::BASE* PARSER::Statement(){
  AST::BASE* Node;

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
    Node = new AST::FENCE(Token.Line, Scanner->Filename.c_str());
    GetToken();
    return Node;
  }
  return 0;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Statements(){
  AST::BASE* Head = 0;
  AST::BASE* Tail = 0;
  AST::BASE* Node = 0;

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

AST::BASE* PARSER::StatementBlock(){
  AST::BASE* Node;

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

AST::BASE* PARSER::Run(const char* Filename){
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

  AST::BASE* AST = Statements();
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

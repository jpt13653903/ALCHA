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

PARSER::PARSER()
{
    error = false;
}
//------------------------------------------------------------------------------

PARSER::~PARSER()
{
}
//------------------------------------------------------------------------------

void PARSER::Error(const char* Message)
{
    error = true;
    ::Error(Token.Line, Scanner.Filename, Message);
    if(Token.Type != TOKEN::TYPE::Unknown) printf(
        ANSI_FG_CYAN "  Token: "
        ANSI_RESET "%s\n", Token.Data.c_str()
    );
}
//------------------------------------------------------------------------------

void PARSER::GetToken()
{
    if(error) return;

    if(!Scanner.GetToken(&Token)) return;

    #ifdef DEBUG
        Debug.Print(ANSI_FG_BRIGHT_BLACK);
        Debug.Print(Scanner.Filename);
        Debug.Print(":" ANSI_FG_CYAN "");
        Debug.Print("%05d", Token.Line);
        Debug.Print("  \t" ANSI_RESET);
        switch(Token.Type){
            case TOKEN::TYPE::Identifier: Debug.Print("Identifier\t"                 ); break;
            case TOKEN::TYPE::Literal   : Debug.Print("Literal   \t"                 ); break;
            case TOKEN::TYPE::String    : Debug.Print("String    \t\""               ); break;
            default                     : Debug.Print("Token %d  \t", (int)Token.Type); break;
        }
        Debug.Print(Token.Data.c_str());
        switch(Token.Type){
            case TOKEN::TYPE::Literal: Debug.Print(" = %s", Token.Value.Display()); break;
            case TOKEN::TYPE::String : Debug.Print("\""); break;
            default                  : break;
        }
        Debug.Print("\n");
    #endif
}
//------------------------------------------------------------------------------

AST::ASSIGNMENT* PARSER::AttributeAssignment()
{
    AST::ASSIGNMENT* Node;

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Attribute expected");
        return 0;
    }
    Node = new AST::ASSIGN(Token.Line, Scanner.Filename);
    Node->Left = Identifier();
    if(!Node->Left){
        Error("Identifier expected");
        delete Node;
        return 0;
    }

    if(Token.Type != TOKEN::TYPE::Assign){
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

AST::ASSIGNMENT* PARSER::AttributeList()
{
    AST::ASSIGNMENT* Head = 0;
    AST::ASSIGNMENT* Tail = 0;
    AST::ASSIGNMENT* Node;

    if(Token.Type != TOKEN::TYPE::OpenAngle) return 0;
    GetToken();

    while(Token.Type != TOKEN::TYPE::Unknown){
        Node = AttributeAssignment();
        if(!Node){
            Error("Attribute assignment expected");
            if(Head) delete Head;
            return 0;
        }
        if(Tail){
            Tail->Next = Node;
            Node->Prev = Tail;
        }else{
            Head = Node;
        }
        Tail = Node;

        if(Token.Type == TOKEN::TYPE::CloseAngle){
            GetToken();
            return Head;
        }
        if(Token.Type != TOKEN::TYPE::Comma){
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

AST::CLASS_DEFINITION* PARSER::ClassDefinition()
{
    if(Token.Type != TOKEN::TYPE::Class) return 0;
    AST::CLASS_DEFINITION* Node = new AST::CLASS_DEFINITION(
        Token.Line, Scanner.Filename
    );
    GetToken();

    Node->Attributes = AttributeList();

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    Node->Identifier = Token.Data;
    GetToken();

    if(Token.Type == TOKEN::TYPE::OpenRound){
        GetToken();

        Node->Parameters = DefParameterList();

        if(Token.Type != TOKEN::TYPE::CloseRound){
            Error(") expected");
            delete Node;
            return 0;
        }
        GetToken();
    }

    if(Token.Type == TOKEN::TYPE::Colon){
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

            ParameterList(Parent->Parameters);
        }while(Token.Type == TOKEN::TYPE::Comma);
    }
    if(Token.Type != TOKEN::TYPE::OpenCurly){
        Error("{ expected");
        delete Node;
        return 0;
    }
    GetToken();

    Node->Body = Statements();

    if(Token.Type != TOKEN::TYPE::CloseCurly){
        Error("} expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::ENUM_DEFINITION* PARSER::EnumDefinition()
{
    if(Token.Type != TOKEN::TYPE::Enum) return 0;
    AST::ENUM_DEFINITION* Node = new AST::ENUM_DEFINITION(
        Token.Line, Scanner.Filename
    );
    GetToken();

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    Node->Identifier = Token.Data;
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenCurly){
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

        if(Token.Type != TOKEN::TYPE::Identifier){
            Error("Identifier expected");
            delete Node;
            return 0;
        }
        Value->Identifier = Token.Data;
        GetToken();
    }while(Token.Type == TOKEN::TYPE::Comma);

    if(Token.Type != TOKEN::TYPE::CloseCurly){
        Error("} expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::String()
{
    if(Token.Type != TOKEN::TYPE::String) return 0;

    AST::STRING* Node = new AST::STRING(Token.Line, Scanner.Filename);

    Node->Value = Token.Data;

    GetToken();
    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Literal()
{
    switch(Token.Type){
        case TOKEN::TYPE::True   : Token.Value = 1; break;
        case TOKEN::TYPE::False  : Token.Value = 0; break;
        case TOKEN::TYPE::Literal:                  break;
        default            : return 0;
    }

    AST::LITERAL* Node = new AST::LITERAL(Token.Line, Scanner.Filename);

    Node->Value = Token.Value;

    GetToken();
    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Identifier()
{
    if(Token.Type != TOKEN::TYPE::Identifier) return 0;

    AST::IDENTIFIER* Node = new AST::IDENTIFIER(Token.Line, Scanner.Filename);
    Node->Name = Token.Data;

    GetToken();
    return Node;
}
//------------------------------------------------------------------------------

bool PARSER::ExpressionList(std::vector<AST::EXPRESSION*>& List)
{
    AST::EXPRESSION* Node;

    Node = Expression();
    if(!Node) return false;
    List.push_back(Node);

    while(Token.Type == TOKEN::TYPE::Comma){
        GetToken();
        Node = Expression();
        if(!Node){
            Error("Expression expected");
            foreach(Element, List) delete *Element;
            List.clear();
            return false;
        }
        List.push_back(Node);
    }
    return true;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Parameter()
{
    AST::ASSIGNMENT* Node;
    AST::EXPRESSION* Expr = Expression();
    if(!Expr) return 0;

    if(Expr->Type == AST::BASE::TYPE::Identifier){
        switch(Token.Type){
            case TOKEN::TYPE::Assign:
                Node = new AST::ASSIGN(Token.Line, Scanner.Filename);
                break;

            case TOKEN::TYPE::Raw_Assign:
                Node = new AST::RAW_ASSIGN(Token.Line, Scanner.Filename);
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

bool PARSER::ParameterList(list<AST::BASE*>& Parameters)
{
    AST::BASE* Node;

    if(Token.Type != TOKEN::TYPE::OpenRound) return 0;
    GetToken();
    if(Token.Type == TOKEN::TYPE::CloseRound){
        GetToken();
        return true; // The caller checks for OpenRound if required
    }

    while(Token.Type != TOKEN::TYPE::Unknown){
        Node = Parameter();
        if(!Node){
            Error("Parameter assignment or expression expected");
            return false;
        }
        Parameters.push_back(Node);

        if(Token.Type == TOKEN::TYPE::CloseRound){
            GetToken();
            return true;
        }
        if(Token.Type != TOKEN::TYPE::Comma){
            Error("',' or ')' expected");
            return false;
        }
        GetToken();
    }
    Error("Incomplete parameter list");
    return false;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Array()
{
    AST::ARRAY* Node;

    if(Token.Type == TOKEN::TYPE::OpenSquare){
        GetToken();

        Node = new AST::ARRAY(Token.Line, Scanner.Filename);
        ExpressionList(Node->Elements);

        if(Token.Type != TOKEN::TYPE::CloseSquare){
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

AST::EXPRESSION* PARSER::ArrayConcat()
{
    AST::ARRAYCONCATENATE* Node;

    if(Token.Type == TOKEN::TYPE::ArrayConcatenate){
        GetToken();

        Node = new AST::ARRAYCONCATENATE(Token.Line, Scanner.Filename);
        ExpressionList(Node->Elements);

        if(Token.Type != TOKEN::TYPE::CloseSquare){
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

AST::EXPRESSION* PARSER::VectorConcat()
{
    AST::VECTORCONCATENATE* Node;

    if(Token.Type == TOKEN::TYPE::Concatenate){
        GetToken();

        Node = new AST::VECTORCONCATENATE(Token.Line, Scanner.Filename);
        ExpressionList(Node->Elements);

        if(Token.Type != TOKEN::TYPE::CloseRound){
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

AST::EXPRESSION* PARSER::Primary()
{
    AST::EXPRESSION* Node;
    bool GlobalAttribute = false;

    if(Token.Type == TOKEN::TYPE::AccessAttribute){
        GlobalAttribute = true;
        GetToken();
    }

    Node = Identifier();
    if(Node){
        if(GlobalAttribute){
            AST::EXPRESSION* Temp = new AST::ACCESSATTRIBUTE(Token.Line, Scanner.Filename);
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

    if(Token.Type == TOKEN::TYPE::OpenRound){
        GetToken();

        Node = Expression();
        if(!Node){
            Error("Expression expected");
            return 0;
        }

        if(Token.Type != TOKEN::TYPE::CloseRound){
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

AST::EXPRESSION* PARSER::CastEpr(AST::EXPRESSION* Node)
{
    AST::EXPRESSION* Temp;

    if(Token.Type == TOKEN::TYPE::CastOp){
        Temp = new AST::CAST(Token.Line, Scanner.Filename);
        GetToken();
        Temp->Left = Node;
        Node = Temp;

        Node->Right = Identifier(); if(Node->Right) return Node;
        Node->Right = Literal   (); if(Node->Right) return Node;

        if(Token.Type != TOKEN::TYPE::OpenRound){
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
        if(Token.Type == TOKEN::TYPE::Comma){
            GetToken();
            Node->Right->Next = Expression();
            if(!Node->Right->Next){
                Error("Expression expected");
                delete Node;
                return 0;
            }
            Node->Right->Next->Prev = Node->Right;
        }
        if(Token.Type != TOKEN::TYPE::CloseRound){
            Error(") expected");
            delete Node;
            return 0;
        }
        GetToken();
    }
    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Cast()
{
    AST::EXPRESSION* Node;
    Node = Primary();
    if(!Node) return 0;

    return CastEpr(Node);
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Postfix()
{
    AST::EXPRESSION* Node = Cast();
    if(!Node) return 0;

    AST::EXPRESSION* Temp;

    while(Token.Type != TOKEN::TYPE::Unknown){
        if(Token.Type == TOKEN::TYPE::OpenSquare){ // Array slice
            Temp = new AST::SLICE(Token.Line, Scanner.Filename);

            Temp->Left  = Node;
            Temp->Right = Array();
            Node = Temp;

            if(!Node->Right){
                Error("Slice array expected");
                delete Node;
                return 0;
            }

        }else if(Token.Type == TOKEN::TYPE::OpenRound){ // Function call
            Temp = new AST::FUNCTIONCALL(Token.Line, Scanner.Filename);

            Temp->Left  = Node;
            ParameterList(((AST::FUNCTIONCALL*)Temp)->Parameters);
            Node = Temp;

        }else if(Token.Type == TOKEN::TYPE::AccessMember){
            Temp = new AST::ACCESSMEMBER(Token.Line, Scanner.Filename);
            GetToken();

            Temp->Left  = Node;
            Temp->Right = Identifier();
            Node = Temp;

            if(!Node->Right){
                Error("Identifier expected");
                delete Node;
                return 0;
            }

        }else if(Token.Type == TOKEN::TYPE::AccessMemberSafe){
            Temp = new AST::ACCESSMEMBERSAFE(Token.Line, Scanner.Filename);
            GetToken();

            Temp->Left  = Node;
            Temp->Right = Identifier();
            Node = Temp;

            if(!Node->Right){
                Error("Identifier expected");
                delete Node;
                return 0;
            }

        }else if(Token.Type == TOKEN::TYPE::AccessAttribute){
            Temp = new AST::ACCESSATTRIBUTE(Token.Line, Scanner.Filename);
            GetToken();

            Temp->Left  = Node;
            Temp->Right = Identifier();
            Node = Temp;

            if(!Node->Right){
                Error("Identifier expected");
                delete Node;
                return 0;
            }

        }else if(Token.Type == TOKEN::TYPE::Increment){
            Temp = new AST::INCREMENT(Token.Line, Scanner.Filename);
            GetToken();

            Temp->Left = Node;
            Node = Temp;

        }else if(Token.Type == TOKEN::TYPE::Decrement){
            Temp = new AST::DECREMENT(Token.Line, Scanner.Filename);
            GetToken();

            Temp->Left = Node;
            Node = Temp;

        }else if(Token.Type == TOKEN::TYPE::Factorial){
            Temp = new AST::FACTORIAL(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Unary()
{
    AST::EXPRESSION* Node;
    AST::EXPRESSION* Head = 0;
    AST::EXPRESSION* Tail = 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        if(Token.Type == TOKEN::TYPE::Negate){
            Node = new AST::NEGATE(Token.Line, Scanner.Filename);

        }else if(Token.Type == TOKEN::TYPE::Bit_NOT){
            Node = new AST::BIT_NOT(Token.Line, Scanner.Filename);

        }else if(Token.Type == TOKEN::TYPE::Colon){
            Node = new AST::RAW(Token.Line, Scanner.Filename);

        }else if(Token.Type == TOKEN::TYPE::Increment){
            Node = new AST::INCREMENT(Token.Line, Scanner.Filename);

        }else if(Token.Type == TOKEN::TYPE::Decrement){
            Node = new AST::DECREMENT(Token.Line, Scanner.Filename);

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

AST::EXPRESSION* PARSER::Range()
{
    AST::EXPRESSION* Node;
    AST::RANGE*      Temp;

    Node = Unary();
    if(!Node) return 0;

    if(Token.Type == TOKEN::TYPE::To){
        Temp = new AST::RANGE(Token.Line, Scanner.Filename);
        GetToken();

        Temp->Left  = Node;
        Temp->Right = Unary();
        if(!Temp->Right){
            Error("Range end expected");
            delete Temp;
            return 0;
        }
        if(Token.Type == TOKEN::TYPE::Step){
            GetToken();
            Temp->Step = Unary();
            if(!Temp->Step){
                Error("Range step expected");
                delete Temp;
                return 0;
            }
        }
        Node = Temp;
    }
    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Reduction()
{
    AST::EXPRESSION* Node = 0;
    switch(Token.Type){
        case TOKEN::TYPE::Bit_AND:
            Node = new AST::AND_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Bit_NAND:
            Node = new AST::NAND_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Bit_OR:
            Node = new AST::OR_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Bit_NOR:
            Node = new AST::NOR_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Bit_XOR:
            Node = new AST::XOR_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Bit_XNOR:
            Node = new AST::XNOR_REDUCE(Token.Line, Scanner.Filename);
            GetToken();
            break;

        case TOKEN::TYPE::Logical_NOT:
            Node = new AST::LOGICAL_NOT(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Stringification()
{
    // TODO
    return Reduction();
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::Replication()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Stringification();
    if(!Node) return 0;

    if(Token.Type == TOKEN::TYPE::Replicate){
        Temp = new AST::REPLICATE(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Exponential()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Replication();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Exponential:
                Temp = new AST::EXPONENTIAL(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Multiplicative()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Exponential();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Multiply:
                Temp = new AST::MULTIPLY(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Divide:
                Temp = new AST::DIVIDE(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Modulus:
                Temp = new AST::MODULUS(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Additive()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Multiplicative();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Add:
                Temp = new AST::ADD(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Subtract:
                Temp = new AST::SUBTRACT(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Shift()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Additive();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Shift_Left:
                Temp = new AST::SHIFT_LEFT(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Shift_Right:
                Temp = new AST::SHIFT_RIGHT(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Relational()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Shift();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Less:
                Temp = new AST::LESS(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Greater:
                Temp = new AST::GREATER(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Less_Equal:
                Temp = new AST::LESS_EQUAL(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Greater_Equal:
                Temp = new AST::GREATER_EQUAL(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Equality()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Relational();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Equal:
                Temp = new AST::EQUAL(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Not_Equal:
                Temp = new AST::NOT_EQUAL(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::BitwiseAND()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = Equality();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Bit_AND:
                Temp = new AST::BIT_AND(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Bit_NAND:
                Temp = new AST::BIT_NAND(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::BitwiseXOR()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = BitwiseAND();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Bit_XOR:
                Temp = new AST::BIT_XOR(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Bit_XNOR:
                Temp = new AST::BIT_XNOR(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::BitwiseOR()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = BitwiseXOR();
    if(!Node) return 0;

    while(Token.Type != TOKEN::TYPE::Unknown){
        switch(Token.Type){
            case TOKEN::TYPE::Bit_OR:
                Temp = new AST::BIT_OR(Token.Line, Scanner.Filename);
                break;
            case TOKEN::TYPE::Bit_NOR:
                Temp = new AST::BIT_NOR(Token.Line, Scanner.Filename);
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

AST::EXPRESSION* PARSER::Expression()
{
    AST::EXPRESSION* Temp;
    AST::EXPRESSION* Node;

    Node = BitwiseOR();
    if(!Node) return 0;

    if(Token.Type == TOKEN::TYPE::TernaryIf){
        Temp = new AST::CONDITIONAL(Token.Line, Scanner.Filename);
        GetToken();

        Temp->Left = Node;
        Node = Temp;

        Node->Right = Cast();
        if(!Node->Right){
            Error("Cast expected");
            delete Node;
            return 0;
        }

        if(Token.Type != TOKEN::TYPE::TernaryElse){
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
        Node->Right->Next->Prev = Node->Right;
    }
    return Node;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PARSER::TypeIdentifier()
{
    AST::EXPRESSION* Node = Identifier();
    if(!Node) return 0;

    AST::EXPRESSION* Temp;

    while(Token.Type != TOKEN::TYPE::Unknown){
        if(Token.Type == TOKEN::TYPE::AccessMember){
            Temp = new AST::ACCESSMEMBER(Token.Line, Scanner.Filename);
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

AST::ASSIGNMENT* PARSER::Initialiser(std::string& Identifier)
{
    AST::ASSIGNMENT* Node;

    switch(Token.Type){
        case TOKEN::TYPE::Assign:
            Node = new AST::ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Raw_Assign:
            Node = new AST::RAW_ASSIGN(Token.Line, Scanner.Filename);
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
    Node->Left       = new AST::IDENTIFIER(Node->Source.Line, Scanner.Filename);
    ((AST::IDENTIFIER*)Node->Left)->Name = Identifier;
    return Node;
}
//------------------------------------------------------------------------------

AST::DEFINITION::ARRAY* PARSER::ArrayDefinition()
{
    AST::DEFINITION::ARRAY* Array;

    if(Token.Type != TOKEN::TYPE::OpenSquare) return 0;
    GetToken();

    Array = new AST::DEFINITION::ARRAY;

    Array->Size = Expression();

    if(Token.Type != TOKEN::TYPE::CloseSquare){
        Error("']' expected");
        delete Array;
        return 0;
    }
    GetToken();
    return Array;
}
//------------------------------------------------------------------------------

AST::DEFINITION* PARSER::DefParameter()
{
    AST::DEFINITION* Node;

    switch(Token.Type){
        case TOKEN::TYPE::Pin:
            Node = new AST::PIN_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Net:
            Node = new AST::NET_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Byte:
            Node = new AST::BYTE_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Char:
            Node = new AST::CHAR_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Num:
            Node = new AST::NUM_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Func:
            Node = new AST::FUNCPTR_DEFINITION(Token.Line, Scanner.Filename);
            GetToken();
            break;
        case TOKEN::TYPE::Identifier:{
            auto Temp = new AST::CLASS_INSTANCE(Token.Line, Scanner.Filename);
            Temp->ClassName = TypeIdentifier();
            Node = Temp;
            if(!Temp->ClassName){
                Error("Invalid identifier or class name specifier");
                delete Node;
                return 0;
            }
            break;
        }

        default:
            return 0;
    }

    if(Token.Type == TOKEN::TYPE::Identifier){
        Node->Identifiers = new AST::DEFINITION::IDENTIFIER();
        Node->Identifiers->Identifier = Token.Data;
        GetToken();

    }else if(Node->Type == AST::BASE::TYPE::Class_Instance){
        auto Temp = ((AST::CLASS_INSTANCE*)Node);
        if(
            Temp->ClassName &&
            Temp->ClassName->Type == AST::BASE::TYPE::Identifier
        ){
            Node = new AST::AUTO_DEFINITION(Temp->Source.Line, Temp->Source.Filename);
            Node->Identifiers = Temp->Identifiers;
            Temp->Identifiers = 0;
            delete Temp;

        }else{
            Error("Identifier expected");
            delete Node;
            return 0;
        }
    }else{
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    assert(Node->Identifiers);
    while(Token.Type == TOKEN::TYPE::OpenSquare){
        GetToken();
        if(Token.Type != TOKEN::TYPE::CloseSquare){
            Error("] expected");
            delete Node;
            return 0;
        }
        GetToken();
        auto Temp = new AST::DEFINITION::ARRAY();
        Temp->Next = Node->Identifiers->Array;
        Node->Identifiers->Array = Temp;
    }
    return Node;
}
//------------------------------------------------------------------------------

AST::DEFINITION* PARSER::DefParameterList()
{
    AST::DEFINITION* Head;
    AST::DEFINITION* Node;

    Head = Node = DefParameter();
    if(!Node) return 0;

    while(Token.Type == TOKEN::TYPE::Comma){
        GetToken();
        Node->Next = DefParameter();
        if(Node->Next) Node->Next->Prev = Node;
        Node = (AST::DEFINITION*)Node->Next;
        if(!Node){
            Error("DefParameter expected");
            delete Head;
            return 0;
        }
    }
    return Head;
}
//------------------------------------------------------------------------------

AST::DEFINITION::IDENTIFIER* PARSER::IdentifierList()
{
    AST::DEFINITION::IDENTIFIER* Head;
    AST::DEFINITION::IDENTIFIER* Node;
    AST::DEFINITION::ARRAY     * Array;

    if(Token.Type != TOKEN::TYPE::Identifier) return 0;

    Head = Node = new AST::DEFINITION::IDENTIFIER;
    Node->Identifier = Token.Data;
    GetToken();

    Node->Array = Array = ArrayDefinition();
    while(Array){
        Array->Next = ArrayDefinition();
        Array = Array->Next;
    }

    if(Token.Type == TOKEN::TYPE::OpenRound){ // Function definition
        GetToken();

        Node->Function   = true;
        Node->Parameters = DefParameterList();
        if(error){
            delete Head;
            return 0;
        }
        if(Token.Type != TOKEN::TYPE::CloseRound){
            Error(") expected");
            delete Head;
            return 0;
        }
        GetToken();
        if(Token.Type != TOKEN::TYPE::OpenCurly){
            Error("{ expected");
            delete Head;
            return 0;
        }
        GetToken();
        Node->FunctionBody = Statements();
        if(Token.Type != TOKEN::TYPE::CloseCurly){
            Error("} expected");
            delete Head;
            return 0;
        }
        GetToken();
        return Head;
    }

    Node->Initialiser = Initialiser(Node->Identifier);

    while(Token.Type == TOKEN::TYPE::Comma){
        GetToken();

        if(Token.Type != TOKEN::TYPE::Identifier){
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
    if(Token.Type != TOKEN::TYPE::Semicolon){
        Error("; expected");
        delete Head;
        return 0;
    }
    GetToken();
    return Head;
}
//------------------------------------------------------------------------------

AST::DEFINITION* PARSER::Definition()
{
    AST::DEFINITION::DIRECTION Direction =
        AST::DEFINITION::DIRECTION::Inferred;

    if(Token.Type == TOKEN::TYPE::Input){
        Direction = AST::DEFINITION::DIRECTION::Input;
        GetToken();

    }else if(Token.Type == TOKEN::TYPE::Output){
        Direction = AST::DEFINITION::DIRECTION::Output;
        GetToken();
    }

    AST::DEFINITION* Node = 0;
    switch(Token.Type){
        case TOKEN::TYPE::Pin:
            Node = new AST::PIN_DEFINITION    (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Net:
            Node = new AST::NET_DEFINITION    (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Void:
            Node = new AST::VOID_DEFINITION   (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Auto:
            Node = new AST::AUTO_DEFINITION   (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Byte:
            Node = new AST::BYTE_DEFINITION   (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Char:
            Node = new AST::CHAR_DEFINITION   (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Num:
            Node = new AST::NUM_DEFINITION    (Token.Line, Scanner.Filename); break;
        case TOKEN::TYPE::Func:
            Node = new AST::FUNCPTR_DEFINITION(Token.Line, Scanner.Filename); break;
        default:
            if(Direction != AST::DEFINITION::DIRECTION::Inferred) Error("Type name expected");
            return 0;
    }
    Node->Direction = Direction;
    GetToken();

    ParameterList(Node->Parameters);

    if(!Node->Parameters.empty()){
        if(Node->Type == AST::BASE::TYPE::Void_Definition){
            Error("Void type does not take parameters");
            delete Node;
            return 0;
        }
        if(Node->Type == AST::BASE::TYPE::Auto_Definition){
            Error("Auto type does not take parameters");
            delete Node;
            return 0;
        }
        if(Node->Type == AST::BASE::TYPE::FuncPtr_Definition){
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
    if(Node->Type == AST::BASE::TYPE::Void_Definition){
        if(!Node->Identifiers->Function){
            Error("Only functions can have \"void\" type.\n");
        }
    }
    return Node;
}
//------------------------------------------------------------------------------

bool PARSER::ValidNamespaceSpecifier(AST::EXPRESSION* Node)
{
    // Only a few operations are invalid:
    switch(Node->Type){
        case AST::BASE::TYPE::AccessMember:
        case AST::BASE::TYPE::Identifier:
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

bool PARSER::ValidTypeSpecifier(AST::EXPRESSION* Node)
{
    // Only a few operations are invalid:
    switch(Node->Type){
        case AST::BASE::TYPE::AccessMember:
        case AST::BASE::TYPE::Identifier:
            break;

        case AST::BASE::TYPE::FunctionCall:
            if(Node->Left){ // Don't test the parameter list
                if(!ValidTypeSpecifier(Node->Left)) return false;
            }
            return true;

        default:
            return false;
    }

    // Only the root may be a function call, and test recursively
    if(Node->Left){
        if(Node->Left->Type == AST::BASE::TYPE::FunctionCall) return false;
        if(!ValidTypeSpecifier(Node->Left)) return false;
    }
    if(Node->Right){
        if(Node->Right->Type == AST::BASE::TYPE::FunctionCall) return false;
        if(!ValidTypeSpecifier(Node->Right)) return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool PARSER::ValidLHS(AST::EXPRESSION* Node)
{
    switch(Node->Type){
        case AST::BASE::TYPE::AccessMember:
        case AST::BASE::TYPE::AccessMemberSafe:
        case AST::BASE::TYPE::AccessAttribute:
        case AST::BASE::TYPE::Identifier:
        case AST::BASE::TYPE::VectorConcatenate:
        case AST::BASE::TYPE::ArrayConcatenate:
            break;

        case AST::BASE::TYPE::Slice:
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
    if(Node->Type == AST::BASE::TYPE::ArrayConcatenate){
        foreach(Element, ((AST::ARRAYCONCATENATE*)Node)->Elements){
            if(!ValidLHS(*Element)) return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Other()
{
    AST::EXPRESSION* Expr = Postfix();
    if(!Expr) return 0;

    if(Token.Type == TOKEN::TYPE::Comma || Token.Type == TOKEN::TYPE::Semicolon){
        if(
            Expr->Type != AST::BASE::TYPE::FunctionCall &&
            Expr->Type != AST::BASE::TYPE::Increment    &&
            Expr->Type != AST::BASE::TYPE::Decrement
        ){
            Error("Unexpected end-of-statement");
            delete Expr;
            return 0;
        }
        if(Token.Type == TOKEN::TYPE::Semicolon){
            Expr->Next = new AST::FENCE(
                Token.Line, Scanner.Filename
            );
            if(Expr->Next) Expr->Next->Prev = Expr;
        }
        GetToken();
        return Expr;
    }

    if(Token.Type == TOKEN::TYPE::AccessMemberPush){ // Namespace push
        if(!ValidNamespaceSpecifier(Expr)){
            Error("Invalid Name-space specifier expression");
            delete Expr;
            return 0;
        }
        AST::NAMESPACE_PUSH* Namespace = new AST::NAMESPACE_PUSH(
            Token.Line, Scanner.Filename
        );
        GetToken();

        Namespace->Namespace  = Expr;
        Namespace->Statements = Statements();

        if(Token.Type != TOKEN::TYPE::CloseCurly){
            Error("} expected");
            delete Namespace;
            return 0;
        }
        GetToken();
        return Namespace;
    }

    if(Token.Type == TOKEN::TYPE::OpenAngle || Token.Type == TOKEN::TYPE::Identifier){
        if(!ValidTypeSpecifier(Expr)){
            Error("Invalid type specifier expression");
            delete Expr;
            return 0;
        }

        AST::CLASS_INSTANCE* Def = new AST::CLASS_INSTANCE(Token.Line, Scanner.Filename);
        if(Expr->Type == AST::BASE::TYPE::FunctionCall){
            Def->ClassName  = Expr->Left;
            Def->Parameters = ((AST::FUNCTIONCALL*)Expr)->Parameters;
            Expr->Left  = 0;
            ((AST::FUNCTIONCALL*)Expr)->Parameters.clear();
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
        case TOKEN::TYPE::Assign:
            Assign = new AST::ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Raw_Assign:
            Assign = new AST::RAW_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Append_Assign:
            Assign = new AST::APPEND_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Add_Assign:
            Assign = new AST::ADD_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Subtract_Assign:
            Assign = new AST::SUBTRACT_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Multiply_Assign:
            Assign = new AST::MULTIPLY_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Divide_Assign:
            Assign = new AST::DIVIDE_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Modulus_Assign:
            Assign = new AST::MODULUS_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Exponential_Assign:
            Assign = new AST::EXPONENTIAL_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::AND_Assign:
            Assign = new AST::AND_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::OR_Assign:
            Assign = new AST::OR_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::XOR_Assign:
            Assign = new AST::XOR_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Shift_Left_Assign:
            Assign = new AST::SHIFT_LEFT_ASSIGN(Token.Line, Scanner.Filename);
            break;
        case TOKEN::TYPE::Shift_Right_Assign:
            Assign = new AST::SHIFT_RIGHT_ASSIGN(Token.Line, Scanner.Filename);
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
        case TOKEN::TYPE::Comma: GetToken(); break;
        case TOKEN::TYPE::Semicolon: break;
        default:
            Error(", or ; expected");
            delete Assign;
            return 0;
    }
    return Assign;
}
//------------------------------------------------------------------------------

AST::IF_STATEMENT* PARSER::IfStatement()
{
    if(Token.Type != TOKEN::TYPE::If) return 0;
    AST::IF_STATEMENT* Node = new AST::IF_STATEMENT(
        Token.Line, Scanner.Filename
    );
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenRound){
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

    if(Token.Type != TOKEN::TYPE::CloseRound){
        Error(") expected");
        delete Node;
        return 0;
    }
    GetToken();

    Node->TrueStatements = StatementBlock();

    if(Token.Type == TOKEN::TYPE::Else){
        GetToken();
        Node->FalseStatements = StatementBlock();
    }

    return Node;
}
//------------------------------------------------------------------------------

AST::WHILE_LOOP* PARSER::WhileLoop()
{
    if(Token.Type != TOKEN::TYPE::While) return 0;
    AST::WHILE_LOOP* Node = new AST::WHILE_LOOP(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenRound){
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

    if(Token.Type != TOKEN::TYPE::CloseRound){
        Error(") expected");
        delete Node;
        return 0;
    }
    GetToken();

    Node->Statements = StatementBlock();
    return Node;
}
//------------------------------------------------------------------------------

AST::LOOP_LOOP* PARSER::LoopLoop()
{
    if(Token.Type != TOKEN::TYPE::Loop) return 0;
    AST::LOOP_LOOP* Node = new AST::LOOP_LOOP(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type == TOKEN::TYPE::OpenRound){
        GetToken();

        Node->Count = Expression();
        if(!Node->Count){
            Error("Expression expected");
            delete Node;
            return 0;
        }

        if(Token.Type != TOKEN::TYPE::CloseRound){
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

AST::FOR_LOOP* PARSER::ForLoop()
{
    if(Token.Type != TOKEN::TYPE::For) return 0;
    AST::FOR_LOOP* Node = new AST::FOR_LOOP(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenRound){
        Error("( expected");
        delete Node;
        return 0;
    }
    GetToken();

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    Node->Identifier = Token.Data;
    GetToken();

    if(Token.Type != TOKEN::TYPE::In){
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

    if(Token.Type != TOKEN::TYPE::CloseRound){
        Error(") expected");
        delete Node;
        return 0;
    }
    GetToken();

    Node->Statements = StatementBlock();
    return Node;
}
//------------------------------------------------------------------------------

AST::IMPORT* PARSER::Import()
{
    if(Token.Type != TOKEN::TYPE::Import) return 0;
    AST::IMPORT* Node = new AST::IMPORT(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type != TOKEN::TYPE::String){
        Error("File name expected");
        delete Node;
        return 0;
    }
    Node->File = Token.Data;
    GetToken();

    if(Token.Type == TOKEN::TYPE::As){
        GetToken();

        if(Token.Type != TOKEN::TYPE::Identifier){
            Error("Identifier expected");
            delete Node;
            return 0;
        }
        Node->Namespace = Token.Data;
        GetToken();
    }

    if(Token.Type != TOKEN::TYPE::Semicolon){
        Error("; expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::SWITCH* PARSER::Switch()
{
    if(Token.Type != TOKEN::TYPE::Switch) return 0;
    AST::SWITCH* Node = new AST::SWITCH(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenRound){
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
    if(Token.Type != TOKEN::TYPE::CloseRound){
        Error(") expected");
        delete Node;
        return 0;
    }
    GetToken();
    if(Token.Type != TOKEN::TYPE::OpenCurly){
        Error("{ expected");
        delete Node;
        return 0;
    }
    GetToken();

    AST::SWITCH::CASE* Case;
    AST::SWITCH::CASE* Tail = 0;

    while(Token.Type == TOKEN::TYPE::Case){
        Case = new AST::SWITCH::CASE;
        if(Tail) Tail->Next  = Case;
        else     Node->Cases = Case;
        Tail = Case;
        GetToken();

        if(Token.Type != TOKEN::TYPE::OpenRound){
            Error("'(' expected");
            delete Node;
            return 0;
        }
        GetToken();

        if(!ExpressionList(Case->Expressions)){
            Error("Expression list expected");
            delete Node;
            return 0;
        }
        if(Token.Type != TOKEN::TYPE::CloseRound){
            Error("')' expected");
            delete Node;
            return 0;
        }
        GetToken();

        Case->Statements = StatementBlock();
    }

    if(Token.Type == TOKEN::TYPE::Default){
        GetToken();
        Node->Default = StatementBlock();
    }

    if(Token.Type != TOKEN::TYPE::CloseCurly){
        Error("} expected");
        delete Node;
        return 0;
    }
    GetToken();
    return Node;
}
//------------------------------------------------------------------------------

AST::ALIAS* PARSER::Alias()
{
    if(Token.Type != TOKEN::TYPE::Alias) return 0;
    GetToken();

    AST::ALIAS* Node = new AST::ALIAS(Token.Line, Scanner.Filename);

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    Node->Identifier = Token.Data;
    GetToken();

    if(Token.Type != TOKEN::TYPE::Assign){
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

    if(Token.Type != TOKEN::TYPE::Semicolon){
        Error("; expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::GROUP* PARSER::Group()
{
    if(Token.Type != TOKEN::TYPE::Group) return 0;
    GetToken();

    AST::GROUP* Node = new AST::GROUP(Token.Line, Scanner.Filename);
    if(Token.Type == TOKEN::TYPE::OpenAngle){
        Node->Attributes = AttributeList();
    }

    if(Token.Type == TOKEN::TYPE::Identifier){
        Node->Identifier = Token.Data;
        GetToken();
    }

    if(Token.Type != TOKEN::TYPE::OpenCurly){
        Error("{ expected");
        delete Node;
        return 0;
    }
    GetToken();

    Node->Body = Statements();

    if(Token.Type != TOKEN::TYPE::CloseCurly){
        Error("} expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::JUMP* PARSER::Jump()
{
    AST::JUMP* Node;

    switch(Token.Type){
        case TOKEN::TYPE::Return:
            Node = new AST::JUMP(
                Token.Line, Scanner.Filename, AST::JUMP::JUMP_TYPE::Return
            );
            break;
        case TOKEN::TYPE::Break:
            Node = new AST::JUMP(
                Token.Line, Scanner.Filename, AST::JUMP::JUMP_TYPE::Break
            );
            break;
        case TOKEN::TYPE::Continue:
            Node = new AST::JUMP(
                Token.Line, Scanner.Filename, AST::JUMP::JUMP_TYPE::Continue
            );
            break;
        default:
            return 0;
    }
    GetToken();

    Node->Expression = Expression();

    if(Token.Type != TOKEN::TYPE::Semicolon){
        Error("; expected");
        delete Node;
        return 0;
    }
    GetToken();

    return Node;
}
//------------------------------------------------------------------------------

AST::RTL* PARSER::RTL()
{
    if(Token.Type != TOKEN::TYPE::RTL) return 0;
    AST::RTL* Node = new AST::RTL(Token.Line, Scanner.Filename);
    GetToken();

    ParameterList(Node->Parameters);
    Node->Statements = StatementBlock();
    return Node;
}
//------------------------------------------------------------------------------

AST::FSM* PARSER::FSM()
{
    if(Token.Type != TOKEN::TYPE::FSM) return 0;
    AST::FSM* Node = new AST::FSM(Token.Line, Scanner.Filename);
    GetToken();

    ParameterList(Node->Parameters);
    Node->Statements = StatementBlock();
    return Node;
}
//------------------------------------------------------------------------------

AST::HDL* PARSER::HDL()
{
    if(Token.Type != TOKEN::TYPE::HDL) return 0;
    AST::HDL* Node = new AST::HDL(Token.Line, Scanner.Filename);
    GetToken();

    if(Token.Type != TOKEN::TYPE::OpenRound){
        Error("( expected");
        delete Node;
        return 0;
    }
    GetToken();

    AST::EXPRESSION* File = Node->Files = String();
    if(File){
        while(Token.Type == TOKEN::TYPE::Comma){
            GetToken();
            File->Next = String();
            if(File->Next) File->Next->Prev = File;
            File = (AST::EXPRESSION*)File->Next;
            if(!File){
                Error("File name string expected");
                delete Node;
                return 0;
            }
        }
    }
    if(Token.Type != TOKEN::TYPE::CloseRound){
        Error(") expected");
        delete Node;
        return 0;
    }
    GetToken();

    if(Token.Type != TOKEN::TYPE::Identifier){
        Error("Identifier expected");
        delete Node;
        return 0;
    }
    Node->Identifier = Token.Data;
    GetToken();

    if(Token.Type == TOKEN::TYPE::OpenRound){
        GetToken();

        AST::BASE* Tail = 0;
        AST::BASE* Temp = Other(); // Also returns assignments
        while(Temp){
            if(!Temp->IsAssignment()){
                Error("Assignment expected");
                delete Temp;
                delete Node;
                return 0;
            }
            if(Tail){
                Tail->Next = Temp;
                Temp->Prev = Tail;
            }else{
                Node->Parameters = (AST::ASSIGNMENT*)Temp;
            }
            Tail = Temp;
            while(Tail->Next) Tail = Tail->Next;
            Temp = Other();
        }
        if(Token.Type != TOKEN::TYPE::CloseRound){
            Error(") expected");
            delete Node;
            return 0;
        }
        GetToken();
    }
    if(Token.Type != TOKEN::TYPE::OpenCurly){
        Error("{ expected");
        delete Node;
        return 0;
    }
    GetToken();

    AST::DEFINITION* Tail = 0;
    AST::DEFINITION* Temp = Definition();
    while(Temp){
        if(Tail){
            Tail->Next  = Temp;
            Temp->Prev  = Tail;
        }else{
            Node->Ports = Temp;
        }
        Tail = Temp;
        Temp = Definition();
    }

    if(Token.Type != TOKEN::TYPE::CloseCurly){
        Error("} expected");
        delete Node;
        return 0;
    }
    GetToken();
    return Node;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Statement()
{
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

    if(Token.Type == TOKEN::TYPE::Semicolon){
        Node = new AST::FENCE(Token.Line, Scanner.Filename);
        GetToken();
        return Node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::BASE* PARSER::Statements()
{
    AST::BASE* Head = 0;
    AST::BASE* Tail = 0;
    AST::BASE* Node = 0;

    while(!error && (Token.Type != TOKEN::TYPE::Unknown)){
        Node = Statement();
        if(error) break;
        if(!Node) return Head;

        if(Tail){
            Tail->Next = Node;
            Node->Prev = Tail;
        }else{
            Head       = Node;
        }
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

AST::BASE* PARSER::StatementBlock()
{
    AST::BASE* Node;

    if(Token.Type == TOKEN::TYPE::OpenCurly){
        GetToken();
        Node = Statements();
        if(Token.Type != TOKEN::TYPE::CloseCurly){
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

AST::BASE* PARSER::Run(const char* Filename)
{
    Debug.Print("Building AST for ");
    Debug.Print(Filename);
    Debug.Print("...\n");

    error = false;

    if(!Scanner.Open(Filename)) return 0;
    GetToken();

    AST::BASE* AST = Statements();
    if(Token.Type != TOKEN::TYPE::Unknown){
        Error("Unexpected token");
        if(AST) delete AST;
        return 0;
    }

    #ifdef DEBUG
        Debug.Print(ANSI_FG_GREEN "\nDisplaying AST of ");
        Debug.Print(Filename);
        Debug.Print(" -------------------------------------\n\n" ANSI_RESET);
        if(AST) AST->Display();
        else    Debug.Print("AST is empty\n");

        AST->Validate();
    #endif
    return AST;
}
//------------------------------------------------------------------------------

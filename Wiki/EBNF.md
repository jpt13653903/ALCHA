[TOC]

# EBNF
In the case of ambiguity, the first match is valid.  Although every effort has been made to make this EBNF as accurate as possible, the actual implementation may differ in subtle ways.

## Scanner
The scanner reads UTF-8 formatted files.  The UTF-8 formatting is retained, even though the input stream is searched for Unicode space and newline characters.  At this stage, white-space and comments are ignored, adjacent strings are concatenated and escape sequences within strings are evaluated.

    :::EBNF
    WhiteSpace = Space | Newline | LineComment | BlockComment;
    
    Space   = ? U+0020 ? | ? U+0009 ? | ? U+00A0 ? | ? U+1680 ? | ? U+180E ? 
            | ? U+2000 ? | ? U+2001 ? | ? U+2002 ? | ? U+2003 ? | ? U+2004 ? 
            | ? U+2005 ? | ? U+2006 ? | ? U+2007 ? | ? U+2008 ? | ? U+2009 ? 
            | ? U+200A ? | ? U+202F ? | ? U+200B ? | ? U+200C ? | ? U+200D ? 
            | ? U+205F ? | ? U+2060 ? | ? U+3000 ? | ? U+FEFF ? ;
    Newline = ? U+0A U+0D ? | ? U+0D U+0A ? | ? U+0A ? | ? U+0D ? | ? U+0B ? 
            | ? U+0C ? | ? U+85 ? | ? U+2028 ? | ? U+2029 ? ;
           
    LineComment  = "//" {? Any byte sequence ? -Newline} Newline;
    BlockComment = "/*" {? Any byte sequence ? -"*/"} "*/";
    Byte         = ? Any input byte that does not belong to anything else ?;
    (*----------------------------------------------------------------------------*)
    
    Token      = String | Literal | Keyword | Identifier | Operator;
    
    Keyword    = "target" | "void"  | "pin"     | "sig"  | "clk"    | "int"    
               | "rat"    | "float" | "complex" | "in"   | "out"    | "signed" 
               | "group"  | "class" | "if"      | "else" | "for"    | "while"  
               | "loop"  | "switch" | "default" | "case" | "import" | "as";
    
    Operator   = "++"  | "--"  | "'"  | "->" | "#"  | "."  | ".{" | "&"  | "~&" 
               | "|"   | "~|"  | "^"  | "~^" | "~"  | ":"  | "\"  | "@{" | "+" 
               | "-"   | "*"   | "/"  | "%"  | "<<" | ">>" | "<"  | ">"  | "<=" 
               | ">="  | "=="  | "!=" | "!"  | "&&" | "||" | "?"  | "="  | ":=" 
               | "@="  | "+="  | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^=" 
               | "<<=" | ">>=" | "("  | ")"  | "["  | "]"  | "{"  | "}"  
               | ","   | ";"   ;
    
    Literal        = ("0b" Binary) | ("0o" Octal) | ("0x" Hexadecimal) | Decimal;
    
    Identifier     = (NonDigit {D | NonDigit}) -Keyword; 
    
    String         = '"' {EscapeSequence | (Byte - '"')} '"' [String];
    
    EscapeSequence = "\n" | "\t" | "\v" | "\b" | "\r" | "\f" 
                   | "\a" | "\\" | "\?" | "\'" | '\"' 
                   | ("\x" H H) | ("\u" H H H H) | ("\U" H H H H H H H H)
                   | ("\" O {O});
    
    (* Either the left of the ".", or the right of the "." must have a digit *)
    (* Within literals, the "_" character is ignored *)
    Binary      = {B} ["." {B}] [("p" | "P"            ) Exponent] ["i" | "j"];
    Octal       = {O} ["." {O}] [("p" | "P"            ) Exponent] ["i" | "j"];
    Hexadecimal = {H} ["." {H}] [("p" | "P"            ) Exponent] ["i" | "j"];
    Decimal     = {D} ["." {D}] [("p" | "P" | "e" | "E") Exponent] ["i" | "j"];
    Exponent    = ["-" | "+"] {D};
    (*----------------------------------------------------------------------------*)
    
    B = "0" | "1";
    O = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7";
    D = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
    H = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" 
      | "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F"; 
    
    NonDigit = ?"a".."z"? | ?"A".."Z"? | "_" 
             | (? Any Unicode >= U+80 ? -Space -Newline);
    (*----------------------------------------------------------------------------*)

## Parser
The parser takes tokens from the scanner and builds an abstract syntax tree.  Nothing is evaluated at this point, not even definitions.

### Definitions

    :::EBNF
    Module     = Statements;
    Statements = Statement {Statement};
    Statement  = TargetDefinition | Definition | Other;
    
    TargetDefinition = "target" AttributeList ";";
    Definition       = ["signed"] ["in" | "out"] ["signed"] 
                       ("void" | "pin" | "sig"   | "clk" | 
                        "int"  | "rat" | "float" | "complex") 
                       [FP_Cast] [AttributeList] IdentifierList;
    
    (* The result from "Concatenation" is checked for syntactic validity before
       use as a class name or assignment target *)
    Other = Concatenation (
             (";"                              ) | (* Function call  *)
             ([AttributeList] IdentifierList   ) | (* Class instance *)
             (AssignmentOperator Expression ";")   (* Assignment     *)
            );
    AssignmentOperator = "="  | ":=" | "@=" | "+=" | "-="  | "*="  | "/=" 
                       | "%=" | "&=" | "|=" | "^=" | "<<=" | ">>=" ; 
    
    IdentifierList = Identifier {ArrayDefinition} (
                      ("(" [DefParameterList] ")" "{" Statements "}"   ) |
                      ({"," Identifier {ArrayDefinition}} ";")
                     );
    ArrayDefinition  = "[" [Expression] "]";
    DefParameterList = Identifier {"," Identifier};
    
    AttributeList       = "<" AttributeAssignment {, AttributeAssignment} ">";
    AttributeAssignment = Identifier "=" String;
    
    (*----------------------------------------------------------------------------*)

### Expressions

    :::EBNF
    Expression     = LogicalOR      [ "?" Primary ":" Primary];
    LogicalOR      = LogicalAND     { "||"                      LogicalAND    };
    LogicalAND     = BitwiseOR      { "&&"                      BitwiseOR     };
    BitwiseOR      = BitwiseXOR     {("|"  | "~|")              BitwiseXOR    };
    BitwiseXOR     = BitwiseAND     {("^"  | "~^")              BitwiseAND    };
    BitwiseAND     = Equality       {("&"  | "~&")              Equality      };
    Equality       = Relational     {("==" | "!=")              Relational    };
    Relational     = Shift          {("<"  | ">" | "<=" | ">=") Shift         };
    Shift          = Additive       {("<<" | ">>")              Additive      };
    Additive       = Multiplicative {("+"  | "-" )              Multiplicative};
    Multiplicative = Replication    {("*"  | "/" | "%")         Replication   };
    
    Replication   = Concatenation ["\" Primary];
    Concatenation = Cast {":" Cast};
    
    Cast       = Reduction [FP_Cast];
    FP_Cast    = "'" (Identifier | Literal | ("(" Primary ["," Primary] ")"));
    Reduction  = ["&" | "~&" | "|" | "~|" | "^" | "~^" | "!"] Array;
    
    Array = ("@{" Expression {"," Expression} "}")
          | (Unary ["->" Unary ["#" Unary]]);
    
    Unary   = {"-" | "~"} Postfix;
    Postfix = Primary {
               "["  SliceList      "]" | (* Slice *)
               "(" [ParameterList] ")" | (* Function call *)
               "."  Identifier         | (* Member reference *)
               "++"                    | (* Increment *)
               "--"                      (* Decrement *)
              };
    
    Primary = Identifier | Literal | String | ("(" Expression ")");
    
    SliceList     = ":" | (Array {"," Array});
    ParameterList = Expression {"," Expression};
    (*----------------------------------------------------------------------------*)




[TOC]

# EBNF
In the case of ambiguity, the first match is valid.  Although every effort has been made to make this EBNF as accurate as possible, the actual implementation may differ in subtle ways.

## Scanner
The scanner reads UTF-8 formatted files.  The UTF-8 formatting is retained, even though the input stream is searched for Unicode space and newline characters.  At this stage, white-space and comments are ignored, adjacent strings are concatenated and escape sequences within strings are evaluated.

[[include repo=doccode path=EBNF/Scanner.ebnf]]

## Parser
The parser takes tokens from the scanner and builds an abstract syntax tree.  Nothing is evaluated at this point, not even definitions.

### Definitions

[[include repo=doccode path=EBNF/Declarations.ebnf]]

### Statements

[[include repo=doccode path=EBNF/Statements.ebnf]]

### Expressions

[[include repo=doccode path=EBNF/Expressions.ebnf]]


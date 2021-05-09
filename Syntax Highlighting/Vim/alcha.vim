" Vim syntax file
" Language:	ALCHA
" Maintainer:	J Taylor <tyljoh010@myuct.ac.za>
" Last Change:	2018-07-29

if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Operators
syn match AlchaPunctuator "'"
syn match AlchaOperator   "-"
syn match AlchaOperator   "--"
syn match AlchaOperator   "#"
syn match AlchaOperator   "##"
syn match AlchaOperator   "%"
syn match AlchaOperator   "%="
syn match AlchaOperator   "&"
syn match AlchaOperator   "&&"
syn match AlchaOperator   "&&&"
syn match AlchaOperator   "&="
syn match AlchaBracket    "("
syn match AlchaBracket    ")"
syn match AlchaOperator   "\*"
syn match AlchaOperator   "\*\*"
syn match AlchaOperator   "\*\*="
syn match AlchaOperator   "\*="
syn match AlchaPunctuator ","
syn match AlchaOperator   "/"
syn match AlchaOperator   "/="
syn match AlchaOperator   ":"
syn match AlchaBracket    ":("
syn match AlchaBracket    ":\["
syn match AlchaOperator   ":="
syn match AlchaPunctuator ";"
syn match AlchaOperator   "?"
syn match AlchaOperator   "?:"
syn match AlchaPunctuator "?\."
syn match AlchaOperator   "@"
syn match AlchaOperator   "\!"
syn match AlchaOperator   "\!="
syn match AlchaOperator   "\$"
syn match AlchaOperator   "\$("
syn match AlchaPunctuator "\."
syn match AlchaOperator   "\.\."
syn match AlchaBracket    "\.{"
syn match AlchaBracket    "\["
syn match AlchaBracket    "\[\*"
syn match AlchaBracket    "\[="
syn match AlchaBracket    "\[->"
syn match AlchaBracket    "\]"
syn match AlchaOperator   "\^"
syn match AlchaOperator   "\^="
syn match AlchaOperator   "\~"
syn match AlchaOperator   "\~&"
syn match AlchaOperator   "\~\^"
syn match AlchaOperator   "\~|"
syn match AlchaOperator   "\~="
syn match AlchaOperator   "`"
syn match AlchaBracket    "{"
syn match AlchaOperator   "|"
syn match AlchaOperator   "||"
syn match AlchaOperator   "|="
syn match AlchaOperator   "|=>"
syn match AlchaOperator   "|->"
syn match AlchaBracket    "}"
syn match AlchaOperator   "+"
syn match AlchaOperator   "++"
syn match AlchaOperator   "+="
syn match AlchaOperator   "<"
syn match AlchaOperator   "<<"
syn match AlchaOperator   "<<="
syn match AlchaOperator   "<="
syn match AlchaOperator   "="
syn match AlchaOperator   "-="
syn match AlchaOperator   "=="
syn match AlchaOperator   ">"
syn match AlchaOperator   ">="
syn match AlchaOperator   ">>"
syn match AlchaOperator   ">>="

" Literal     = (("0b" Binary) | ("0o" Octal) | ("0x" Hexadecimal) | Decimal) [Exponent] ["i" | "j"];
" Binary      = (B {B | "_"} ["." {B | "_"}]) | ("." B {B | "_"});
" Octal       = (O {O | "_"} ["." {O | "_"}]) | ("." O {O | "_"});
" Decimal     = (D {D | "_"} ["." {D | "_"}]) | ("." D {D | "_"});
" Hexadecimal = (H {H | "_"} ["." {H | "_"}]) | ("." H {H | "_"});
" Exponent    = ("p" | "P" | "e" | "E") ["-" | "+"] D {D};
syn match AlchaPrefix contained "0b\|0o\|0x"
syn match AlchaExp contained "[pPeE]_*\([+-]\?\(\d\|_\)\+\)"
syn match AlchaConstant "\<\d\(\d\|_\)*\(\.\(\d\|_\)\+\)\?\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"    contains=AlchaExp
syn match AlchaConstant "\<0b[01_]\+\(\.[01_]\+\)\?\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"           contains=AlchaExp,AlchaPrefix
syn match AlchaConstant "\<0o\(\o\|_\)\+\(\.\(\o\|_\)\+\)\?\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"   contains=AlchaExp,AlchaPrefix
syn match AlchaConstant "\<0x\(\x\|_\)\+\(\.\(\x\|_\)\+\)\?\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"   contains=AlchaExp,AlchaPrefix

" Keywords
syn keyword AlchaSpecial      __FILE__ __LINE__ __DATE__ __TIME__ __CLASS__ __FUNCTION__ __NAMESPACE__
syn keyword AlchaKeyword      true false
syn keyword AlchaType         void auto pin net byte char num string bool
syn keyword AlchaStructure    class enum struct group alias
syn keyword AlchaStorageClass input output
syn keyword AlchaStorageClass public private protected
syn keyword AlchaConditional  if else
syn keyword AlchaRepeat       for in while loop
syn keyword AlchaConditional  switch case default
syn keyword AlchaInclude      import as
syn keyword AlchaKeyword      return break continue goto
syn keyword AlchaKeyword      func inline
syn keyword AlchaKeyword      rtl fsm hdl
syn keyword AlchaKeyword      stimulus emulate sequence assert wait
syn keyword AlchaKeyword      this result finally

" Strings
syn match  AlchaEscape contained "\\\(n\|t\|v\|b\|r\|f\|a\|\\\|?\|'\|\"\|x\x\{2}\|u\x\{4}\|U\x\{8}\|\o\{1,11}\|&\(\d\|\a\)\{-};\)"
syn region AlchaString start=/"/ skip=/\\"/ end=/"/ contains=@Spell,AlchaEscape

" Comment
syn match  AlchaComment   "\/\/.*$" contains=@Spell
syn match  AlchaToDoComment "!!.*$" contains=@Spell
syn region AlchaComment                 start="\/\*" end="\*\/" contains=@Spell
syn region AlchaNestedComment contained start="\/+"  end="+\/"  contains=@Spell
syn region AlchaComment                 start="\/+"  end="+\/"  contains=@Spell,AlchaNestedComment

" Link the types
hi def link AlchaOperator      Operator
hi def link AlchaBracket       Delimiter
hi def link AlchaPunctuator    Delimiter
hi def link AlchaType          Type
hi def link AlchaStorageClass  StorageClass
hi def link AlchaStructure     Structure
hi def link AlchaConditional   Conditional
hi def link AlchaRepeat        Repeat
hi def link AlchaSpecial       Preproc
hi def link AlchaInclude       Include
hi def link AlchaPrefix        SpecialChar
hi def link AlchaExp           SpecialChar
hi def link AlchaConstant      Constant
hi def link AlchaKeyword       Keyword
hi def link AlchaEscape        SpecialChar
hi def link AlchaString        String
hi def link AlchaComment       Comment
hi def link AlchaNestedComment Comment
hi def link AlchaToDoComment   Todo

" Set the syntax type
let b:current_syntax = "alcha"


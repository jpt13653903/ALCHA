" Vim syntax file
" Language:	ALCHA
" Maintainer:	J Taylor <tyljoh010@myuct.ac.za>
" Last Change:	2016-02-15

if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Operators
syn match AlchaOperator "<<=\|>>=\|++\|--\|->\|\.{\|\~&\|\~|\|\~\^\|<<\|>>\|<=\|>=\|==\|!=\|&&\|||\|:=\|\~=\|+=\|-=\|\*=\|\/=\|%=\|&=\||=\|\^=\|'\|@\|\.\|&\||\|\^\|\~\|:\|\\\|+\|-\|\*\|\/\|%\|<\|>\|!\|?\|=\|(\|)\|\[\|\]\|{\|}\|,\|;"

" Constants
" Binary      = {B} ["." {B}] [("p" | "P"            ) Exponent] ["i" | "j"];
" Octal       = {O} ["." {O}] [("p" | "P"            ) Exponent] ["i" | "j"];
" Hexadecimal = {H} ["." {H}] [("p" | "P"            ) Exponent] ["i" | "j"];
" Decimal     = {D} ["." {D}] [("p" | "P" | "e" | "E") Exponent] ["i" | "j"];
" Exponent    = ["-" | "+"] D {D};
syn match AlchaPrefix display contained "0b\|0o\|0x"
syn match AlchaBinExp display contained "[pP]_*\([+-]\?\(\d\|_\)\+\)"
syn match AlchaDecExp display contained "[eE]_*\([+-]\?\(\d\|_\)\+\)"
syn match AlchaConstant "\<\d\(\d\|_\)*\(\.\(\d\|_\)*\)\?\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*" contains=AlchaBinExp,AlchaDecExp
syn match AlchaConstant "\(\.\d\(\d\|_\)*\)\([pPeE]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"               contains=AlchaBinExp,AlchaDecExp
syn match AlchaConstant "\<0b[01_]*\(\.[01_]*\)\?\([pP]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"           contains=AlchaPrefix,AlchaBinExp
syn match AlchaConstant "\<0o\(\o\|_\)*\(\.\(\o\|_\)*\)\?\([pP]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"   contains=AlchaPrefix,AlchaBinExp
syn match AlchaConstant "\<0x\(\x\|_\)*\(\.\(\x\|_\)*\)\?\([pP]_*\([+-]\?\(\d\|_\)\+\)\)\?\([ij]\)\?_*"   contains=AlchaPrefix,AlchaBinExp

" Keywords
syn match AlchaKeyword "\<\(__FILE__\|__LINE__\|__CLASS__\|__FUNCTION__\|__DATE__\|__TIME__\|target\|set_target\|void\|auto\|pin\|sig\|clk\|int\|rat\|float\|complex\|in\|out\|signed\|class\|if\|else\|for\|while\|loop\|switch\|case\|default\|import\|as\|return\|break\|continue\|rtl\|fsm\|hdl\)\>"

" Strings
syn match  AlchaEscape display contained "\\n\|\\t\|\\v\|\\b\|\\r\|\\f\|\\a\|\\\\\|\\?\|\\'\|\\\"\|\\x\x\{2}\|\\u\x\{4}\|\\U\x\{8}\|\\\o\{1,11}\|\\&\(\d\|\a\)\{-};"
syn region AlchaString start=/"/ skip=/\\"/ end=/"/ contains=@Spell,AlchaEscape

" Comment
syn match  AlchaComment "\/\/.*" contains=@Spell
syn region AlchaComment       start=/\/\*/ end=/\*\// contains=@Spell
syn region AlchaNestedComment start=/\/+/  end=/+\//  contains=@Spell,AlchaNestedComment

" Link the types
hi def link AlchaOperator      Operator
hi def link AlchaPrefix        SpecialChar
hi def link AlchaBinExp        SpecialChar
hi def link AlchaDecExp        SpecialChar
hi def link AlchaConstant      Constant
hi def link AlchaKeyword       Keyword
hi def link AlchaEscape        SpecialChar
hi def link AlchaString        String
hi def link AlchaComment       Comment
hi def link AlchaNestedComment Comment

" Set the syntax type
let b:current_syntax = "alcha"


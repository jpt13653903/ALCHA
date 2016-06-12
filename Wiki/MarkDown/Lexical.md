[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Lexical

The scanner reads UTF-8 formatted files.  The UTF-8 formatting is retained, even though the input stream is searched for Unicode space and newline characters.  At this stage, white-space and comments are ignored, adjacent strings are concatenated and escape sequences within strings are evaluated.  The lexical EBNF is presented below:

[[include repo=code path=Wiki/EBNF/Scanner.ebnf]]

## Literals
ALCHA literals are all stored as infinite-precision rational numbers (by means of the [GNU MP library](https://gmplib.org/)). An imaginary literal can be specified by using either a `j` or `i` postfix.  Literals can be binary (`0b` prefix), octal (`0o` prefix), hexadecimal (`0x` prefix) or decimal (no prefix).  Decimal numbers have an optional decimal exponent by means of an `e` postfix.  Binary, octal, decimal and hexadecimal numbers have an optional binary exponent by means of a `p` postfix.  All literals can be cast to a fixed-point format by means of the `'` operator.  Within numerical literals, the underscore character (`_`) is ignored.  Character literals are string literals with only one character.  Below are some examples:

    :::C++
    123.456           // decimal constant
    123.456'(16, 128) // Decimal constant cast to 16 bits with full-scale 128
    0b101.0101        // Binary constant
    0o123.456         // Octal constant
    0xABC.DEF         // Hexadecimal constant
    123_456_e7        // Decimal constant with decimal exponent (10^7)
    123_456_p7        // Decimal constant with binary exponent (2^7)
    0x123_ABC_p7      // Hexadecimal constant with binary exponent (2^7)
    7.3 + 8.9j        // Decimal complex constant
    "A"               // Unicode character constant
    "ABC"             // Unicode string literal (character array)

Unicode characters can be used directly within strings, such as `"`&ohm;`"` and `"`&alpha;&beta;&gamma;`"`.  Certain non-printable characters can be used by means of escaping.  ALCHA escape sequences are similar to D escape sequences.  The following table provides a summary:

Sequence     | Character
--------     | ---------
`\a`         | Alarm (`U+07`)
`\b`         | Backspace (`U+08`)
`\f`         | Form-feed (`U+0C`)
`\n`         | New-line (`U+0A`)
`\r`         | Carriage return (`U+0D`)
`\t`         | Horizontal tab (`U+09`)
`\v`         | Vertical tab (`U+0B`)
`\\`         | Backslash (`U+5C`)
`\'`         | Single quotation mark (`U+27`)
`\"`         | Double quotation mark (`U+22`)
`\?`         | Question mark (`U+3F`)
`\nnn`       | up to 11-digit octal Unicode
`\xHH`       | 2-digit hexadecimal Unicode
`\uHHHH`     | 4-digit hexadecimal Unicode
`\UHHHHHHHH` | 8-digit hexadecimal Unicode
`\&nnnn;`    | Named character, where `nnnn` is any of the [HTML-5 character names](https://w3.org/TR/html5/syntax.html#named-character-references)

## Identifiers
Non-digits in ALCHA are defined as the '`_`' character, as well as any character in the ranges '`a`' to '`z`', '`A`' to '`Z`' and any Unicode character above `U+7F`, as long as it is not a white-space or newline character.

Digits in ALCHA are defined as any character in the range '`0`' to '`9`'.

Identifiers start with a non-digit, which may then be followed with any digit or non-digit.

## Predefined Constants

The following constants are predefined in the same precision as the surrounding variables.  They can be redefined by the developer, following the scoping rules.

Symbol                        | Description            | Value
------                        | -----------            | -----
`pi` or &pi; (`U+0C30`)       | &pi;                   | 3.141 592 653 ...
`e`                           | Natural base           | 2.718 281 828 ...
`i` or `j`                    | Imaginary constant     | &radic;(-1)
`gamma` or &gamma; (`U+03B3`) | Euler's constant       | 0.577 215 664 ...

## Special Keywords

Alcha supports the following special keywords:

Keyword         | Description
-------         | -----------
`__FILE__`      | A string constant containing the current source file name
`__LINE__`      | A literal constant equal to the current source line number
`__DATE__`      | A string constant containing the current date, in "yyyy-mm-dd" format
`__TIME__`      | A string constant containing the current time, in "HH-mm-ss" format
`__CLASS__`     | A string constant containing the current class name.  If not in a class, "Global" is returned.
`__FUNCTION__`  | A string constant containing the current function name.  If not in a function, "Global" is returned.
`__NAMESPACE__` | A string constant containing the current name-space.  If not in a name-space, "Global" is returned.

## Syntax Highlighting Scripts
At the moment, only Vim is supported.  The Vim syntax file can be downloaded from [the repository](https://sourceforge.net/p/alcha/code/ci/master/tree/Wiki/Vim/alcha.vim?format=raw).  It is a direct implementation of the scanner, so it does not have any error-checking features.

[[include repo=code path=Wiki/MarkDown/Footer.md]]


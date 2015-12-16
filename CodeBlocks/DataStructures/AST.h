#ifndef AST_h
#define AST_h
//------------------------------------------------------------------------------

#include "mpfr.h"
//------------------------------------------------------------------------------

struct AST{
 enum TYPE{
 } Type;

 AST* Children; // First child
 AST* Next;     // Next sibling

          AST();
 virtual ~AST();

 void Evaluate();
};
//------------------------------------------------------------------------------

struct AST_BitwiseAssign: public AST{
};

struct AST_Assign: public AST{
};

struct AST_MultiplyAssign: public AST{
};

struct AST_DivideAssign: public AST{
};

struct AST_ModulusAssign: public AST{
};

struct AST_AddAssign: public AST{
};

struct AST_SubtractAssign: public AST{
};

struct AST_ShiftLeftAssign: public AST{
};

struct AST_ShiftRightAssign: public AST{
};

struct AST_BitwiseAndAssign: public AST{
};

struct AST_BitwiseXorAssign: public AST{
};

struct AST_BitwiseOrAssign: public AST{
};
//------------------------------------------------------------------------------

struct AST_Conditional: public AST{ // A ? B : C
};

struct AST_LogicalOr: public AST{
};

struct AST_LogicalAnd: public AST{
};

struct AST_BitwiseOr: public AST{
};

struct AST_BitwiseXor: public AST{
};

struct AST_BitwiseAnd: public AST{
};

struct AST_Equal: public AST{
};

struct AST_NotEqual: public AST{
};

struct AST_Less: public AST{
};

struct AST_Greater: public AST{
};

struct AST_LessEqual: public AST{
};

struct AST_GreaterEqual: public AST{
};

struct AST_ShiftLeft: public AST{
};

struct AST_ShiftRight: public AST{
};

struct AST_Add: public AST{
};

struct AST_Subtract: public AST{
};

struct AST_Multiply: public AST{
};

struct AST_Divide: public AST{
};

struct AST_Modulus: public AST{
};

struct AST_Concatenate: public AST{
};

struct AST_Replicate: public AST{
};
//------------------------------------------------------------------------------

struct AST_FixedPointCast: public AST{
};
//------------------------------------------------------------------------------

// Unary (prefix)
struct AST_Negate: public AST{
};

struct AST_LogicalNot: public AST{
};

struct AST_BitwiseNot: public AST{
};

struct AST_AndReduce: public AST{
};

struct AST_OrReduce: public AST{
};

struct AST_XorReduce: public AST{
};
//------------------------------------------------------------------------------

// Postfix
struct AST_Slice: public AST{ // 3-param: Variable, Start, opt. End
};

struct AST_BitSlice: public AST{
};

struct AST_Member: public AST{ // Stuff like Class.Member
};

struct AST_FunctionCall: public AST{
};

struct AST_Increment: public AST{
};

struct AST_Decrement: public AST{
};
//------------------------------------------------------------------------------

struct AST_Constant: public AST{
  mpq_t Value;

  AST_Constant();
 ~AST_Constant();
};

struct AST_Variable: public AST{
 // Points to variable in symbol table
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


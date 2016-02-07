#ifndef ClockTree_h
#define ClockTree_h
//------------------------------------------------------------------------------

/// @todo Need a name?  Or does this and the symbol table point to each other?

/// @todo Remember to use "set_clock_groups" for unrelated clocks

struct CLOCK_TREE{
  enum TYPE{
   Pin,   // Root level of the tree
   PLL,   // PLL-derived clock
   Enable // Clock-enable type clock
  } Type;

  double Frequency; // In Hz

  CLOCK_TREE* Next;    // Linked list of siblings
  CLOCK_TREE* Derived; // Points to first child

  CLOCK_TREE();
 ~CLOCK_TREE(); // Also deletes siblings and children
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

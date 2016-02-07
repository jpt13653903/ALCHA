#ifndef TimingSpec_h
#define TimingSpec_h
//------------------------------------------------------------------------------

/*
- Clocks:
  - Frequency
  - Phase, jitter, etc. are outside the scope of ALCHA at this point
- Ignore path (ports)
  - All ports that does not have a timing requirement directly attached are
    automatically marked for timing exclusion
- Output delay (for external synchronous IC's)
- Input delay (for external synchronous IC's)
- Min-max delay
- Multi-cycle path
*/
//------------------------------------------------------------------------------

#include "gmp.h"
//------------------------------------------------------------------------------

struct TIMING_SPEC{
 struct CLOCK{ // For now, all clocks are 50% duty-cycle and have 0 phase shift
  STRING Name;      // Also the pin / node name
  mpq_t  Frequency; // [Hz]
  CLOCK* Next;      // Also deletes the rest of the list
 };

 struct IGNORE{
  STRING  From;
  STRING  To;
  IGNORE* Next; // Also deletes the rest of the list
 };

 struct INPUT{
  CLOCK* Clock; // Reference only, memory handled elsewhere
  STRING Pin;

  bool  PosEdge;
  mpq_t PCB_Clock_Delay; // [s]
  mpq_t PCB_Data_Delay;  // [s]
  mpq_t Device_Delay;    // [s]

  INPUT* Next;  // Also deletes the rest of the list
 };

 struct OUTPUT{
  CLOCK* Clock; // Reference only, memory handled elsewhere
  STRING Pin;

  bool  PosEdge;
  mpq_t PCB_Clock_Delay; // [s]
  mpq_t PCB_Data_Delay;  // [s]
  mpq_t Sample;          // [s]
  mpq_t Hold;            // [s]

  OUTPUT* Next;  // Also deletes the rest of the list
 };

 struct ABSOLUTE{ // min-max delay specification
  STRING From;
  STRING To;

  mpq_t Minimum;
  mpq_t Maximum;
  
  ABSOLUTE* Next; // Also deletes the rest of the list
 };

 struct MULTICYCLE{
  STRING      From;
  STRING      To;
  unsigned    Cycles;
  MULTICYCLE* Next; // Also deletes the rest of the list
 }

 // Linked lists:
 CLOCK*      Clocks;
 IGNORE*     Ignores;
 INPUT*      Inputs;
 OUTPUT*     Outputs;
 ABSOLUTE*   Absolutes;
 MULTICYCLE* MultiCycles;
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

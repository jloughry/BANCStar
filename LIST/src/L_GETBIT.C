
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  GETBITS: return the right-adjusted n bits from position p of x.         **
**                                                                          **
*****************************************************************************/

unsigned getbits(unsigned x, int p, int n)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering GETBITS\n");
#endif

     return (x >> (p+1-n)) & ~(~0 << n);          /* I love C */

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit GETBITS\n");
#endif

}


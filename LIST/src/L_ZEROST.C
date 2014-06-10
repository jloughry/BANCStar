
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  ZERO_NUMSTRING: convert an integer to a string; return "0" if zero.     **
**                                                                          **
*****************************************************************************/

char *zero_numstring(int number)
{
     char zero_string[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering ZERO_NUMSTRING\n");
#endif

     strcpy(zero_string, "");
     sprintf(zero_string, "%d", number);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit ZERO_NUMSTRING\n");
#endif

     return(zero_string);
}


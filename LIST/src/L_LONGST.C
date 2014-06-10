
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  LONG_NUMSTRING: convert a long integer to string--empty string if zero. **
**                                                                          **
*****************************************************************************/

char *long_numstring(long number)
{
     char long_string[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering LONG_NUMSTRING\n");
#endif

     strcpy(long_string, "");

     if (number != 0)
          sprintf(long_string, "%ld", number);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit LONG_NUMSTRING\n");
#endif

     return(long_string);
}


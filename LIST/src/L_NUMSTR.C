
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  NUMSTRING: convert an integer to a string; return empty string if zero. **
**                                                                          **
*****************************************************************************/

char *numstring(int number)
{
     char numstr_string[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering NUMSTRING\n");
#endif

     strcpy(numstr_string, "");

     if (number != 0)
          sprintf(numstr_string, "%d", number);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit NUMSTRING\n");
#endif

     return(numstr_string);
}


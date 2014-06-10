
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  RESET_TABS: resync tab spacing to compensate for escape sequences.      **
**                                                                          **
*****************************************************************************/

int reset_tabs(char *string, int how_far)
{
     int i;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering RESET_TABS\n");
#endif

     strcpy(string, "");

     for (i=0; i<=how_far; i++)
          strcat(string, " ");

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit RESET_TABS\n");
#endif

     return(0);
}



#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  SPACES: returns a string of n spaces.                                   **
**                                                                          **
*****************************************************************************/

char *spaces(int n)
{
     char spaces_string[MAX_LINE_LENGTH];
     int i;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SPACES\n");
#endif

     strcpy(spaces_string, "");
     for (i=1; i<=n; i++)
          strcat(spaces_string, " ");

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SPACES\n");
#endif

     return(spaces_string);
}


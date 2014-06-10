
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  CHECK_LINE: given a valid line, syntax-check it.                        **
**                                                                          **
**                This function does not yet do anything.                   **
**                                                                          **
*****************************************************************************/

int check_line(char *line)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering CHECK_LINE\n");
#endif

     if (strlen(line) == 0)
          return(-1);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit CHECK_LINE\n");
#endif

     return(0);
}


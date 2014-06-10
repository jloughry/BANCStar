
#include "l_extern.h"

extern unsigned char con[], coff[];

/*****************************************************************************
**                                                                          **
**  SET_IN_CONDENSED: set the given line segment in condensed type.         **
**                                                                          **
*****************************************************************************/

int set_in_condensed(char *in_line)
{
     char longline[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SET_IN_CONDENSED\n");
#endif

     strcpy(longline, con);
     strcat(longline, in_line);
     strcat(longline, coff);

     strcpy(in_line, longline);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SET_IN_CONDENSED\n");
#endif

     return(0);
}


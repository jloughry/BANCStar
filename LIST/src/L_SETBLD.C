
#include "l_extern.h"

extern unsigned char bold[], boff[];

/*****************************************************************************
**                                                                          **
**  SET_IN_BOLDFACE: set the given line segment in boldface type.           **
**                                                                          **
******************************************************************************/

int set_in_boldface(char *in_line)
{
     char longline[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SET_IN_BOLDFACE\n");
#endif

     strcpy(longline, bold);
     strcat(longline, in_line);
     strcat(longline, boff);

     strcpy(in_line, longline);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SET_IN_BOLDFACE\n");
#endif

     return(0);
}


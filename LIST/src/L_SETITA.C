
#include "l_extern.h"

extern unsigned char ital[], itoff[];

/*****************************************************************************
**                                                                          **
**  SET_IN_ITALICS: set the given line segment in italic type.              **
**                                                                          **
*****************************************************************************/

int set_in_italics(char *in_line)
{
     char longline[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SET_IN_ITALICS\n");
#endif

     strcpy(longline, ital);
     strcat(longline, in_line);
     strcat(longline, itoff);

     strcpy(in_line, longline);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SET_IN_ITALICS\n");
#endif

     return(0);
}


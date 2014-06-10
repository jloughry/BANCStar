
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  PRINT_NEW_PAGE_NUMBER: prints a large BANCStar code page number.        **
**                                                                          **
*****************************************************************************/

int print_new_page_number(long page_number)
{
     int print_big_page_number(long);

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_NEW_PAGE_NUMBER\n");
#endif

     /*
     **  Doesn't do much, does it?  This will eventually be pulled in-line.
     */

     fprintf(fp_output, "\n");

     print_big_page_number(page_number);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit PRINT_NEW_PAGE_NUMBER\n");
#endif

     return(0);
}


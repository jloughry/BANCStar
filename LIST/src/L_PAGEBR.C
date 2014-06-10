
#include "l_extern.h"

extern unsigned char normal[], page_break_char[];

/*****************************************************************************
**                                                                          **
**  PRINT_PAGE_BREAK: prints a BANCStar code page break.                    **
**                                                                          **
*****************************************************************************/

int print_page_break(boolean forward, boolean reverse)
{
     int i;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_PAGE_BREAK\n");
#endif

     /*
     **  Get the printer out of whatever strange state it might be in.
     */

     fprintf(fp_output, "%s", normal);

     /*
     **  Complain sarcastically about weird not-quite-error conditions.
     */

     if ( (forward == 0) && (reverse == 1) && (top_of_page != 1) && !partial_page )
          fprintf(fp_output, "Wow. Ì I didn't know you could jump page boundaries like that.\n");

     if ( (forward == 1) && (reverse == 0) && (top_of_page != 1) && !partial_page )
          fprintf(fp_output, "Wow.ÃÄ I didn't know you could jump page boundaries like that.\n");

     if ( (forward == 1) && (reverse == 1) && (top_of_page != 1) && !partial_page )
          fprintf(fp_output, "Wow.ÃÌ I didn't know you could jump page boundaries like that.\n");

     /*
     **  Print the actual page break (printer-dependent character).
     */

     for (i=1; i<=79; i++)
          fprintf(fp_output, "%s", page_break_char);
     fprintf(fp_output, "\n");

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit PRINT_PAGE_BREAK\n");
#endif

     return(0);
}


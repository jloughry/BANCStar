
#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  LINENUMBER: returns a line number formatted with up to 4 leading zeros.  **
**                                                                           **
******************************************************************************/

int linenumber(char *number, long n)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering LINENUMBER\n");
#endif

    if (n < 10)
    {
        (void)sprintf(number, "0000%ld", n);
        return(0);
    }

    if (n < 100)
    {
        (void)sprintf(number, "000%ld", n);
        return(0);
    }

    if (n < 1000)
    {
        (void)sprintf(number, "00%ld", n);
        return(0);
    }

    if (n < 10000)
    {
        (void)sprintf(number, "0%ld", n);
        return(0);
    }

    else
    {
        (void)sprintf(number, "%ld", n);
        return(0);
    }

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit LINENUMBER\n");
#endif

}


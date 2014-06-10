
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  PRINT_CHECK_REPORT: report the results of CHECK to stdout.              **
**                                                                          **
**           This function is called only if there are syntax errors        **
**           detected in the source listing.  The listing is cancelled.     **
**                                                                          **
*****************************************************************************/

int print_check_report(int status, char *name, long line, char c)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_CHECK_REPORT\n");
#endif

     switch(status) {
     case 1:
          fprintf(stdout, "File %s:\textra digits found in line %ld\n", name, line+1);
          break;
     case 2:
          fprintf(stdout, "File %s:\tblank line found at line %ld\n", name, line);
          break;
     case 3:
          fprintf(stdout, "File %s:\tmissing comma in line %ld\n", name, line);
          break;
     case 4:
          fprintf(stdout, "File %s:\textra comma in line %ld\n", name, line);
          break;
     case 5:
          fprintf(stdout, "File %s:\tblank found in line %ld\n", name, line+1);
          break;
     case 6:
          fprintf(stdout, "File %s:\t\"%c\" found in line %ld\n", name, c, line+1);
          break;
     default:
          fprintf(stdout, "File %s:\t%ld lines:\tfile OK\n", name, line);
          break;
     }

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit PRINT_CHECK_REPORT\n");
#endif

     return(0);
}


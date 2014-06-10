
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  HELP: display a Help message for the user if she invokes the "/h" or    **
**        "/H" options on the command line.                                 **
**                                                                          **
**  output: full-page help message listing all of the options of LIST.      **
**                                                                          **
**  returns: (0) if successful.  (If the user asks for help and yet         **
**           provides enough information to continue, then do so.)          **
**                                                                          **
**  modified: 05/08/93                                                                        **
**                                                                          **
*****************************************************************************/

int help( void )
{
     fprintf(stdout, "%s", USAGE);
     fprintf(stdout, "\n");
     fprintf(stdout, "The options, which may appear in any order, are as follows:\n");
     fprintf(stdout, "\n");
     fprintf(stdout, "     /icl      Use the prompt file ICL.PFL\n");
     fprintf(stdout, "     /liab     Use the prompt file LIAB.PFL\n");
     fprintf(stdout, "     /other    Use the prompt file OTHER.PFL\n");
     fprintf(stdout, "     /rates    Use the prompt file RATES.PFL\n");
     fprintf(stdout, "     /pNAME    Use the prompt file specified by NAME\n");
     fprintf(stdout, "     /sM       Start listing at line M of the source file.\n");
     fprintf(stdout, "     /eN       Stop listing at line N of the source file.\n");
     fprintf(stdout, "     /a        Print listing only; do not print List Report.\n");
     fprintf(stdout, "     /d        Format output for the IBM 4019 Laser Printer and send to file.\n");
     fprintf(stdout, "     /f        Send output to the file C:\\LISTFILE.RJL -- do not print.\n");
     fprintf(stdout, "     /h        Display this Help message.\n");
     fprintf(stdout, "     /n        Do not perform syntax checking.\n");
     fprintf(stdout, "     /x        Print extended Reference List report. [default]\n");
     fprintf(stdout, "     /z        Print List report only; do not print listing.\n");
     return(0);
}


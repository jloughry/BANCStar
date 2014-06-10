
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  CHECK:      check a BANCStar file for illegal characters.               **
**                                                                          **
**  input:      any .SCN or .FRM file.                                      **
**                                                                          **
**  output:     "File OK" if the input file is a legal BANCSTAR code file,  **
**              otherwise it complains about extra digits, blank lines,     **
**              missing or extra commas, blanks or other illegal            **
                characters.                                                 **
**                                                                          **
**  returns:    0 if all file(s) okay, non-zero otherwise.                  **
**                                                                          **
**  programmer: Joe Loughry                                                 **
**                                                                          **
**  created:    6/8/90                                                      **
**                                                                          **
**  modified:   04/07/93                                                    **
**                                                                          **
*****************************************************************************/

int check( char *name, boolean nocheck, long *errline, char *errchar ) {
     int iscomma(int), isblank(int), isminus(int), isnewline(int);
     int print_check_report(int, char *, long, char);
     int check_line(char *);

     char line[MAX_LINE_LENGTH];
     int numcommas, fieldlength, linelength, c, status, errors, rc;
     long linecount;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering CHECK\n");
#endif

     errors = 0;

     if (nocheck)
          return(errors);

     if ((fp_check = fopen(name, "r")) == NULL)
     {
          perror("LIST");
          fclose(fp_check);
          return(-1);
     }

     /*
     **  This function is basically deterministic finite-state automaton
     **  that recognizes valid BANCStar code.  Actually, all it does is
     **  look for extra or missing commas, too many digits, blank lines
     **  or illegal characters.  Once it has assembled an entire line,
     **  it sends it off to check_line() for closer analysis.
     **
     **  As with most such clever things, I don't entirely understand
     **  how it works any more, either.  But it does work.
     */

     linecount = 0;
     numcommas = 0;
     fieldlength = 0;
     linelength = 0;
     status = 0;

     while ((c = fgetc(fp_check)) != EOF)
     {
          line[linelength] = (char)c;

          ++linelength;

          if (isdigit(c))
          {
               ++fieldlength;
               if (fieldlength > 5)
               {
                    status = 1;
                    break;
               }
          }

          if (iscomma(c))
          {
               ++numcommas;
               fieldlength = 0;
          }

          if (isnewline(c))
          {
               ++linecount;

               if (linelength == 1)
               {
                    status = 2;
                    break;
               }

               if (numcommas < 3)
               {
                    status = 3;
                    break;
               }

               if (numcommas > 3)
               {
                    status = 4;
                    break;
               }

               line[linelength] = '\0';
               numcommas = 0;
               fieldlength = 0;
               linelength = 0;

               if ((rc = check_line(line)) != 0)
               {
                    status = rc;
                    break;
               }
          }

          if (isblank(c))
          {
               status = 5;
               break;
          }

          if ((!isdigit(c))&&(!iscomma(c))&&(!isnewline(c))&&(!isminus(c)))
          {
               status = 6;
               break;
          }

          /*
          **  Check to see if the user wants to escape out of the program.
          */

          if ( kbhit() )
          {
               if ( getch() == 27 )
               {
                    fprintf(stdout, "\nAre you sure you want to exit? (Y/N) ");
                    console_input = getch();

                    if ( ( console_input == 13 ) || ( console_input == 27 ) ||
                         ( console_input == 'Y') || ( console_input == 'y' ) )
                    {
                         fclose(fp_input);
                         fclose(fp_output);
                         fclose(fp_prompt);
                         fclose(fp_check);
                         fprintf(stdout, "\nOkay.\n");

                         exit(-2);
                    }
               }
               else
                    fprintf(stdout, "\nContinuing . . .\n");

          }  /* end of user exit loop */

     } /* end while fgetc(fp_check */

     /*
     **  report any errors found to LIST High Command.
     */

     fclose(fp_check);
     if (status != 0)
          errors = 1;

     *errline = linecount;
     *errchar = (char)c;

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit CHECK\n");
#endif

     return(errors);

}  /* end check() */


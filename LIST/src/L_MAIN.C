
#include "l_global.h"
#include <time.h>

/*****************************************************************************
**                                                                          **
**  MAIN function.                                                          **
**                                                                          **
**      modified 04/03/93 -- changed logic in main "check, list, convert,   **
**      screenprint" statement to make screen prints an exclusive option.   **
**                                                                          **
**      modified 04-05-93 -- instead of allocating the maximum amount of    **
**      space for prompt names, I changed struct pfile to use a char * to   **
**      point to memory allocated just for that purpose.  This should       **
**      reduce the runtime memory requirements of the program considerably. **
**                                                                          **
**      modified 04-11-93 -- removed references to halloc() and hfree().    **
**                                                                          **
**      modified 05-08-93 -- added code to convert promptfile to alternate  **
**      BRANCHBANKER data dictionary format for conversion.                 **
**                                                                          **
*****************************************************************************/

int main(int argc, char **argv)
 {
     int setup( int, char **, Promptfile, long *, long *, boolean *,
                boolean *, boolean *, boolean *, boolean *, boolean *,
                boolean *, boolean *, boolean *, int *, Sellstar_system * );
     int help( void );
     int check( char *, boolean, long *, char * );
     int print_check_report(int, char *, long, char);
     int list( char *, Promptfile, long, long, boolean, boolean, boolean );
     int convert ( char *, Promptfile, Sellstar_system );
     int make_screen_prints ( char *, Promptfile, Sellstar_system, boolean );
     int linenumber( char *, long );
     int un_setup_printer( void );
     int bbdd( Promptfile prompt, int starting_at );

     Promptfile prompt;

     int rc, checkresult;
     boolean nocheck;
     boolean convert_file;
     boolean report_only;
     boolean list_only;
     boolean extended_report;
     boolean file_only;
     boolean laser_option;
     boolean screen_print_option;
     boolean convert_data_dictionary;
     int starting_where;

     Sellstar_system main_m;

     long startlistline, endlistline, errline;
     char errchar;
     char outtext_string[MAX_LINE_LENGTH];
     int text_color;
     int rand_color_1, rand_color_2;
     struct dostime_t randomtime;

     /*
     **  Print an informative message on the screen, in color.
     */

     sprintf(outtext_string, "\n");
     _outtext(outtext_string);
     text_color = _gettextcolor();

     _dos_gettime(&randomtime);
     rand_color_1 = ( randomtime.second % 6 ) + 9;
     _settextcolor(rand_color_1);
     sprintf(outtext_string, "%s", BANNER);
     _outtext(outtext_string);

     _dos_gettime(&randomtime);
     rand_color_2 = ( ( randomtime.second + 2 ) % 6 ) + 9;
     _settextcolor(rand_color_2);
     sprintf(outtext_string, "%s", COPYRIGHT);
     _outtext(outtext_string);

     _settextcolor(text_color);
     sprintf(outtext_string, "\n");
     _outtext(outtext_string);

     /*
     **  Give the user Help if desired.
     */

     if ( argc == 1 )
     {
          fprintf(stdout, USAGE);
          exit(-1);
     }

     if ( argc == 2 )
     {
          help();                   /* give the user Help here. */
          exit(-1);
     }

     /*
     ** allocate an array of struct pfile to hold the prompt file.
     */

     if ( (prompt = (Promptfile)malloc((long)(MAX_NUM_PROMPTS + 1) *
                                                sizeof(struct pfile))) == NULL )
     {
          fprintf(stdout, "Insufficient memory to run LIST.\n");
          exit(-1);
     }

     /*
     **  Get the filename to process.
     */

     strcpy(filename, argv[argc-1]);

     /*
     **  Look for it on disk.
     */

     _splitpath(filename, drive, subdir, file, ext);

     if (strlen(subdir) == 0)
          if (strcmp(strupr(ext), ".SCN") == 0)
               strcpy(subdir, SCREENS);
          else if (strcmp(strupr(ext), ".FRM") == 0)
               strcpy(subdir, FORMS);

     sprintf(pathname, "%s%s", drive, subdir);

     sprintf(longfilename, "%s%s%s%s", drive, subdir, file, ext);

     /*
     **  Before taking the time to read in the prompt file, check to make sure
     **  we can find the indicated input file.
     */

     if ((done = _dos_findfirst(longfilename, 47, &dta)) != 0)
     {
          perror("LIST");

          exit(-1);
     }

     /*
     **  Otherwise, call SETUP() and begin.
     */

     rc = setup( argc, argv, prompt, &startlistline, &endlistline, &nocheck,
                 &report_only, &list_only, &extended_report, &file_only,
                 &convert_file, &laser_option, &screen_print_option,
                 &convert_data_dictionary, &starting_where, &main_m
     );

     if ( convert_data_dictionary ) {
        bbdd(prompt, starting_where);
        fcloseall();
        goto all_done;
     }

     /*
     **  Get the filename [ again ] to hand off to List().
     */

     strcpy(filename, argv[argc-1]);

     _splitpath(filename, drive, subdir, file, ext);

     if (strlen(subdir) == 0)
          if (strcmp(strupr(ext), ".SCN") == 0)
               strcpy(subdir, SCREENS);
          else if (strcmp(strupr(ext), ".FRM") == 0)
               strcpy(subdir, FORMS);

     sprintf(pathname, "%s%s", drive, subdir);

     sprintf(longfilename, "%s%s%s%s", drive, subdir, file, ext);

     if ((done = _dos_findfirst(longfilename, 47, &dta)) != 0)
     {
          perror("LIST");

          exit(-1);
     }

     if ( convert_file ) {
        if (strcmp(strupr(ext), ".SCN") == 0)
           fprintf(stdout, "Converting screen file to BRANCHBANKER format . . .\n");
        else {
           fprintf(stdout, "BRANCHBANKER conversion does not apply to forms.\n");

          exit(-1);
        }
     }
     else
        fprintf(stdout, "Running . . .\n");

     /*
     **  (There may be more than one file.)
     */

     while ( !done )
     {
          strcpy(name, pathname);
          strcat(name, dta.name);
          strupr(name);

          /*
          **  First CHECK the file.  Only if it passes do we actually LIST it.
          */

          if ((checkresult=check(name, nocheck, &errline, &errchar))==0)
          {
                if ( convert_file ) {
                        convert ( name, prompt, main_m );
                }
                else {
                        if ( ( screen_print_option ) &&
                             (strcmp(strupr(ext), ".SCN") == 0) ) {
                                make_screen_prints( name, prompt, main_m,
                                                    laser_option );
                        }
                        else {
                                list( name, prompt, startlistline, endlistline,
                                      report_only, list_only, extended_report );
                        }
                }
          }
          else
                if (checkresult != (-1))
                        print_check_report(checkresult, name, errline, errchar);

          /*
          **  If sending this file to a file, print a formfeed here (since it
          **  is presumably NOT going directly to DOS Print).
          */

          if ( ( file_only ) && ( !convert_file ) )
               fprintf(fp_output, "\f");

          /*
          **  Check to see if the user wants to escape out and exit.
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

          /*
          **  Otherwise, look for the next file.
          */

          done = _dos_findnext(&dta);

     }  /* end of MAIN while loop */

     if ( !convert_file )
        un_setup_printer();          /* turn off automatic perforation skip */

     if ( laser_option ) {
        fputc(27, fp_output);        /* RESET IBM LASER PRINTER TO HP MODE! */
        fputc(91, fp_output);
        fputc(75, fp_output);
        fputc( 3, fp_output);
        fputc( 0, fp_output);
        fputc( 1, fp_output);
        fputc(49, fp_output);
        fputc( 2, fp_output);
     }

     /*
     **  Clean up, free memory, and exit.
     */

     fclose(fp_output);
     fclose(fp_input);
     fclose(fp_prompt);
     fclose(fp_check);
     fclose(fp_main);

     /*
     **  Send the whole mess to the printer, cancelling any previous job
     **  left in the print queue BY THIS PROGRAM ONLY, to avoid locking up
     **  the machine on a file-access conflict.
     */

     if ( file_only )
     {
          fprintf(stdout, "Output has been sent to the file C:\\LISTFILE.RJL\n");

          if ( laser_option ) {
             fprintf(stdout, "\n");
             fprintf(stdout, "To print to the IBM Laser Printer, you must first be logged on to the LAN.\n");
             fprintf(stdout, "Restart the computer with the LAN diskette, log on and exit to DOS.\n");
             fprintf(stdout, "Then use NPRINT C:\\LISTFILE.RJL /nb /noff /S=WA001HME01 /Q=BKS_01 to print.\n");
          }
     }
     else if ( !convert_file )
     {
          system(CANCEL_PRINTING);

          system(SEND_TO_PRINTER);
     }

     /*
     **  And we're outa here.
     */
all_done:
     return(0);
}

/*
**      "'Gnarly' is an instantaneous assessment of savoir faire;
**           'bitchin' is more of a performance metric...."
*/


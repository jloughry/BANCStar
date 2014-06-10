
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  LIST:       produce nicely formatted listings of BANCStar code.         **
**                                                                          **
**  input:      any .SCN or .FRM file, plus a prompt file.                  **
**                                                                          **
**  output:     List takes a screen or form file as input and sends it to   **
**              the printer in a more readable form.  Prompt numbers are    **
**              set in boldface, forward and reverse block conditionals     **
**              are set off and uniquely identified, pages are separated    **
**              and numbered, and the listing is printed with line numbers, **
**              perforation breaks, and a header showing the file name and  **
**              date.  Off to the right is a fully commented pseudocode     **
**              translation.                                                **
**                                                                          **
**  history:    This algorithm is based on the prototype LIST.BAS,          **
**              developed over a period of months in my spare time.  It     **
**              has been carefully tuned according to the whims of its      **
**              users to produce correct output for all of the cases on     **
**              which it has been tested; however, lack of a complete       **
**              grammar for BANCStar has made proving the program           **
**              difficult.  This version (unlike the prototype) will be     **
**              designed to accomodate such a grammar if and when one       **
**              is ever provided.                                           **
**                                                                          **
**  programmer: Joe Loughry                                                 **
**                                                                          **
**  created:    4/06/90                                                     **
**                                                                          **
**  modified:   04/11/93 -- removed references to halloc() and hfree()      **
**                                                                          **
**  The Second Law of Computer Programming: Whenever possible, steal code.  **
**                                                                          **
*****************************************************************************/

int list( char *name, Promptfile prompt, long start, long end,
          boolean report_only, boolean list_only, boolean extended_report ) {
     int read_bancstar_source_code( char *, struct sourceline * );

     int linenumber( char *, long );
     int arrows( char *, boolean *, boolean *, struct sourceline, long );
     int sourcecode( struct sourceline, struct sourceline_string * );
     int reset_tabs( char *, int );
     int indentation( char *, struct sourceline, boolean );
     int comments( char *, struct sourceline, Promptfile, long );
     char *numstring( int );
     int set_in_condensed ( char * );
     int reset_printer( void );
     int setup_printer( void );

     int print_list_header( char *, long, long, boolean, boolean );
     int generate_header_macro ( char *, long );
     int print_list_footer( boolean, boolean );
     int print_page_break( boolean, boolean );
     int print_new_page_number( long );
     int print_list_report( char *, Promptfile, long, boolean );

     int attr_shift_over;                  /* used for aligning tab settings */
     char lnumline[MAX_LINE_LENGTH];       /* to hold line number of listing */
     char arrowline[MAX_LINE_LENGTH];      /* for block conditional arrows   */
     char taboverline[MAX_LINE_LENGTH];    /* for tabs after source code     */
     char indentline[MAX_LINE_LENGTH];     /* for indentation of comments    */
     char new_taboverline[MAX_LINE_LENGTH];/* shortened version of tabln     */
     char new_indentline[MAX_LINE_LENGTH]; /* shortened version of indentln  */
     char commentline[MAX_LINE_LENGTH];    /* for the comments themselves    */

     char inline[MAX_LINE_LENGTH];         /* input line of BANCStar code    */
     struct sourceline inputline;          /* four fields of the input line  */
     struct sourceline_string inputline_string;  /* string version of above  */

     char outline[MAX_LINE_LENGTH];        /* output line for printing       */

     long linenum = 1;                     /* current line number            */
     long pagenum = 1;                     /* BANCStar code page number      */
     long total_number_of_lines = 0;       /* length of the file in lines    */

     int length_of_page = 0;               /* to print running page headers  */

     boolean in_forward_conditional = FALSE;     /* currently inside a 3001? */
     boolean in_reverse_conditional = FALSE;     /* currently inside a 3101? */
     boolean start_new_page = FALSE;         /* used to keep last page break */
                                           /* from appearing doubled         */
     char *fgets_rc;                       /* return code from line input fn */

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering LIST\n");
#endif

     /*
     ** local variables and setup.
     */

     top_of_page = FALSE;
     end_of_page = FALSE;
     indent_level = 0;

     /*
     **  Are we starting in the middle of a page?
     */

     partial_page = FALSE;
     if ( (start != 1) || (end != A_VERY_LARGE_NUMBER) )
          partial_page = TRUE;

     /*
     **  Clear out the String section.
     */

     strcpy(indentline, "");
     strcpy(lnumline, "");
     strcpy(arrowline, "");
     strcpy(taboverline, "");
     strcpy(indentline, "");
     strcpy(new_taboverline, "");
     strcpy(new_indentline, "");
     strcpy(commentline, "");

     /*
     ** count the number of lines in the file for Annie.
     */

     linenum = 1;
     total_number_of_lines = 1;

     if ((fp_input = fopen(name, "r")) == NULL)
     {
          perror("LIST");
          fclose(fp_input);
          return(-1);
     }

     while (((fgets_rc = fgets(inline, MAX_LINE_LENGTH, fp_input)) != NULL))
     {
          ++linenum;
          ++total_number_of_lines;

          if (linenum < start)
               total_number_of_lines = 0;

          if (linenum > end)
               break;
     }

     fclose(fp_input);

     /*
     **  Correct the value of total_number_of_lines for boundary conditions.
     */

     if ( start == 1 )
          total_number_of_lines -= 1;            /* "fencepost errors" */
     else
          total_number_of_lines -= 2;

     /*
     ** check the available space on C: drive and complain if insufficient.
     */

     if ( _dos_getdiskfree ( 0, &dfinfo ) != 0 )
     {
          perror("LIST");
     }
     else
     {
          bytes_per_disk_cluster = dfinfo.sectors_per_cluster *
                                   dfinfo.bytes_per_sector;
          total_disk_space = dfinfo.total_clusters *
                             bytes_per_disk_cluster;
          free_disk_space = dfinfo.avail_clusters *
                            bytes_per_disk_cluster;

          if ( free_disk_space < ( total_number_of_lines * EXPANSION_FACTOR ) )
          {
               fprintf(stdout, "\n\aPreliminary analysis suggests that there is insufficient space\n");
               fprintf(stdout, "on the C:\\ drive to properly buffer a file of this size.  Please\n");
               fprintf(stdout, "consider deleting some files on the hard disk in order to free up more space.\n");
               fclose(fp_input);
               fclose(fp_output);
               fclose(fp_check);
               fclose(fp_prompt);

               exit(-3);
          }
     }

     /*
     **  Open the file to be read.
     */

     if ((fp_input = fopen(name, "r")) == NULL)
     {
          perror("LIST");
          fclose(fp_input);
          return(-1);
     }

     linenum = 0;
     external_references = 0;
     data_model_references = 0;

     /*
     **  If we have a starting number different from 1, then skip ahead.
     **  [ Keeping track, of course, of all the block conditionals. ]
     */

     if (start > 1)               /* then skip lines in file to start */
     {
          while (((fgets_rc = fgets(inline, MAX_LINE_LENGTH, fp_input)) != NULL))
          {
               ++linenum;

               /*
               **  Read a line of code and look at it.
               */

               read_bancstar_source_code(inline, &inputline);

               if (inputline.L4 < 0)
                    ++pagenum;

               if ( (inputline.L1 == 3001) && ((inputline.L2 != 0) ||
                                               (inputline.L3 != 0) ||
                                               (inputline.L4 != 0)) )
                    in_forward_conditional = 1;

               if ( (inputline.L1 == 3001) && (inputline.L2 == 0) &&
                    (inputline.L3 == 0) && (inputline.L4 == 0) )
                    in_forward_conditional = 0;

               if ( (inputline.L1 == 3101) && (inputline.L2 == 0) &&
                    (inputline.L3 == 0) && (inputline.L4 == 0) )
                    in_reverse_conditional = 1;

               if ( (inputline.L1 == 3101) && ((inputline.L2 != 0) ||
                                               (inputline.L3 != 0) ||
                                               (inputline.L4 != 0)) )
                    in_reverse_conditional = 0;

               if ( linenum > ( start - 1 ) )
                    break;                       /* out of the while loop */
          }

          if (fgets_rc == NULL)
          {
               fprintf(stdout, USAGE);
               fprintf(stdout, "Not that many lines in file.\n");
               fclose(fp_input);

               exit(-1);
          }
     }

     /*
     **  Now that we're at the right spot, begin the listing.
     */

     if ( !report_only ) {
          print_list_header( name, pagenum, total_number_of_lines,
                            in_forward_conditional, in_reverse_conditional
          );

          /*
          **  generate and send to the printer a page header temporary
          **  macro, and then enable it for overlay with slightly different
          **  top and bottom margin settings.
          */

          /*
          generate_header_macro ( name, total_number_of_lines );
          */
     }

     /*
     **  Main LIST while loop:
     */

     length_of_page = 0;

     while ((fgets_rc = fgets(inline, MAX_LINE_LENGTH, fp_input)) != NULL)
     {
          ++linenum;

          /*
          **  Are we done yet?
          */

          if ( linenum > end )
               break;

          end_of_page = FALSE;

          if ( start_new_page == TRUE )
          {
               if ( !report_only )
                    print_new_page_number(pagenum);

               start_new_page = FALSE;
          }

          /*
          **  Read a line of code.
          */

          read_bancstar_source_code(inline, &inputline);

          strcpy(outline, "");

          /*
          **  First construct a line number.
          */

          linenumber(lnumline, linenum);
          set_in_condensed(lnumline);

          /*
          **  Then analyze it.
          */

          arrows(arrowline, &in_forward_conditional, &in_reverse_conditional,
                 inputline, linenum);
          sourcecode(inputline, &inputline_string);
          indentation(indentline, inputline, in_reverse_conditional);
          comments(commentline, inputline, prompt, linenum);

          /*
          **  correct horizontal tab settings for the influence of all those
          **  invisible escape sequences and other non-printing characters.
          */

          attr_shift_over = strlen(numstring(inputline.L1)) +
                            strlen(numstring(inputline.L2)) +
                            strlen(numstring(inputline.L3)) +
                            strlen(numstring(inputline.L4)) + 3;
          attr_shift_over -= ((attr_shift_over - 14) * 2);

          reset_tabs(taboverline, attr_shift_over);

          strncpy(new_taboverline, taboverline, strlen(taboverline)-4);
          new_taboverline[strlen(taboverline)-4] = '\0';

          strncpy(new_indentline, indentline, strlen(indentline)-3);
          new_indentline[strlen(indentline)-3] = '\0';

          /*
          **  assemble the final output line.
          */

          sprintf(outline, "%s %s%s,%s,%s,%s%s. %s%s\n",
                  lnumline,
                  arrowline,
                  inputline_string.L1_string,
                  inputline_string.L2_string,
                  inputline_string.L3_string,
                  inputline_string.L4_string,
                  new_taboverline,
                  new_indentline,
                  commentline
          );

          /*
          **  keep a copy of the line if it contains any external references.
          */

          if ( ( ( inputline.L1 == 8500 ) || ( inputline.L1 == 8550 ) ||
                                             ( inputline.L1 == 8400 ) )
              && ( external_references < MAX_EXTERNALS ) )
          {
               if ( ( external_line[external_references] = (char *)malloc(strlen(outline) + 1)) == NULL ) {
                  fprintf(stdout, "LIST (L_LIST): malloc failed on external ref %s", outline);
                  exit(-1);
               }
               else {
                  strcpy(external_line[external_references], outline);
                  ++external_references;
               }
          }

          /*
          **  keep a copy of the line if it contains any data model calls.
          */

          if ( ( inputline.L1 >= 9200 ) && ( inputline.L1 <= 9401 ) &&
               ( data_model_references < MAX_EXTERNALS ) )
          {
               if ( ( data_model_line[data_model_references] = (char *)malloc(strlen(outline) + 1)) == NULL ) {
                  fprintf(stdout, "LIST (L_LIST): malloc failed on data model %s", outline);
                  exit(-1);
               }
               else {
                  strcpy(data_model_line[data_model_references], outline);
                  ++data_model_references;
               }
          }

          /*
          **  print the line.  Doing it this way (indirectly) allows us more
          **  freedom later on to do other creative things with the output.
          */

          if ( !report_only )
               fprintf(fp_output, "%s", outline);

          /*
          **  print a page break only if that was the end of a BANCStar page.
          */

          if (inputline.L4 < 0)
          {
               ++pagenum;

               if ( !report_only )
                    print_page_break(in_forward_conditional,
                                     in_reverse_conditional
                    );

               end_of_page = TRUE;
               start_new_page = TRUE;
          }

          /*
          **  Check to see if the boss wants to go home now.
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

     } /* end of main LIST while loop. */

     /*
     **  correct linenum if only printing a range of lines.
     */

     if ( linenum > end )
          --linenum;

     /*
     **  get the ending date and time.
     */

     _dos_getdate(&enddate);
     _dos_gettime(&endtime);

     /*
     **  (only need to print the page footer if we printed a listing.)
     */

     if ( !report_only )
          print_list_footer(in_forward_conditional, in_reverse_conditional);

     if ( ( !report_only ) && ( !list_only ) )
          fprintf(fp_output, "\f");

     /*
     **  Print the final report.
     */

     reset_printer();

     if ( !list_only )
          print_list_report( name, prompt, total_number_of_lines,
                            extended_report );

     fclose(fp_input);

     reset_printer();

#ifdef DIAGNOSTCS
     fprintf(stderr, "Exit LIST\n");
#endif

     return(0);
}


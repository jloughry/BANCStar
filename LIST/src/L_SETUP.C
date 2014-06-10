
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  SETUP:      reads in the prompt file for LIST.                          **
**                                                                          **
**  file:       L_SETUP.C                                                   **
**                                                                          **
**  modified:   05/08/93                                                    **
**                                                                          **
**  input:      command line (argc, argv)                                   **
**                                                                          **
**  output:     Setup locates and reads in the contents of the prompt file, **
**              finds and decodes the command line parameters for start,    **
**              end, and nocheck, and sets up the Prompt data structure     **
**              for statistics gathering at the end of the program.  If     **
**              we are NOT going to just convert files this time, then it   **
**              also opens the file c:\listfile.rjl for output spooling.    **
**                                                                          **
**  returns:    prompt, *startlistline, *endlistline, *nocheck,             **
**              *convert_only                                               **
**              The program returns (0) if successful.                      **
**                                                                          **
**  notes:      Open fp_output in "w" mode, but to avoid problems with      **
**              cooked-mode translation of 0dh to 0d0ah automatically,      **
**              have to change the current stream to "wb" mode before       **
**              trying to write binary data.  Unfortunately, setmode()      **
**              will not work (only if we haven't yet written anything),    **
**              so the only way to do it is to close the stream, re-open    **
**              it as "ab", write the binary data, close it again, and      **
**              finally re-open the file with mode "a".  What a crock.      **
**                                                                          **
**              05-08-93: added code to convert promptfiles to BRANCHBANKER **
**              data dictionary format.                                     **
**                                                                          **
*****************************************************************************/

int setup ( int argc, char **argv, Promptfile prompt,
            long *startlistline, long *endlistline, boolean *nocheck,
            boolean *report_only, boolean *list_only,
            boolean *extended_report, boolean *file_only,
            boolean *convert_only, boolean *laser_option,
            boolean *screen_print_option, boolean *convert_data_dictionary,
            int *starting_where, Sellstar_system *main_m ) {
     int read_command_line_options( int, char **, long *, long *, boolean *,
                                    boolean *, boolean *, boolean *,
                                    boolean *, boolean *, boolean *,
                                    boolean *, boolean *, int *,
                                    Sellstar_system * );
     int read_promptfile_code( char *, struct pfile * );
     int setup_printer( void );
     int reset_printer( void );

     int i, j;
     boolean control_characters_found;
     char line[MAX_LINE_LENGTH];
     struct pfile promptfile_input_line;
     char new_prompt_name[MAX_LINE_LENGTH];
     char control_string[MAX_LINE_LENGTH];

     /*
     **  Get the current date and time and store them for later.
     */

     _dos_getdate(&startdate);
     _dos_gettime(&starttime);

     /*
     **  get the command line options.
     */

     read_command_line_options ( argc, argv, startlistline, endlistline,
                                 nocheck, report_only, list_only,
                                 extended_report, file_only, convert_only,
                                 laser_option, screen_print_option,
                                 convert_data_dictionary, starting_where,
                                 main_m );

     /*
     **  Make sure that starting and ending lines are consistent and make sense
     */

     if (*startlistline == 0)
          *startlistline = 1;

     if (*endlistline == 0)
          *endlistline = A_VERY_LARGE_NUMBER;

     if ( (*startlistline < 0) || (*endlistline < 0) )
     {
          fprintf(stdout, USAGE);
          fprintf(stdout, "Starting or ending lines may not be less than 1.\n");

          exit(-1);
     }

     if (*startlistline > *endlistline)
     {
          fprintf(stdout, USAGE);
          fprintf(stdout, "Starting line must be less than ending line.\n");

          exit(-1);
     }

     /*
     **  read in the prompt file.
     */

     strcpy(filename, promptfilename);
     _splitpath(filename, drive, subdir, file, ext);
     sprintf(pathname, "%s%s", drive, subdir);

     _dos_findfirst(filename, 47, &dta);

     strcpy(name, pathname);
     strcat(name, dta.name);
     strupr(name);

     if ( ( strlen( promptfilename ) == 0 ) ||
          ( strcmp( strupr( ext ), ".PFL" ) != 0 ) )
     {
          fprintf(stdout, USAGE);
          fprintf(stdout, "You must specify a prompt file.\n");

          exit(-1);
     }

     if ((fp_prompt = fopen(name, "r")) == NULL)
     {
          perror("LIST");
          fclose(fp_prompt);

          exit(-1);
     }

     /*
     **  get the prompt file's date and time before we lose the file pointer.
     */

     promptfile_handle = fileno(fp_prompt);
     _dos_getftime(promptfile_handle, &pdate, &ptime);

     /*
     **  read in the prompt file.
     */

     i = 0;
     promptfile_input_line.name = (char *)malloc((size_t)MAX_PROMPT_LENGTH+1);

     while ( ( i < MAX_NUM_PROMPTS ) &&
             ( (fgets(line, MAX_LINE_LENGTH, fp_prompt)) != NULL) )
     {
          ++i;

          read_promptfile_code(line, &promptfile_input_line);

          /*
          **  Get the prompt's name.
          */

          prompt[i].name = (char *)malloc(strlen(promptfile_input_line.name)+1);
          strcpy(prompt[i].name, promptfile_input_line.name);

          /*
          **  File read/write return code.
          */

          if ( (strlen(prompt[i].name) == 0) && (i == 160) )
          {
               free(prompt[i].name);
               prompt[i].name = (char *)malloc(strlen("return code")+1);
               strcpy(prompt[i].name, "return code");
               prompt[i].type = 4;
          }

          /*
          **  Branch & Link parameters.
          */

          if ( (strlen(prompt[i].name) == 0) && (i == 1908) )
          {
               free(prompt[i].name);
               prompt[i].name = (char *)malloc(strlen("return address low")+1);
               strcpy(prompt[i].name, "return address low");
               prompt[i].type = 4;
          }

          if ( (strlen(prompt[i].name) == 0) && (i == 1909) )
          {
               free(prompt[i].name);
               prompt[i].name = (char *)malloc(strlen("return address high")+1);
               strcpy(prompt[i].name, "return address high");
               prompt[i].type = 4;
          }

          /*
          **  Undefined prompts.
          */

          if (strlen(prompt[i].name) == 0)
          {
               free(prompt[i].name);
               prompt[i].name = (char *)malloc(strlen("-->UNDEFINED<--")+1);
               strcpy(prompt[i].name, "-->UNDEFINED<--");
               prompt[i].type = 4;
          }

          /*
          **  Sniff out any non-printing control characters in the prompt.
          */

          j = 0;
          control_characters_found = FALSE;

          while ( (prompt[i].name[j] != NULL) && (!control_characters_found) )
          {
               if (iscntrl(prompt[i].name[j]))
                    control_characters_found = TRUE;
               j++;
          }

          /*
          **  If you find any, process them similarly to how they appear
          **  in the prompt file listing.
          */

          if ( control_characters_found )
          {
               strcpy(new_prompt_name, "ASCII (");

               j = 0;

               while ( prompt[i].name[j] != NULL )
               {
                    if ( prompt[i].name[j] == '"' )
                    {
                         ++j;
                         continue;
                    }
                    sprintf(control_string, "%d", (int)prompt[i].name[j]);
                    strcat(new_prompt_name, control_string);

                    if ( ( prompt[i].name[j+1] != NULL ) &&
                             ( prompt[i].name[j+1] != '"' ) )
                           strcat( new_prompt_name, ", " );
                    else
                           strcat( new_prompt_name, ")" );

                    ++j;
               }
               free(prompt[i].name);
               prompt[i].name = (char *)malloc(strlen(new_prompt_name)+1);
               strcpy( prompt[i].name, new_prompt_name );
          }

          /*
          **  Get the prompt's type.
          */

          prompt[i].type = promptfile_input_line.type;

          /*
          **  If the type is >= 10, then filter out the COLOR information.
          */

          if (prompt[i].type >= 10)
               prompt[i].type = prompt[i].type % 10;

          /*
          **  Get the prompt's length.
          */

          prompt[i].length = promptfile_input_line.length;

          /*
          **  Mark the prompt unused in the PFILE data structure.
          */

          prompt[i].referenced = 0;
          prompt[i].referenced_list = NULL;

          prompt[i].displayed = 0;
          prompt[i].displayed_list = NULL;

          prompt[i].modified = 0;
          prompt[i].modified_list = NULL;

          prompt[i].contents = (char *)malloc(1);
          strcpy( prompt[i].contents, "" );
     }
     fclose(fp_prompt);

     /*
     **  Check to make sure the user doesn't want to exit.
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
     **  Open the printer spooling file for output as fp_output.
     */

     if ( !*convert_only ) {
        if ((fp_output = fopen(LISTFILE, "w")) == NULL)
        {
             perror("LIST");
             fclose(fp_output);

             exit(-1);
        }
     }

     /*
     **  Set up the printer.
     */

     if ( !*convert_only ) {
        reset_printer();
        setup_printer();
     }

     if ( !*convert_only )
        if ( *laser_option ) {
           fputc(27, fp_output);     /* disable perforation skip */
           fputc(79, fp_output);

           fputc(27, fp_output);     /* set IBM PPDS (Proprinter emulation) */
           fputc(91, fp_output);     /* mode on laserprinter */
           fputc(75, fp_output);
           fputc( 3, fp_output);
           fputc( 0, fp_output);
           fputc( 1, fp_output);
           fputc(49, fp_output);
           fputc( 1, fp_output);
     }

     if ( *laser_option )
        *file_only = TRUE;

     return(0);
}


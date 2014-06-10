
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  SCREENPRINT:  automatically generate screen prints of a BANCStar        **
**                screen file.                                              **
**                                                                          **
**  input:        any .SCN file, plus a prompt file and a MAIN.M file.      **
**                                                                          **
**  output:       a series of screen prints, one per page (with a report)   **
**                from the indicated screen file.  On the IBM 4019 Laser    **
**                Printer, page borders are set off by a hairline, and      **
**                windows are printed with a grey shaded background.        **
**                                                                          **
**                All visible prompts are shown, together with table        **
**                constants used as static text on screen.  BANCStar        **
**                windows are each printed on a separate page.              **
**                                                                          **
**                Screen prints are shown at the end of the List Report.    **
**                                                                          **
**  programmer:   Joe Loughry                                               **
**                                                                          **
**  created:      11/01/91                                                  **
**                                                                          **
**  modified:     04/11/93                                                  **
**                                                                          **
**  notes:        This enhancement was suggested by Annie Cruz: 10/31/91    **
**                                                                          **
**                04/03/93: added code to print prompt numbers on screen    **
**                prints along with a listing of all prompts appearing on   **
**                a screen and their name, type, length, helpfile, etc.,    **
**                to facilitate common documentation tasks.                 **
**                                                                          **
**                okay to open tempfile in "w+" mode, as we are not doing   **
**                pure binary writing to this file.                         **
**                                                                          **
**                cannot use setmode() with files that are already open,    **
**                so we have to close the file and then reopen it in "ab"   **
**                mode before we can write binary data.  Then we restore    **
**                things to "a" mode when ready to continue with text.      **
**                                                                          **
**                04/06/93: _dos_findfirst() trashes my data structures!    **
**                                                                          **
*****************************************************************************/

int make_screen_prints ( char *name, Promptfile prompt, Sellstar_system main_m,
                         boolean laser_option ) {
     int read_bancstar_source_code ( char *, struct sourceline * );
     int get_display_prompt ( char *, Promptfile, int, long );
     int read_in_main_dot_m ( Sellstar_system, MainDotM );
     int get_table_item ( char *, MainDotM, User_table, int );
     int set_in_boldface ( char * );
     int draw_tiny_label( unsigned prompt_number, int row, int column,
                          boolean window, boolean has_table, boolean has_help );

     char inline[MAX_LINE_LENGTH];         /* input line of BANCStar code    */
     struct sourceline inputline;          /* four fields of the input line  */
     char name_of_prompt[MAX_LINE_LENGTH]; /* displayed name of prompt       */
     char response_field[MAX_LINE_LENGTH]; /* displayed repsonse fields (BB) */
     char table_string[MAX_LINE_LENGTH];   /* contents of table lookup -->WS */

     MainDotM table;

     unsigned int M1;                      /* absolute values of code fields */
     unsigned int M2;
     unsigned int M3;
     unsigned int M4;

     int     i, j, k;                      /* general purpose integer vars   */
     int     row, col;                     /* row and column within screen   */
     boolean bs_window;                    /* displaying a BANCStar "window" */
     boolean pause_flag_only;              /* is this an "interesting" page? */

     long linenum = 1;                     /* current line number            */
     long pagenum = 1;                     /* BANCStar code page number      */
     long total_number_of_lines = 0;       /* length of the file in lines    */

     char screen_map[26][81];              /* map of text on BS screen       */

     char *fgets_rc;                       /* return code from line input    */
     char text_line[MAX_LINE_LENGTH];      /* buffer for sprintf function    */
     char short_line[20];                  /* used for holding page numbers  */

     char high_byte;                       /* used for laserprinter boxes    */
     char low_byte;
     unsigned word_quantity;

     FILE *fp_temp;                        /* for accumulating name report   */
     char helpfile[4];                     /* holds either "YES\0" or blanks */
     char table_loc[5];                    /* if associated with a table item*/
     unsigned int oldM1;                   /* used for remembering our place */
     long long_i;                          /* 32-bit counter                 */
     int report_length = 0;                /* used for paginating reports    */
     boolean has_table;
     boolean has_help;
     /*
     // This next part is subtle.  Since _dos_findfirst() and access()
     // both seem to trash my MAIN.M structure when used to determine
     // the existence of a helpfile for a given prompt, I'm going to do
     // it this way instead: making a note of all the help files that
     // exist before we start (before reading in MAIN.M, anyway), and
     // encoding that information in a very compact bit structure.  I
     // hope this works.
     */
     char file_vector[MAX_NUM_PROMPTS / 8];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SCREEN PRINT\n");
#endif

     printf("\nSearching for helpfiles.");
     for (i=0; i<(MAX_NUM_PROMPTS / 8); i++)
        file_vector[i] = 0x00;
     if (main_m == CONSUMER) {
        for (i=1; i<=2000; i++) {
                sprintf(text_line, "%s%d.hlp", CONSUMER_HELPFILE_DIR, i );
                if (access(text_line, 0) == 0) {
                        file_vector[i/8] |= (0x01 << (i%8));
                }
                if (i%100 == 0) printf(".");
        }
     }
     else if (main_m == LIABILITIES) {
        for (i=1; i<=2000; i++) {
                sprintf(text_line, "%s%d.hlp", LIABILITIES_HELPFILE_DIR, i );
                if (access(text_line, 0) == 0) {
                        file_vector[i/8] |= (0x01 << (i%8));
                }
                if (i%100 == 0) printf(".");
        }
     }
     printf("\n");
     /*
     // if the preceeding worked, we should be able now to test for the
     // existence of a suitable helpfile "i" with the expression:
     //
     //         ( file_vector[i/8] & (0x01 << (i%8)) )
     //
     // which will be non-zero if the file exists and zero otherwise.
     */
/*
     for (i=1; i<=2000; i++) {
        if ( file_vector[i/8] & (0x01 << (i%8)) )
            printf("\n%d.HLP", i);
     }
     printf("\n");
*/
     /*
     **  Allocate an array of MainDotM to hold the contents of main.m.
     */

     if ( ( table = ( MainDotM )malloc( ( long )( ( MAX_LENGTH_TABLE + 1 ) * 2)
                                        * sizeof( MainDotM ) ) ) == NULL ) {
        fprintf(stdout, "Insufficient memory to allocate space for MAIN.M\n");

        fclose(fp_input);
        fclose(fp_output);
        fclose(fp_prompt);
        fclose(fp_check);
        fclose(fp_main);

        exit(-1);
     }

     read_in_main_dot_m ( main_m, table );

     /*
     **  Count the number of lines in the file.
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
     }
     rewind(fp_input);

     /*
     **  Correct the value of total_number_of_lines for boundary conditions.
     */

     total_number_of_lines -= 1;                   /* fencepost error */

     /*
     **  Set up initial conditions.
     */

     linenum = 0;
     pagenum = 1;
     bs_window = FALSE;
     pause_flag_only = TRUE;            /* until determined to be false */

     for (i=1; i<=25; i++)
        for (j=1; j<=80; j++) {
           screen_map[i][j] = ' ';
     }

     /*
     **  Draw a frame around text area on page.
     */

     if ( laser_option ) {
        fputc(27,  fp_output);     /* set cursor position to upper left */
        fputc(91,  fp_output);
        fputc(81,  fp_output);
        fputc(6,   fp_output);     /* 6-byte command */
        fputc(0,   fp_output);
        fputc(2,   fp_output);     /* unsigned displacement from (0,0) */
        fputc(4,   fp_output);     /* 1/300 inch increments */
        fputc(0,   fp_output);     /* horizontal byte high */
        fputc(1,   fp_output);     /* horizontal byte low */
        fputc(0,   fp_output);     /* vertical byte high */
        fputc(140, fp_output);     /* vertical byte low */

        fputc(27,  fp_output);     /* draw a black box */
        fputc(91,  fp_output);
        fputc(105, fp_output);
        fputc(7,   fp_output);     /* 7-byte command */
        fputc(0,   fp_output);
        fputc(1,   fp_output);     /* unit = 1/300 inch */
        fputc(0,   fp_output);     /* write mode OR */
        fputc(9,   fp_output);     /* rectangle width high byte */
        fputc(96,  fp_output);     /* rectangle width low byte */
        fputc(4,   fp_output);     /* rectangle height high byte */
        fputc(221, fp_output);     /* rectangle height low byte */
        fputc(1,   fp_output);     /* use pattern black */

        fputc(27,  fp_output);     /* move cursor (+2,+2) relative */
        fputc(91,  fp_output);
        fputc(81,  fp_output);
        fputc(6,   fp_output);     /* 6-byte command */
        fputc(0,   fp_output);
        fputc(3,   fp_output);     /* relative move from current position */
        fputc(4,   fp_output);     /* 1/300 inch increments */
        fputc(0,   fp_output);     /* horizontal byte high */
        fputc(2,   fp_output);     /* horizontal byte low */
        fputc(0,   fp_output);     /* vertical byte high */
        fputc(2,   fp_output);     /* vertical byte low */

        fputc(27,  fp_output);     /* draw a white box */
        fputc(91,  fp_output);
        fputc(105, fp_output);
        fputc(7,   fp_output);     /* 7-byte command */
        fputc(0,   fp_output);
        fputc(1,   fp_output);     /* unit = 1/300 inch */
        fputc(3,   fp_output);     /* write mode XOR */
        fputc(9,   fp_output);     /* rectangle width high byte */
        fputc(92,  fp_output);     /* rectangle width low byte */
        fputc(4,   fp_output);     /* rectangle height high byte */
        fputc(217, fp_output);     /* rectangle height low byte */
        fputc(0,   fp_output);     /* use previous pattern */

        fputc(27,  fp_output);     /* set cursor position to upper left */
        fputc('[', fp_output);
        fputc('Q', fp_output);
        fputc(6,   fp_output);
        fputc(0,   fp_output);
        fputc(2,   fp_output);     /* unsigned displacement from (0,0) */
        fputc(4,   fp_output);     /* 1/300 inch increments */
        fputc(0,   fp_output);     /* horizontal byte high */
        fputc(0,   fp_output);     /* horizontal byte low */
        fputc(0,   fp_output);     /* vertical byte high */
        fputc(0,   fp_output);     /* vertical byte low */
     } /* end if laser_option */

     fp_temp = fopen(TEMPFILE, "w+");   /* open temp file for read/write */
     if (fp_temp == NULL) {
          perror("LIST");
          fcloseall();
          return(-1);
     }
     fprintf(fp_temp, "\n");
     sprintf(text_line, "   Num   Type     Len    Prompt                                  Table  Help");
     set_in_boldface(text_line);
     fprintf(fp_temp, "\n%s", text_line);
     sprintf(text_line, "   ----  -------  -----  --------------------------------------  -----  ----");
     set_in_boldface(text_line);
     fprintf(fp_temp, "\n%s", text_line);
     /*
     **  Print page header for this page.
     */
     sprintf ( text_line, "\n%02d-%02d-%04d", startdate.month,
                                              startdate.day,
                                              startdate.year );
     for (i=11; i<=40-strlen(name)/2; i++)
        strcat(text_line, " ");

     strcat(text_line, name);

     for (i=40+strlen(name)/2+1; i<=80-8; i++)
        strcat(text_line, " ");

     sprintf(short_line, "Page %d\n", pagenum);
     strcat(text_line, short_line);

     set_in_boldface(text_line);
     fprintf(fp_output, "%s", text_line);

     fprintf(fp_output, "\n\n");     /* set to top of box */

     /*
     **  Main SCREENPRINT while loop:
     */

     while ((fgets_rc = fgets(inline, MAX_LINE_LENGTH, fp_input)) != NULL)
     {
          ++linenum;

          /*
          **  Read a line of code.
          */

          read_bancstar_source_code(inline, &inputline);

          M1 = abs ( inputline.L1 );
          M2 = abs ( inputline.L2 );
          M3 = abs ( inputline.L3 );
          M4 = abs ( inputline.L4 );

          /*
          **  If this is a BANCStar window, then paint it to match.
          */

          if ( M1 == 8000 ) {
             bs_window = TRUE;
             /*
             **  On the IBM Laser print a grey box for the BANCStar window.
             */
             if ( laser_option ) {
                fclose(fp_output);
                if ((fp_output = fopen(LISTFILE, "ab")) == NULL) {
                        perror("LIST -- reopening fp_output in L_SCNPRT");
                        fclose(fp_output);
                        goto bail;
                }
                fputc(27,  fp_output);   /* set cursor to position of window */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(2,   fp_output);   /* use character origin (row1,col1) */
                fputc(0,   fp_output);   /* measure in terms of characters */
                fputc(0,   fp_output);
                fputc((unsigned char)(M3 % 100), fp_output);
                fputc(0,   fp_output);
                fputc((unsigned char)(M3 / 100 + 2), fp_output);

                fputc(27, fp_output);   /* move cursor over/down a little bit */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(3,   fp_output);   /* relative move from current pos */
                fputc(4,   fp_output);   /* 1/300 inch increments */
                fputc(0,   fp_output);   /* horizontal high byte */
                fputc(30,  fp_output);   /* horizontal low byte */
                fputc(0,   fp_output);   /* vertical high byte */
                fputc(30,  fp_output);   /* vertical low byte */

                fputc(27, fp_output);   /* move out of way of frame */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(3,   fp_output);   /* relative move from current pos */
                fputc(4,   fp_output);   /* 1/300 inch increments */
                fputc(0,   fp_output);   /* horizontal high byte */
                fputc(2,   fp_output);   /* horizontal low byte */
                fputc(0,   fp_output);   /* vertical high byte */
                fputc(3,   fp_output);   /* vertical low byte */

                fputc(27,  fp_output);  /* draw a black shadow behind window */
                fputc('[', fp_output);
                fputc('i', fp_output);
                fputc(7,   fp_output);  /* 7-byte command */
                fputc(0,   fp_output);
                fputc(1,   fp_output);           /* use 1/300 inch units */
                fputc(0,   fp_output);           /* logical OR mode */

                word_quantity = ( ( M4 % 100 ) - ( M3 % 100 ) + 1 ) * 30 - 4;
                high_byte = (unsigned char)(word_quantity / 256);
                low_byte = (unsigned char)(word_quantity % 256);
                fputc(high_byte, fp_output);     /* width in pels */
                fputc(low_byte,  fp_output);

                word_quantity = ( ( M4 / 100 ) - ( M3 / 100 ) + 1 ) * 50 + 25 - 5;
                high_byte = (unsigned char)(word_quantity / 256);
                low_byte = (unsigned char)(word_quantity % 256);
                fputc(high_byte, fp_output);     /* height in pels */
                fputc(low_byte,  fp_output);

                fputc(1,   fp_output);           /* make a black box */

                fputc(27,  fp_output);   /* set cursor to position of window */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(2,   fp_output);   /* use character origin (row1,col1) */
                fputc(0,   fp_output);   /* measure in terms of characters */
                fputc(0,   fp_output);
                fputc((unsigned char)(M3 % 100), fp_output);
                fputc(0,   fp_output);
                fputc((unsigned char)(M3 / 100 + 2), fp_output);

                fputc(27, fp_output);   /* move out of way of frame */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(3,   fp_output);   /* relative move from current pos */
                fputc(4,   fp_output);   /* 1/300 inch increments */
                fputc(0,   fp_output);   /* horizontal high byte */
                fputc(2,   fp_output);   /* horizontal low byte */
                fputc(0,   fp_output);   /* vertical high byte */
                fputc(3,   fp_output);   /* vertical low byte */

                fputc(27,  fp_output);  /* draw a grey box the size of window */
                fputc('[', fp_output);
                fputc('i', fp_output);
                fputc(8,   fp_output);
                fputc(0,   fp_output);
                fputc(1,   fp_output);           /* use 1/300 inch units */
                fputc(1,   fp_output);           /* overwrite mode. */

                word_quantity = ( ( M4 % 100 ) - ( M3 % 100 ) + 1 ) * 30 - 4;
                high_byte = (unsigned char)(word_quantity / 256);
                low_byte = (unsigned char)(word_quantity % 256);
                fputc(high_byte, fp_output);     /* width in pels */
                fputc(low_byte,  fp_output);

                word_quantity = ( ( M4 / 100 ) - ( M3 / 100 ) + 1 ) * 50 + 25 - 5;
                high_byte = (unsigned char)(word_quantity / 256);
                low_byte = (unsigned char)(word_quantity % 256);
                fputc(high_byte, fp_output);     /* height in pels */
                fputc(low_byte,  fp_output);

                fputc(2,   fp_output);           /* make a filled box */
                fputc(10,   fp_output);          /* use 10% greyscale */

                fputc(27,  fp_output);   /* set cursor position to upper left */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);   /* 6-byte command */
                fputc(0,   fp_output);
                fputc(2,   fp_output);   /* unsigned displacement from (0,0) */
                fputc(4,   fp_output);   /* 1/300 inch increments */
                fputc(0,   fp_output);   /* horizontal byte high */
                fputc(0,   fp_output);   /* horizontal byte low */
                fputc(0,   fp_output);   /* vertical byte high */
                fputc(150, fp_output);   /* vertical byte low */
                fclose(fp_output);
                if ((fp_output = fopen(LISTFILE, "a")) == NULL) {
                        perror("LIST -- reopening fp_output in L_SCNPRT");
                        fclose(fp_output);
                        goto bail;
                }
             } /* end if laser_option */
          } /* end if BANCStar_window */

          /*
          **  Clear the prompt, if indicated.
          */

          if ( ( M1 > 10000 ) && ( M1 <= 12000 ) )
             if ( ( M2/10 == ( M1 - 10000 ) ) && ( M2%10 == 5 ) )
                strcpy ( prompt[M1-10000].contents, "" );

          /*
          **  If this is a table transfer, then set the contents of the
          **  indicated prompt to the table lookup value.
          */

          if ( M1 > 30000 ) {
             if ( M4 == 107 ) {
                get_table_item ( table_string, table, ITEM, M3 );
                prompt[(int)(M1-30000)].contents = (char *)malloc(sizeof(table_string)+1);
                strcpy ( prompt[(int)(M1-30000)].contents, table_string );
             }
             else if ( M4 == 108 ) {
                get_table_item ( table_string, table, DESCRIPTION, M3 );
                prompt[(int)(M1-30000)].contents = (char *)malloc(sizeof(table_string)+1);
                strcpy ( prompt[(int)(M1-30000)].contents, table_string );
             }
          } /* end if table_transfer */

          /*
          **  If it is a display prompt, then paint it on the screen.
          */

          if ( ( inputline.L1 > 0 ) && ( inputline.L1 <= 2000 ) ) {
             get_display_prompt ( name_of_prompt, prompt, M1, linenum );
             if ( M2 != 0 ) {    /* then we display the name */
                /*
                **  Write text into screen_map buffer . . .
                */

                row = M2/100;
                col = M2%100;
                if ( bs_window ) {
                        ++row;
                }

                for (k=0; k<=strlen(name_of_prompt)-1; k++) {
                   screen_map[row][col+k] = name_of_prompt[k];
                   if ( col+k >= 80 ) {       /* wrap text at end of lines */
                      ++row;
                      col=1;
                   }
                }
             } /* end if display_name */

             if ( M3 != 0 ) {                      /* . . . and the response */

                strcpy( response_field, "" );
                for (i=1; i<=M3; i++)
                   strcat ( response_field, "_" );

                /*
                **  Write text into screen_map buffer . . .
                */

                row = M4/100;
                col = M4%100;
                if ( bs_window ) {
                        ++row;
                }

                for (k=0; k<=strlen(response_field)-1; k++) {
                   screen_map[row][col+k] = response_field[k];
                   if ( col+k >= 80 ) {       /* wrap text at end of lines */
                      ++row;
                      col=1;
                   }
                }

                if ( strlen ( prompt[M1].contents ) != 0 ) {   /* table-->WS */

                   /*
                   **  Write text into screen_map buffer . . .
                   */

                   row = M4/100;
                   col = M4%100;
                   if ( bs_window ) {
                           ++row;
                   }

                   for (k=0; k<=strlen(prompt[M1].contents)-1; k++) {
                      screen_map[row][col+k] = prompt[M1].contents[k];
                      if ( col+k >= 80 ) {       /* wrap text at end of lines */
                         ++row;
                         col=1;
                      }
                   }
                } /* end if display_response */

                if ( (main_m == LIABILITIES) &&
                              ( M1 != 1547 ) &&
                              ( M1 != 1600 ) ) {
                        pause_flag_only = FALSE;
                }
                else if ( ( main_m == CONSUMER ) &&
                          ( M1 != 1547 ) &&
                          ( M1 != 1254 ) ) {
                        pause_flag_only = FALSE;
                }
                if (screen_map[row][col] == '_') {
                        /*
                        // figure out if it has a table item attached.
                        */
                        strcpy(table_loc, "    ");
                        has_table = FALSE;
                        if (inputline.L3 < 0) {
                                oldM1 = M1;
                                if (fgets(inline, MAX_LINE_LENGTH, fp_input) !=
                                      NULL) {
                                        read_bancstar_source_code(inline,
                                                                  &inputline);
                                        M1 = abs ( inputline.L1 );
                                        M2 = abs ( inputline.L2 );
                                        M3 = abs ( inputline.L3 );
                                        M4 = abs ( inputline.L4 );
                                        if (( M1 > 30000 ) && ( M2 == oldM1 )) {
                                                sprintf( table_loc,"%4u", M3);
                                                has_table = TRUE;
                                        }
                                        else {
                                                sprintf( table_loc, "    ");
                                                has_table = FALSE;
                                        }
                                }
                                rewind(fp_input);
                                for (long_i=0; long_i<linenum; long_i++) {
                                        fgets( inline,
                                               MAX_LINE_LENGTH,
                                               fp_input );
                                }
                                read_bancstar_source_code(inline, &inputline);
                                M1 = abs ( inputline.L1 );
                                M2 = abs ( inputline.L2 );
                                M3 = abs ( inputline.L3 );
                                M4 = abs ( inputline.L4 );
                        }
                        strcpy(helpfile, "   ");                /* 3 blanks */
                        has_help = FALSE;
                        if ( file_vector[M1/8] & (0x01 << (M1%8)) != 0 ) {
                                strcpy(helpfile, "YES");
                                has_help = TRUE;
                        }
                        sprintf(short_line, "%d/%d", M3, prompt[M1].length);
                        while (strlen(short_line) < 5) {
                                strcat(short_line, " ");
                        }
                        fprintf(fp_temp,
                                "\n   %4u  %-7.7s  %s  %-38.38s  %s   %s",
                                M1, prompt_type[prompt[M1].type], short_line,
                                prompt[M1].name, table_loc, helpfile);
                        ++report_length;
                        if ((report_length > 30) || ((report_length%60) == 0)){
                                fprintf(fp_temp, "\f");
                                fprintf(fp_temp, "\n");
                                report_length = 0;
                                sprintf(text_line, "   Num   Type     Len    Prompt                                  Table  Help");
                                set_in_boldface(text_line);
                                fprintf(fp_temp, "\n%s", text_line);
                                sprintf(text_line, "   ----  -------  -----  --------------------------------------  -----  ----");
                                set_in_boldface(text_line);
                                fprintf(fp_temp, "\n%s", text_line);
                        }
                        /*
                        // 04-03-93: if this is a "blank" prompt on the screen,
                        // label it appropriately (laser-printed version only).
                        */
                        if ( laser_option ) {
                                draw_tiny_label( M1, row, col, bs_window,
                                                 has_table, has_help);
                        }
                } /* end if prompt-to-be-labeled */
             } /* end if display_response */
          } /* end if display_prompt */

          if ( inputline.L4 < 0 ) {         /* then we are at end of page */
             /*
             **  Write the entire contents of the screen_map out to file,
             **  followed by the prompt listing report, then clear out
             **  the screen map for the next page.
             */
             for (row=1; row<=25; row++) {
                for (col=1; col<=80; col++)
                   fputc(screen_map[row][col], fp_output);
                fprintf(fp_output, "\n");
             }
             rewind(fp_temp);
             if ( pause_flag_only ) {
                fprintf(fp_output, "\n");
                fprintf(fp_output, "\nNo displayable prompts on this page.\n");
             }
             else {
                while(fgets(text_line, MAX_LINE_LENGTH, fp_temp)) {
                        fprintf(fp_output, "%s", text_line);
                }
             }
             fclose(fp_temp);
             report_length = 0;
             for (i=1; i<=25; i++)
                for (j=1; j<=80; j++) {
                   screen_map[i][j] = ' ';
             }
             bs_window = FALSE;
             pause_flag_only = TRUE;
             ++pagenum;
             if ( linenum >= total_number_of_lines )
                goto skip_empty_last_page;
             fp_temp = fopen(TEMPFILE, "w+");   /* open temp file */
             if (fp_temp == NULL) {
                  perror("LIST");
                  fcloseall();
                  return(-1);
             }
             fprintf(fp_temp, "\n");
             sprintf(text_line, "   Num   Type     Len    Prompt                                  Table  Help");
             set_in_boldface(text_line);
             fprintf(fp_temp, "\n%s", text_line);
             sprintf(text_line, "   ----  -------  -----  --------------------------------------  -----  ----");
             set_in_boldface(text_line);
             fprintf(fp_temp, "\n%s", text_line);

             fprintf(fp_output, "\f");

             /*
             **  Draw a frame around text area on the next page.
             */

             if ( laser_option ) {
                fputc(27,  fp_output);     /* set cursor position to upper left */
                fputc(91,  fp_output);
                fputc(81,  fp_output);
                fputc(6,   fp_output);     /* 6-byte command */
                fputc(0,   fp_output);
                fputc(2,   fp_output);     /* unsigned displacement from (0,0) */
                fputc(4,   fp_output);     /* 1/300 inch increments */
                fputc(0,   fp_output);     /* horizontal byte high */
                fputc(1,   fp_output);     /* horizontal byte low */
                fputc(0,   fp_output);     /* vertical byte high */
                fputc(140, fp_output);     /* vertical byte low */

                fputc(27,  fp_output);     /* draw a black box */
                fputc(91,  fp_output);
                fputc(105, fp_output);
                fputc(7,   fp_output);     /* 7-byte command */
                fputc(0,   fp_output);
                fputc(1,   fp_output);     /* unit = 1/300 inch */
                fputc(0,   fp_output);     /* write mode OR */
                fputc(9,   fp_output);     /* rectangle width high byte */
                fputc(96,  fp_output);     /* rectangle width low byte */
                fputc(4,   fp_output);     /* rectangle height high byte */
                fputc(221, fp_output);     /* rectangle height low byte */
                fputc(1,   fp_output);     /* use pattern black */

                fputc(27,  fp_output);     /* move cursor (+2,+2) relative */
                fputc(91,  fp_output);
                fputc(81,  fp_output);
                fputc(6,   fp_output);     /* 6-byte command */
                fputc(0,   fp_output);
                fputc(3,   fp_output);     /* relative move from current position */
                fputc(4,   fp_output);     /* 1/300 inch increments */
                fputc(0,   fp_output);     /* horizontal byte high */
                fputc(2,   fp_output);     /* horizontal byte low */
                fputc(0,   fp_output);     /* vertical byte high */
                fputc(2,   fp_output);     /* vertical byte low */

                fputc(27,  fp_output);     /* draw a white box */
                fputc(91,  fp_output);
                fputc(105, fp_output);
                fputc(7,   fp_output);     /* 7-byte command */
                fputc(0,   fp_output);
                fputc(1,   fp_output);     /* unit = 1/300 inch */
                fputc(3,   fp_output);     /* write mode XOR */
                fputc(9,   fp_output);     /* rectangle width high byte */
                fputc(92,  fp_output);     /* rectangle width low byte */
                fputc(4,   fp_output);     /* rectangle height high byte */
                fputc(217, fp_output);     /* rectangle height low byte */
                fputc(0,   fp_output);     /* use previous pattern */

                fputc(27,  fp_output);     /* set cursor position to upper left */
                fputc('[', fp_output);
                fputc('Q', fp_output);
                fputc(6,   fp_output);
                fputc(0,   fp_output);
                fputc(2,   fp_output);     /* unsigned displacement from (0,0) */
                fputc(4,   fp_output);     /* 1/300 inch increments */
                fputc(0,   fp_output);     /* horizontal byte high */
                fputc(0,   fp_output);     /* horizontal byte low */
                fputc(0,   fp_output);     /* vertical byte high */
                fputc(0,   fp_output);     /* vertical byte low */
             } /* end if laser_option */

             /*
             **  Print page header for the next page.
             */
             sprintf ( text_line, "\n%02d-%02d-%04d", startdate.month,
                                                      startdate.day,
                                                      startdate.year );
             for (i=11; i<=40-strlen(name)/2; i++)
                strcat(text_line, " ");
             strcat(text_line, name);
             for (i=40+strlen(name)/2+1; i<=80-8; i++)
                strcat(text_line, " ");
             sprintf(short_line, "Page %d\n", pagenum);
             strcat(text_line, short_line);
             set_in_boldface(text_line);
             fprintf(fp_output, "%s", text_line);
             fprintf(fp_output, "\n\n");     /* set to top of box */
          } /* end of if end_of_page */
     } /* end of main CONVERT while loop. */

     skip_empty_last_page:          /* used to avoid printing an extra page */

     fclose(fp_input);
     fclose(fp_main);

bail:
     for (i=1; i<=1000; i++) {
        free ( table[i].description );
        free ( table[i].item );
     }

#ifdef DIAGNOSTCS
     fprintf(stderr, "Exit SCREEN PRINT\n");
#endif

     remove(TEMPFILE);
     return(0);
}

/******************************************************************************
**
** draw_tiny_label:  use the IBM PPDS include-plot-commands mode to draw a
**                   very small four-digit prompt label on the page at the
**                   indicated row and column position.
**
** created:     04-03-93
**
** modified:    04-03-93
**
** programmer:  J. Loughry
**
** notes:       I kept running out of printer memory (I think) when trying
**              to do this using rule-and-fill.  Maybe the HP 7550A plotter
**              emulation mode will be a little less memory-intensive.
**
**              The HP plotter mode has frustrating limits on how close to
**              the edge of the paper it will let me plot!
**
******************************************************************************/

int draw_tiny_label( unsigned prompt_number, int row, int column,
                     boolean bs_window, boolean table, boolean help ) {
        char plotter_string[256];
        char superscript[40];
        char bogus[10];
        long x, y;
        int i;
        fclose(fp_output);
        if ((fp_output = fopen(LISTFILE, "ab")) == NULL) {
                perror("LIST -- reopening fp_output in L_SCNPRT");
                fclose(fp_output);
                goto bail;
        }
        fputc(27,  fp_output);          /* include-plot-commands */
        fputc(91,  fp_output);
        fputc(109, fp_output);
        fputc(89,  fp_output);          /* total length of string */
        fputc(0,   fp_output);
        fputc(9,   fp_output);          /* number of parameter bytes */
        fputc(193, fp_output);          /* mode */
        fputc(0,   fp_output);          /* X1 parameter */
        fputc(0,   fp_output);
        fputc(0,   fp_output);          /* Y1 parameter */
        fputc(0,   fp_output);
        fputc(40,  fp_output);          /* X2 parameter */
        fputc(0,   fp_output);
        fputc(52,  fp_output);          /* Y2 parameter */
        fputc(0,   fp_output);
        if ( bs_window ) {
                --row;
        }
        x = ((long)(column + 1) * 1016) / 10 - 320;
        y = 5558 + (((long)(24 - row + 1) * 1016) / 6) + 1110;
        if ( bs_window ) {
                y -= 40;
        }
        strcpy(superscript, "");
        if ( table ) {
                strcat(superscript, "T");
        }
        if ( help ) {
                strcat(superscript, "H");
        }
        while ( strlen(superscript) < 2 ) {
                strcat(superscript, " ");
        }
        sprintf(bogus, "%u", prompt_number);
        for (i=0; i < (4-strlen(bogus)); i++) {
                strcat(superscript, " ");
        }
        sprintf(plotter_string,
                "IN;SP1;PT0.1;SI0.08,0.12;PU%06ld,%06ld;LB%u%c;PU;SI0.07,0.09;PR0,60.0;LB%s%c;",
                x, y, prompt_number, 0x03, superscript, 0x03 );
        fputs(plotter_string, fp_output);
        /*
        printf("\nstrlen(plotter_string) = %u -- total length of string in header above should be this + 10", strlen(plotter_string));
        */
        fclose(fp_output);
        if ((fp_output = fopen(LISTFILE, "a")) == NULL) {
                perror("LIST -- reopening fp_output in L_SCNPRT");
                fclose(fp_output);
                goto bail;
        }
bail:
        return(0);
}


#include "l_extern.h"

#define DATAFIELD       0x5F
#define BACKGROUND      0x1E

/*****************************************************************************
**                                                                          **
**  CONVERT:    convert from BANCStar screen file format to BRANCHBANKER    **
**              data entry screen format (hexadecimal binary).              **
**                                                                          **
**  input:      any .SCN or .FRM file, plus a prompt file.                  **
**                                                                          **
**  output:     a series of BRANCHBANKER data entry screen files, in the    **
**              proper hexadecimal format.  Since a single BANCStar         **
**              screen file can contain several pages, the result will      **
**              be a series of BRANCHBANKER screen files, each containing   **
**              the text (but no logic) of one of the BANCSTAR screen       **
**              pages.  All constants, names of prompts, and table          **
**              constants loaded into working storage for display on the    **
**              screen will (hopefully) be preserved.                       **
**                                                                          **
**  programmer: Joe Loughry                                                 **
**                                                                          **
**  created:    9/13/91                                                     **
**                                                                          **
**  modified:   05/10/93                                                    **
**                                                                          **
**  notes:      Generates a binary image of the screen and writes it to     **
**              the indicated file for further processing.                  **
**                                                                          **
**              05-05-93: added code to eliminate duplicate prompt numbers  **
**              in converted BRANCHBANKER screen files (can cause problems  **
**              with the data entry screen tailor later on).                **
**                                                                          **
**              05-08-93: convert BS promptfile to BB data dictionary;      **
**              prompt names and types only.  Conversion of helpfiles and   **
**              DD comments to come at a later time.                        **
**                                                                          **
**              05-09-93: removed annoying bright red "message" box to      **
**              inform user that we were writing a file.                    **
**                                                                          **
**              05-09-93: changed name of output file set from              **
**              LISTnnnn.SCN to simply nnnn.SCN.  This was done to simplify **
**              batch operations (we're limited as to just how smart a      **
**              DOS batch file can be about comparing different filenames), **
**                                                                          **
**              05-10-93: changed colors to match Annie's specifications.   **
**                                                                          **
*****************************************************************************/

int convert ( char *name, Promptfile prompt, Sellstar_system main_m )
{
     int read_bancstar_source_code ( char *, struct sourceline * );
     int get_display_prompt ( char *, Promptfile, int, long );
     int create_window ( Window, char *, short, long, short );
     int destroy_windows ( void );
     int read_in_main_dot_m ( Sellstar_system, MainDotM );
     int get_table_item ( char *, MainDotM, User_table, int );

     char inline[MAX_LINE_LENGTH];         /* input line of BANCStar code    */
     struct sourceline inputline;          /* four fields of the input line  */
     char name_of_prompt[MAX_LINE_LENGTH]; /* displayed name of prompt       */
     char response_field[MAX_LINE_LENGTH]; /* displayed repsonse fields (BB) */
     char table_string[MAX_LINE_LENGTH];   /* contents of table lookup -->WS */

     MainDotM table;

     unsigned int M1, M2, M3, M4;          /* absolute values of code fields */

     int     i, j, k, c;                   /* general purpose integer vars   */
     boolean bs_window;                    /* displaying a BANCStar "window" */
     boolean pause_flag_only;              /* is this an "interesting" page? */
     short   bs_window_color;              /* foreground color of window     */
     long    bs_window_bk_color;           /* background color of window     */

     long linenum = 1;                     /* current line number            */
     long pagenum = 1;                     /* BANCStar code page number      */
     long total_number_of_lines = 0;       /* length of the file in lines    */

     unsigned char text_map[25][80];       /* map of text on BB screen       */
     unsigned char color_map[25][80];      /* map of colors on BB screen     */

     char BB_filename[MAX_LINE_LENGTH];    /* converted BRANCHBANKER path    */
     char BB_name[MAX_LINE_LENGTH];        /* BRANCHBANKER filename          */
     char BB_file2[MAX_LINE_LENGTH];       /* ??? */
     FILE *fp_BB;                          /* intermediate output file       */
     int BS_prompt[128];                   /* list of data entry fields/len. */
     int BS_index = 0;                     /* index into above list          */
     int duplicate_found = 0;              /* keeps duplicates out of list ^ */

     Window wd;                            /* text window descriptor         */
     char outtext_line[MAX_LINE_LENGTH];   /* buffer for _outtext function   */

     int left_space_clear;                 /* the following quantities       */
     int right_space_clear;                /* are used by the heuristic      */
     int top_space_clear;                  /* window placing and moving      */
     int bottom_space_clear;               /* algorithm.                     */
     int horizontal_window_coordinate;     /* Used for placing windows.      */
     int vertical_window_coordinate;       /*               "                */
     int width_of_window;                  /*               "                */
     int height_of_window;                 /*               "                */

     int bs_window_top = 0;                /* coordinates of BANCStar        */
     int bs_window_bottom = 0;             /* window currently on screen     */

     char *fgets_rc;                       /* return code from line input    */

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

     fclose(fp_input);

     /*
     **  Correct the value of total_number_of_lines for boundary conditions.
     */

     total_number_of_lines -= 1;                   /* fencepost error */

     /*
     **  Re-open the file to be read.
     */

     if ((fp_input = fopen(name, "r")) == NULL)
     {
          perror("LIST");
          fclose(fp_input);
          return(-1);
     }

     linenum = 0;
     pagenum = 1;
     bs_window = FALSE;
     pause_flag_only = TRUE;

     for (i=0; i<25; i++)
        for (j=0; j<80; j++) {
           text_map[i][j] = ' ';
           color_map[i][j] = BACKGROUND;   /* normal screen background color */
     }

     for (BS_index=0; BS_index<128; BS_index++)
        BS_prompt[BS_index] = 0x0000;
     BS_index = 0x0000;


     _setbkcolor ( BLUE );
     _clearscreen ( _GCLEARSCREEN );
     _settextcolor ( BRIGHT_WHITE );

     /*
     **  Main CONVERT while loop:
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

          left_space_clear = 40;
          right_space_clear = 40;
          top_space_clear = 12;
          bottom_space_clear = 12;

          /*
          **  If this is a BANCStar window, then display it.
          */

          if ( M1 == 8000 ) {
             bs_window_color = M2%16;
             bs_window_bk_color = (long)(M2/16);
             _settextcolor ( bs_window_color );
             _setbkcolor ( bs_window_bk_color );
             _settextwindow ( M3/100, M3%100, M4/100, M4%100 );
             _clearscreen ( _GWINDOW );
             _settextwindow ( 1, 1, 24, 80 );

             for (i=(M3/100)-1; i<=(M4/100)-1; i++)
                for (j=(M3%100)-1; j<=(M4%100)-1; j++) {
                   text_map[i][j] = ' ';
                   color_map[i][j] = (char)M2;
             }

             left_space_clear = M3%100;
             right_space_clear = 80 - M4%100;
             top_space_clear = M3/100;
             bottom_space_clear = 24 - M4/100;

             bs_window_top = M3/100;
             bs_window_bottom = M4/100;

             bs_window = TRUE;
          }

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
                prompt[(int)(M1-30000)].contents = (char *)malloc(sizeof(table_string));
                strcpy ( prompt[(int)(M1-30000)].contents, table_string );
             }
             else if ( M4 == 108 ) {
                get_table_item ( table_string, table, DESCRIPTION, M3 );
                prompt[(int)(M1-30000)].contents = (char *)malloc(sizeof(table_string));
                strcpy ( prompt[(int)(M1-30000)].contents, table_string );
             }
          }

          /*
          **  If it is a display prompt, then paint it on the screen.
          */

          if ( ( inputline.L1 > 0 ) && ( inputline.L1 <= 2000 ) ) {
             get_display_prompt ( name_of_prompt, prompt, M1, linenum );

             if ( M2 != 0 ) {    /* then we display the name */
                if ( bs_window ) {
                   _settextcolor ( bs_window_color );
                   _setbkcolor ( bs_window_bk_color );
                }
                else {
                   _settextcolor ( BRIGHT_WHITE );
                   _setbkcolor ( BLUE );
                }
                _settextposition ( M2/100, M2%100 );
                _outtext ( name_of_prompt );

                for (i=0; i<strlen(name_of_prompt); i++) {
                        text_map[(M2/100)-1][(M2%100)+i-1] = name_of_prompt[i];
                }
                left_space_clear = min ( left_space_clear, ( M2 % 100 ) );
                right_space_clear = max ( right_space_clear,
                                          ((M2%100)+strlen(name_of_prompt)) );
             }

             if ( M3 != 0 ) {                      /* response field . . . */
                strcpy( response_field, "" );
                for (i=1; i<=M3; i++)
                   strcat ( response_field, "_" );
                _settextposition ( M4/100, M4%100 );
                if ( bs_window ) {
                   _settextcolor ( bs_window_color );
                   _setbkcolor ( bs_window_bk_color );
                }
                else {
                   _settextcolor ( WHITE );
                   _setbkcolor ( BLUE );
                }
                _outtext ( response_field );

                for (i=0; i<strlen(response_field); i++) {
                        text_map[(M4/100)-1][(M4%100)+i-1] = response_field[i];
                }
                left_space_clear = min ( left_space_clear, ( M4 % 100 ) );
                right_space_clear = max ( right_space_clear,
                                          ((M4%100)+strlen(response_field)) );

                if ( strlen ( prompt[M1].contents ) != 0 ) {   /* table-->WS */
                   _settextposition ( M4/100, M4%100 );
                   if ( bs_window ) {
                      _settextcolor ( bs_window_color );
                      _setbkcolor ( bs_window_bk_color );
                   }
                   else {
                      _settextcolor ( WHITE );
                      _setbkcolor ( BLUE );
                   }
                   _outtext ( prompt[M1].contents );

                   for (i=0; i<strlen(prompt[M1].contents); i++) {
                           text_map[(M4/100)-1][(M4%100)+i-1] = prompt[M1].contents[i];
                   }
                   left_space_clear = min ( left_space_clear, ( M4 % 100 ) );
                   right_space_clear = max ( right_space_clear,
                                      ((M4%100)+strlen(prompt[M1].contents)) );
                }
                else {       /* encode it as a data field for BRANCHBANKER */
                   if ( M3 != 0 ) {
                        duplicate_found = 0;
                        for (k=0; k<=BS_index; k+=2) {
                           if (BS_prompt[k] == M1) {
                                duplicate_found = 1;
                                break;
                           }
                        }
                        if (!duplicate_found) {
                           for (i=0; i<strlen(response_field); i++) {
                                text_map[(M4/100)-1][(M4%100)+i-1] = ' ';
                                color_map[(M4/100)-1][(M4%100)+i-1] = DATAFIELD;
                           }
                           BS_prompt[BS_index] = M1;
                           ++BS_index;
                           BS_prompt[BS_index] = (int)strlen(response_field);
                           ++BS_index;
/*
printf("\n%04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X",
        BS_prompt[0], BS_prompt[1], BS_prompt[2], BS_prompt[3],
        BS_prompt[4], BS_prompt[5], BS_prompt[6], BS_prompt[7] );
*/
                           left_space_clear = min ( left_space_clear,
                                                    ( M4 % 100 ) );
                           right_space_clear = max ( right_space_clear,
                                       ((M4%100)+strlen(prompt[M1].contents)) );
                        }
                        else {
                           for (i=0; i<strlen(response_field); i++) {
                                text_map[(M4/100)-1][(M4%100)+i-1] = '?';
                                /*
                                color_map[(M4/100)-1][(M4%100)+i-1] = DATAFIELD;
                                */
                           }
                        }
                   }
                }
                if ( ( M1 != 1547 ) && ( M1 != 1254 ) )
                   pause_flag_only = FALSE;

                if ( ( main_m != CONSUMER ) && ( M1 != 1600 ) )
                   pause_flag_only = FALSE;
             }
          }

          if ( inputline.L4 < 0 ) {         /* then we are at end of page */

             /*
             **  Paint the familiar BANCStar headers and footers.
             */

             _settextwindow ( 1, 1, 25, 80 );
             _settextcolor ( BLUE );
             _setbkcolor ( WHITE );
             strcpy ( outtext_line, "  LIST  Conversion  ENTRY  " );
             _settextposition ( 1, 40-strlen(outtext_line)/2 );
             _outtext ( outtext_line );

             _settextcolor ( WHITE );
             _setbkcolor ( BLUE );
             _settextposition ( 1, 70 );
             sprintf ( outtext_line, "PAGE %d", pagenum );
             _outtext ( outtext_line );

             _settextcolor ( BLUE );
             _setbkcolor ( WHITE );

             _settextposition ( 25, 2 );
             strcpy ( outtext_line, "1 MENU ");
             _outtext ( outtext_line );

             _settextposition ( 25, 10 );
             strcpy ( outtext_line, "2 PRT SC");
             _outtext ( outtext_line );

             _settextposition ( 25, 19 );
             strcpy ( outtext_line, " 3 DONE ");
             _outtext ( outtext_line );

             _settextposition ( 25, 38 );
             strcpy ( outtext_line, "5 D-MENU");
             _outtext ( outtext_line );

             _settextposition ( 25, 47 );
             strcpy ( outtext_line, "6PREV PG");
             _outtext ( outtext_line );

             _settextposition ( 25, 73 );
             strcpy ( outtext_line, "9SALES");
             _outtext ( outtext_line );

             /*
             **  Pop up a window on the screen.
             */

             wd.row1 = 9;
             wd.col1 = 33;
             wd.row2 = 15;
             wd.col2 = 71;

             /*
             **  Find the best place for the window.
             */

             height_of_window = abs ( wd.row2 - wd.row1 + 1 );
             width_of_window = abs ( wd.col2 - wd.col1 + 2 );

             if ( ( top_space_clear > ( 24 - bottom_space_clear ) ) &&
                  ( top_space_clear >= height_of_window ) ) {
                wd.row1 = top_space_clear - ( height_of_window / 2 );
                wd.row2 = top_space_clear - ( height_of_window / 2 )
                                           + height_of_window;
             }

             if ( ( top_space_clear <= ( 24 - bottom_space_clear ) ) &&
                  ( ( 24 - bottom_space_clear ) >= height_of_window ) ) {
                wd.row1 = ( 24 - bottom_space_clear - height_of_window ) / 2
                                                     + bottom_space_clear;
                wd.row2 = ( 24 - bottom_space_clear - height_of_window ) / 2
                                                     + bottom_space_clear
                                                     + height_of_window;
             }

             horizontal_window_coordinate = ( ( right_space_clear -
                                                left_space_clear ) / 2 )
                                              + right_space_clear;

             if ( ( horizontal_window_coordinate > ( 80 - width_of_window ) ) ||
                  ( horizontal_window_coordinate < 0 ) ) {
                horizontal_window_coordinate = 33;
                wd.col1 = horizontal_window_coordinate;
                wd.col2 = horizontal_window_coordinate + width_of_window - 2;
             }

             vertical_window_coordinate = wd.row1;

             if ( ( vertical_window_coordinate > ( 24 - height_of_window ) ) ||
                  ( vertical_window_coordinate < 0 ) ) {
                vertical_window_coordinate = 9;
                wd.row1 = vertical_window_coordinate;
                wd.row1 = vertical_window_coordinate + height_of_window - 1;
             }

             if ( bs_window ) {
                if ( bs_window_top > height_of_window ) {
                   wd.row1 = 3;
                   wd.col1 = 33;
                   wd.row2 = 9;
                   wd.col2 = 71;
                }

                if ( ( bs_window_bottom + height_of_window ) < 22 ) {
                   wd.row1 = bs_window_bottom + 1;
                   wd.col1 = 33;
                   wd.row2 = bs_window_bottom + height_of_window + 1;
                   wd.col2 = 71;

                   if ( wd.row1 <= 12 ) {
                      wd.row1 += 3;
                      wd.row2 += 3;
                   }
                }
                else {
                   wd.row1 = 7;
                   wd.col1 = 33;
                   wd.row2 = 13;
                   wd.col2 = 71;
                }
             }

/*
             _settextposition(1,1);

             fprintf(stdout, "bs_window_top =    %d\n", bs_window_top);
             fprintf(stdout, "bs_window_bottom = %d\n", bs_window_bottom);
             fprintf(stdout, "wd.row1           =  %d\n", wd.row1);
             fprintf(stdout, "wd.col1           =  %d\n", wd.col1);
             fprintf(stdout, "wd.row2           =  %d\n", wd.row2);
             fprintf(stdout, "wd.col2           =  %d\n", wd.col2);

             if ( bs_window )
                fprintf(stdout, "bs_window=TRUE\n");
             else
                fprintf(stdout, "bs_window=FALSE\n");
*/

             create_window ( wd, name, MAGENTA, WHITE, BLACK );

             sprintf ( outtext_line, " This is page %d.  The corresponding\n",
                       pagenum );
             _settextposition(2,2);
             _outtext ( outtext_line );
             _outtext ( "  BRANCHBANKER screen file will be:\n\n" );
/*
             if ( pause_flag_only )
                strcpy ( outtext_line, "  (Nothing much here.)\n\n" );
             else
                strcpy ( outtext_line, "\n\n" );
             _outtext ( outtext_line );
*/
             strcpy ( BB_filename, " " );
             strcat ( BB_filename, CONVERSION );
             /*
             // 05-09-93: we used to call the output file LISTnnnn.scn
             //         Now we call it 1nnn.SCN
             */
             sprintf( BB_name, "%04d.SCN", 1000 + pagenum );
             strcat ( BB_filename, BB_name );
             strcat ( BB_filename, " " );
             strupr ( BB_filename );

             strcpy( BB_file2, CONVERSION );
             strcat( BB_file2, BB_name );

             for ( i=wd.col1;
                   i<=(wd.col2-wd.col1)/2-strlen(BB_filename)/2+wd.col1;
                   i++ )
                      _outtext ( " " );

             _setbkcolor ( CYAN );
             _settextcolor ( BLACK );

             _outtext ( BB_filename );

             _setbkcolor ( WHITE );
             _settextcolor ( BLACK );
             _outtext ( "\n\n" );

             _outtext ( " Convert to BRANCHBANKER format? (Y/N)" );

             c = getch();

             if ( ( c == 'Y' ) || ( c == 'y' ) ) {
                /*
                **  Write converted file to disk.
                */
/*
                wd.row1 = 11;
                wd.col1 = 18;
                wd.row2 = 17;
                wd.col2 = 55;
                create_window ( wd, "Message:", BRIGHT_BLUE, RED, BRIGHT_RED );

                _outtext ( "\n\n\n        Writing file . . .");
*/
                while(BS_index < 128) {
                        BS_prompt[BS_index] = 0;
                        ++BS_index;
                }
                if ((fp_BB=fopen(BB_file2, "wb")) == NULL) {
                        perror("Error opening BB_file2");
                        fclose(fp_BB);
                        goto quit;
                }
                fwrite(BS_prompt, sizeof(int), 128, fp_BB);
                fwrite(text_map, sizeof(char), 2000, fp_BB);
                fwrite(color_map, sizeof(char), 2000, fp_BB);
                fclose(fp_BB);
             }

             if ( c == 27 ) {
quit:
                wd.row1 = 12;
                wd.col1 = 20;
                wd.row2 = 16;
                wd.col2 = 57;
                create_window ( wd, "Quit", BRIGHT_BLUE, RED, BLACK );

                _outtext ( "\n\n Are you sure you want to exit? (Y/N)");
                console_input = getch();

                if ( ( console_input == 13 ) || ( console_input == 27 ) ||
                     ( console_input == 'Y') || ( console_input == 'y' ) )
                {
                   fclose(fp_input);
                   fclose(fp_output);
                   fclose(fp_prompt);
                   fclose(fp_check);
                   fclose(fp_main);

                   destroy_windows();

                   _setbkcolor ( BLACK );
                   _settextcolor ( WHITE );
                   _clearscreen ( _GCLEARSCREEN );

                   exit(-2);
                }
             }

             bs_window = FALSE;
             pause_flag_only = TRUE;
             ++pagenum;
             destroy_windows();

             _setbkcolor ( BLUE );
             _clearscreen ( _GCLEARSCREEN );
             _settextcolor ( BRIGHT_WHITE );

             for (i=0; i<25; i++)
                for (j=0; j<80; j++) {
                   text_map[i][j] = ' ';
                   color_map[i][j] = BACKGROUND;  /* normal screen background color */
             }
             for (BS_index=0; BS_index<128; BS_index++)
                BS_prompt[BS_index] = 0x0000;
             BS_index = 0;

             /*
             **  To avoid screen flicker, repaint the headers here.
             */

             _settextwindow ( 1, 1, 25, 80 );
             _settextcolor ( BLUE );
             _setbkcolor ( WHITE );
             strcpy ( outtext_line, "  LIST  Conversion  ENTRY  " );
             _settextposition ( 1, 40-strlen(outtext_line)/2 );
             _outtext ( outtext_line );

             _settextcolor ( WHITE );
             _setbkcolor ( BLUE );
             _settextposition ( 1, 70 );
             sprintf ( outtext_line, "PAGE %d", pagenum );
             _outtext ( outtext_line );

             _settextcolor ( BLUE );
             _setbkcolor ( WHITE );

             _settextposition ( 25, 2 );
             strcpy ( outtext_line, "1 MENU ");
             _outtext ( outtext_line );

             _settextposition ( 25, 10 );
             strcpy ( outtext_line, "2 PRT SC");
             _outtext ( outtext_line );

             _settextposition ( 25, 19 );
             strcpy ( outtext_line, " 3 DONE ");
             _outtext ( outtext_line );

             _settextposition ( 25, 38 );
             strcpy ( outtext_line, "5 D-MENU");
             _outtext ( outtext_line );

             _settextposition ( 25, 47 );
             strcpy ( outtext_line, "6PREV PG");
             _outtext ( outtext_line );

             _settextposition ( 25, 73 );
             strcpy ( outtext_line, "9SALES");
             _outtext ( outtext_line );
          }
     } /* end of main CONVERT while loop. */

     wd.row1 = 11;
     wd.col1 = 23;
     wd.row2 = 16;
     wd.col2 = 56;
     create_window ( wd, "Exit", BRIGHT_BLUE, GREEN, BRIGHT_WHITE );

     _outtext ( "\n\n  All done.  Hit any key to exit.");
     _outtext (   "\n                ");

     console_input = getch();
     destroy_windows();

     fclose(fp_input);
     fclose(fp_main);

     for (i=1; i<=1000; i++) {
        free ( table[i].description );
        free ( table[i].item );
     }

     _setbkcolor ( BLACK );
     _settextcolor ( WHITE );
     _clearscreen ( _GCLEARSCREEN );

     return(0);
}


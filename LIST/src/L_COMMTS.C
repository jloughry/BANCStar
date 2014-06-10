
#include "l_extern.h"

extern unsigned char con[], coff[], bold[], boff[], ital[], itoff[];

/*****************************************************************************
**                                                                          **
**  COMMENTS: assemble an illuminating commentary on the given line.        **
**                                                                          **
*****************************************************************************/

int comments( char *return_string, struct sourceline inline,
              Promptfile prompt, long line_number )
{
     int get_prompt( char *, Promptfile, int, long );
     int get_display_prompt( char *, Promptfile, int, long );
     int get_modified_prompt( char *, Promptfile, int, long );
     int set_in_boldface( char * );
     int set_in_condensed( char * );
     int set_in_italics( char * );
     char *numstring( int );
     char *long_numstring( long );

     char substring[MAX_LINE_LENGTH];       /* holding pattern for strings */

                                            /*******************************/
     char prompt_name[MAX_LINE_LENGTH];     /* various names of prompts    */
     char short_name_1[MAX_LINE_LENGTH];    /* found in the first,         */
     char short_name_2[MAX_LINE_LENGTH];    /*          second,            */
     char short_name_3[MAX_LINE_LENGTH];    /*       third,                */
     char short_name_4[MAX_LINE_LENGTH];    /* and fourth positions        */
                                            /* of the line.                */
     unsigned int M1, M2, M3, M4;           /*******************************/

     int operator_2, operand_2;       /* for example, in the integer value */
     int operator_3, operand_3;       /* 11547, the operator is "1" and    */
     int operator_4, operand_4;       /* the operand is "1547".            */

     double arithmetic_result;        /* the result of a line of constants */

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering COMMENTS\n");
#endif

     M1 = abs(inline.L1);             /* so we don't have to worry about   */
     M2 = abs(inline.L2);             /* minus signs.                      */
     M3 = abs(inline.L3);
     M4 = abs(inline.L4);

     strcpy(return_string, "");       /* will eventually hold the comment. */

     /*
     **  Displayed prompts are set in boldface:
     */

     if ( (M1 >= 1) && (M1 <= 2000) )
     {
          get_display_prompt( return_string, prompt, M1, line_number );
          set_in_boldface( return_string );

          /*
          **  Prompts on forms have justification information in M2.
          **  Since most of these will be "12", we decode all but those.
          */

          if ( strcmp( strupr( ext ), ".FRM" ) == 0 )   /* then it's a form */
          {
               if ( M2 != 12 )
               {
                    strcat(return_string, con);

                    /*
                    **  Format:
                    */

                    strcat(return_string, " ");

                    if ( ( M2 / 10 ) == 1 )
                    {
                         strcat(return_string, "(Unformatted[] ");
                    }
                    else if ( ( M2 / 10 ) == 2 )
                    {
                         strcat(return_string, "(Unformatted[,] ");
                    }
                    else if ( ( M2 / 10 ) == 3 )
                    {
                         strcat(return_string, "(Unformatted[.] ");
                    }
                    else
                    {
                         strcat(return_string, "(");
                    }

                    /*
                    **  Justification:
                    */

                    if ( ( M2 % 10 ) == 1 )
                    {
                         strcat(return_string, "right justified)");
                    }
                    else if ( ( M2 % 10 ) == 2 )
                    {
                         strcat(return_string, "left justified)");
                    }
                    else if ( ( M2 % 10 ) == 3 )
                    {
                         strcat(return_string, "next w/space)");
                    }
                    else if ( ( M2 % 10 ) == 4 )
                    {
                         strcat(return_string, "prompt left)");
                    }
                    else if ( ( M2 % 10 ) == 5 )
                    {
                         strcat(return_string, "right no print)");
                    }
                    else if ( ( M2 % 10 ) == 6 )
                    {
                         strcat(return_string, "next w/o space)");
                    }
                    else
                    {
                         strcat(return_string, ")");
                    }

                    /*
                    **  Return now if it's a form.
                    */

                    strcat(return_string, coff);
               }

               if ( inline.L1 < 0 )        /* minus sign on prompt number */
               {
                    strcat(return_string, " (N/A)");
                    strcat(return_string, coff);
               }

               return(0);
          }

          /*
          **  Minus sign in the third position indicates either
          **  an automatic table pop-up or an automatic help.
          */

          if (inline.L3 < 0)
          {
               strcat(return_string, con);
               strcat(return_string, " (automatic help)");
               strcat(return_string, coff);
          }

          /*
          **  No response field on this prompt (label only).
          */

          if (inline.L3 == 0)
          {
               strcat(return_string, con);
               strcat(return_string, " (no response)");
               strcat(return_string, coff);
               return(0);
          }

          /*
          ** Response only on this prompt (no name displayed).
          */

          if (inline.L2 == 0)
          {
               strcat(return_string, con);
               strcat(return_string, " (response only)");
               strcat(return_string, coff);
          }

          return(0);
     }

     /*
     **  Prompts referenced in arithmetic statements are set in italics.
     **  Other comments are set in condensed; prompt names are underlined.
     */

     /*
     **  Stop Code.
     */

     if ( M1 == 2999 )
     {
          strcpy(return_string, "Stop Code");
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  End Block Conditionals:
     */

     if ( (M1 == 3001) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
     {
          strcpy(return_string, "End Block");
          set_in_condensed(return_string);
          return(0);
     }

     if ( (M1 == 3101) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
     {
          strcpy(return_string, "[End Reverse Block]");
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Does anybody ever actually use these "Page Conditionals"?
     */

     if ( ( M1 >= 3000 ) && ( M1 <= 3101 ) )
     {
          if ( M3 >= 10 )
          {
               strcat(return_string, "Page ");
               M3 -= 10;
          }

          /*
          **  Generate an English description of the line.
          */

          if ( M1 == 3000 )
               strcat(return_string, "If ");

          if ( M1 == 3001 )
               strcat(return_string, "Block If ");

          if ( M1 == 3100 )
               strcat(return_string, "Proceed If ");

          if ( M1 == 3101 )
               strcat(return_string, "[Reverse Block] Proceed If ");

          /*
          **  Prompts mentioned in comments are always underlined.
          */

          get_prompt( short_name_2, prompt, M2, line_number );
          set_in_italics( short_name_2 );

          strcat( return_string, short_name_2 );

          /*
          **  Now the condition itself.
          */

          if ( ( M3 == 1 ) && ( M4 == 0 ) )
          {
               strcat(return_string, " is null");
               set_in_condensed(return_string);
               return(0);
          }

          if ( ( M3 == 2 ) && ( M4 == 0 ) )
          {
               strcat(return_string, " is not null");
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Depending on the prompt type, it may be either "less than"
          **  or "null."
          */

          if ( ( M3 == 1 ) &&
               ( ( prompt[M2].type != P_ALPHA ) || ( M4 > 30000 ) ) )
               strcat(return_string, " < ");
          else if ( M3 == 1 )
          {
               strcat(return_string, " is null");
               set_in_condensed(return_string);
               return(0);
          }

          if ( ( M3 == 2 ) &&
               ( ( prompt[M2].type != P_ALPHA ) || ( M4 > 30000 ) ) )
               strcat(return_string, " <= ");
          else if ( M3 == 2 )
          {
               strcat(return_string, " is not null");
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Number three is always equality.
          */

          if ( M3 == 3 )
               strcat(return_string, " == ");

          /*
          **  The others may or may not depend on the prompt's declared type.
          */

          if ( ( M3 == 4 ) &&
               ( ( prompt[M2].type != P_ALPHA ) || ( M4 > 30000 ) ) )
               strcat(return_string, " >= ");
          else if ( M3 == 4 )
               strcat(return_string, " <> ");

          if ( ( M3 == 5 ) &&
               ( ( prompt[M2].type != P_ALPHA ) || ( M4 > 30000 ) ) )
               strcat(return_string, " > ");
          else if ( M3 == 5 )
          {
               strcat(return_string, " until null");
               set_in_condensed(return_string);
               return(0);
          }

          if ( ( M3 == 6 ) &&
               ( ( prompt[M2].type != P_ALPHA ) || ( M4 > 30000 ) ) )
               strcat(return_string, " <> ");
          else if ( M3 == 6 )
          {
               strcat(return_string, " while null");
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Error condition: 4th parameter should never be less than 10,000.
          */

          if ( M4 < 10000 )
          {
               strcat(return_string, "???");
               set_in_condensed(return_string);

               sprintf(substring, "\n%s%s(Possible syntax error in line %ld)",
                                  coff,
                                  boff,
                                  line_number
               );
               strcat(return_string, substring);
               return(1);
          }

          /*
          **  Comparing to a constant integer.
          */

          if ( ( M4 >= 10000 ) && ( M4 < 20000 ) )
          {
               sprintf(substring, "%d", M4-10000);
               strcat(return_string, substring);
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Comparing to another prompt -- get its name.
          */

          if ( ( M4 > 20000 ) && ( M4 <= 22000 ) )
          {
               get_prompt( short_name_4, prompt, M4-20000, line_number );
               set_in_italics( short_name_4 );
               strcat( return_string, short_name_4 );
               set_in_condensed( return_string );
               return(0);
          }

          /*
          **  Comparing to an ASCII character -- display it if printable.
          */

          if ( ( M4 >= 30000 ) && ( M4 <= 30255 ) )
          {
               if ( isprint(M4 - 30000) )
               {
                    sprintf(substring, "'%c'", (char)(M4 - 30000));
                    strcat(return_string, substring);
                    set_in_condensed(return_string);
                    return(0);
               }
               else
               {
                    sprintf(substring, "' '");
                    strcat(return_string, substring);
                    set_in_condensed(return_string);
                    return(0);
               }
          }

          /*
          **  And if we haven't already, set the line in condensed print
          **  and return.
          */

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Draw commands.
     */

     if ( ( M1 >= 7000 ) && ( M1 <= 7999 ) )
     {
          sprintf(substring, "Draw picture #%d", M2 / 100 );
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Window commands.  First for forms, then for screens.
     */

     if ( ( M1 == 8000 ) && ( strcmp(strupr(ext), ".FRM") == 0 ) )
     {
          strcat(return_string, "(pause here for user)");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8000 )
     {
          sprintf(substring, "Window (%02d,%02d)-(%02d,%02d): color %d (%s on %s)",
                  M3 / 100,
                  M3 % 100,
                  M4 / 100,
                  M4 % 100,
                  M2,
                  bancstar_color[M2 % 16],
                  bancstar_color[M2 / 16]
          );
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Form printing commands.
     */

     if ( M1 == 8100 )
     {
          sprintf(substring, "Print text file %s%d.TXT%s at row %d, column %d",
                  ital,
                  M2,
                  itoff,
                  M3 / 10,
                  M4 / 10
          );
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Save Address and Goto commands.
     */

     if ( ( M1 == 8400 ) && ( M2 == 0 ) && ( M3 == 0 ) && ( M4 == 0 ) )
     {
          strcat(return_string, "Save Address");
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Goto a page.
     */

     if ( ( M1 == 8500 ) && ( M3 < 501 ) )
     {
          sprintf(substring, "Goto page %d", M3);
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Go to a Function Key.
     */

     if ( ( M1 == 8500 ) && ( M3 >= 501 ) && ( M3 <= 520 ) )
     {
          sprintf(substring, "Go to Function Key %d", M3 - 500);
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Less often used: go to a prompt.
     */

     if ( ( M1 == 8500 ) && ( M3 >= 1001 ) && ( M3 <= 3000 ) )
     {
          get_prompt( short_name_3, prompt, M3 - 1000, line_number );
          set_in_italics ( short_name_3 );
          sprintf( substring, "Go to Prompt #%d [%s]", M3 - 1000, short_name_3);
          strcat( return_string, substring );
          set_in_condensed( return_string );
          return( 0 );
     }

     /*
     **  And go to a particular transaction within a document module.
     */

     if ( ( M1 == 8500 ) && ( M3 >= 4001 ) && ( M3 <= 4024 ) )
     {
          sprintf(substring, "Go to Transaction %d", M3 - 4000);
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  And, of course, somebody had to use the weird one: Go to
     **  Product & Sales (P), Menu (M), Exit (E), Storage (S), or
     **  Multi-task (T) on the main (transaction) menu.
     */

     /*
     **  ASCII 80: ("P") Product & Sales:
     */

     if ( ( M1 == 8500 ) && ( M3 == 4080 ) )
     {
          sprintf(substring, "Go to Product & Sales");
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  ASCII 77: ("M") Menu:
     */

     if ( ( M1 == 8500 ) && ( M3 == 4077 ) )
     {
          sprintf(substring, "Go back to Menu");
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  ASCII 69: ("E") Exit:
     */

     if ( ( M1 == 8500 ) && ( M3 == 4069 ) )
     {
          sprintf(substring, "Exit BANCStar");
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  ASCII 83: ("S") Storage:
     */

     if ( ( M1 == 8500 ) && ( M3 == 4083 ) )
     {
          sprintf(substring, "Go to Storage screen");
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  ASCII 84: ("T") Multi-task:
     */

     if ( ( M1 == 8500 ) && ( M3 == 4084 ) )
     {
          sprintf(substring, "Go to Multi-task menu");
          strcat(return_string, substring);
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Our particular way of returning from a Branch & Link operation.
     */

     if ( ( M1 == 8550 ) && ( M2 == 0 ) && ( M3 == 11908 ) && ( M4 == 11909 ) )
     {
          /*
          **  Dummy code to keep the accounting correct.
          */

          get_prompt(substring, prompt, M3-10000, line_number);
          get_prompt(substring, prompt, M4-10000, line_number);

          strcat(return_string, "Return from subroutine");
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Combination goto.
     */

     if ( M1 == 8550 )
     {
          strcat(return_string, "Combination goto: Mod ");

          if ( M2 > 10000 )
          {
               get_prompt( short_name_2, prompt, M2 - 10000, line_number);
               set_in_italics( short_name_2 );
               strcat( return_string, short_name_2 );
               strcat( return_string, ", Doc " );
          }
          else
          {
               sprintf(substring, "%d, Tran %d, Doc ", (M2 / 100), (M2 % 100));
               strcat(return_string, substring);
          }

          if ( M3 > 10000 )
          {
               get_prompt(short_name_3, prompt, M3 - 10000, line_number );
               set_in_italics( short_name_3 );
               strcat( return_string, short_name_3 );
               strcat( return_string, ", Line " );
          }
          else
          {
               sprintf(substring, "%d, Page %d, Line ", ( M3 / 100 ),
                       ( M3 % 100 )
               );
               strcat(return_string, substring);
          }

          if ( M4 > 10000 )
          {
               get_prompt(short_name_4, prompt, M4 - 10000, line_number );
               set_in_italics( short_name_4 );
               strcat( return_string, short_name_4 );
          }
          else
          {
               sprintf(substring, "%d", M4);
               strcat(return_string, substring);
          }

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Miscellaneous commands.
     */

     if ( M1 == 8560 )
     {
          get_prompt( short_name_4, prompt, M4, line_number );
          set_in_italics( short_name_4 );
          sprintf( return_string, "DOS exec command %s", short_name_4 );
          set_in_condensed( return_string );
          return(0);
     }

     if ( M1 == 8600 )
     {
          strcat(return_string, "Activate F6 & F7");
          set_in_condensed(return_string);
          return(0);
     }

     if ( ( M1 >= 8601 ) && ( M1 <= 8607 ) )
     {
          strcat(return_string, "Deactivate F6 & F7");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8608 )
     {
          strcat(return_string, "Deactivate F2 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8609 )
     {
          strcat(return_string, "Deactivate F1 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8610 )
     {
          strcat(return_string, "Deactivate F0 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8611 )
     {
          strcat(return_string, "Deactivate F1 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8612 )
     {
          strcat(return_string, "Deactivate F2 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8613 )
     {
          strcat(return_string, "Deactivate F3 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8614 )
     {
          strcat(return_string, "Deactivate F4 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8615 )
     {
          strcat(return_string, "Deactivate F5 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8616 )
     {
          strcat(return_string, "Deactivate F6 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8617 )
     {
          strcat(return_string, "Deactivate F7 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8618 )
     {
          strcat(return_string, "Deactivate F8 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8619 )
     {
          strcat(return_string, "Deactivate F9 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8620 )
     {
          strcat(return_string, "Deactivate F10 - activate all other F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( ( M1 >= 8621 ) && ( M1 <= 8630 ) )
     {
          strcat(return_string, "Deactivate all F-keys");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8650 )
     {
          sprintf(return_string, "Set video mode %d", M4);
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 8700 )
     {
          strcat(return_string, "Auto Solve");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 9001 )
     {
          strcat(return_string, "Auto-Save");
          set_in_condensed(return_string);
          return(0);
     }

     if ( M1 == 9100 )
     {
          strcat(return_string, "Host Interface (native)");
          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Data Model commands.
     */

     if ( ( M1 >= 9200 ) && ( M1 <= 9401 ) )
     {
          if ( ( M1 == 9200 ) || ( M1 == 9201 ) )
               strcat(return_string, "Run Procedure: ");

          if ( ( M1 == 9300 ) || ( M1 == 9301 ) )
               strcat(return_string, "Put to Data Model: ");

          if ( ( M1 == 9400 ) || ( M1 == 9401 ) )
               strcat(return_string, "Get from Data Model: ");

          if ( M2 == 0 )
               strcat(return_string, "[number] ");
          else if ( M2 == 1 )
               strcat(return_string, "[label] ");
          else if ( M2 == 2 )
               strcat(return_string, "[value] ");
          else
               strcat(return_string, "[???] ");

          /*
          **  Data Model commands may mention up to two prompts by name.
          */

          if ( ( M3 >= 1 ) && ( M3 <= 2000 ) )
          {
               get_prompt( short_name_3, prompt, M3, line_number );
               set_in_italics( short_name_3 );
               strcat( return_string, short_name_3 );
               strcat( return_string, ", " );
          }

          /*
          **  Second one may or may not be specified (cross-reference).
          */

          if ( ( M4 >= 1 ) && ( M4 <= 2000 ) )
          {
               get_prompt( short_name_4, prompt, M4, line_number );
               set_in_italics( short_name_4 );
               strcat( return_string, short_name_4 );
          }

          /*
          **  Return the data model comments.
          */

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Arithmetic commands.  These are a real bear to decode.
     */

     if ( ( M1 > 10000 ) && ( M1 <= 12000 ) )
     {
          /*
          **  First get the name of the prompt we are to modify.
          */

          get_modified_prompt( prompt_name, prompt, M1-10000, line_number );
          set_in_italics( prompt_name );

          get_prompt( short_name_1, prompt, M1-10000, line_number );
          set_in_italics( short_name_1 );

          --prompt[M1-10000].referenced;      /* counted this prompt twice */

          /*
          **  Are we just clearing it out?  Then set it equal to zero.
          */

          if ( ( M2 == 22002 ) && ( M3 == 22002 ) && ( M4 == 22002 ) )
          {
               sprintf(substring, "%s = 0", short_name_1);
               strcat(return_string, substring);
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Otherwise, pull apart the operators and operands and wade on in.
          */

          operator_2 = M2 % 10;
          operator_3 = M3 % 10;
          operator_4 = M4 % 10;

          operand_2 =  M2 / 10;
          operand_3 =  M3 / 10;
          operand_4 =  M4 / 10;

          /*
          **  Special handling for the Pause Flag.
          */

          if ( ( M1 == 11547 ) && ( M2 == 15475 ) && (M3 == 22002 ) &&
               ( M4 == 22002 ) )
          {
               /*
               **  Dummy code to keep the accounting correct.
               */

               get_prompt ( short_name_2, prompt, 1547, line_number );

               strcat(return_string, "(Clear pause flag)");
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Clear the prompt (gets the long version of the prompt name).
          */

          if ( ( operand_2 == ( M1 - 10000 ) ) && ( operator_2 == 5 ) &&
               ( M3 == 22002 ) && ( M4 == 22002 ) )
          {
               /*
               **  Dummy code to keep the accounting correct.
               */

               get_prompt ( short_name_2, prompt, operand_2, line_number );

               sprintf(substring, "Clear %s", prompt_name);
               strcat(return_string, substring);
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  First print the prompt name, followed by an equal sign.
          */

          sprintf(substring, "%s = ", short_name_1);
          strcat(return_string, substring);

          /*
          **  LEN: get the length of the operand.
          */

          if ( operator_2 == 0 )
          {
               /*
               **  . . . of a prompt
               */

               if ( operand_2 <= 2000 )
               {
                    get_prompt(short_name_2, prompt, operand_2, line_number );
                    set_in_italics( short_name_2 );
                    sprintf( substring, "len(%s)", short_name_2 );
                    strcat( return_string, substring );
               }

               /*
               **  . . . or of a constant integer.
               */

               else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
               {
                    sprintf(substring, "len(%d)", operand_2-2200);
                    strcat(return_string, substring);
               }
          }

          /*
          **  Arithmetic operations: we do not print the operator for #1.
          */

          else if ( ( operator_2 >= 1 ) && ( operator_2 <= 4 ) )
          {
               /*
               **  . . . whether it is a prompt
               */

               if ( operand_2 <= 2000 )
               {
                    get_prompt( short_name_2, prompt, operand_2, line_number );
                    set_in_italics( short_name_2 );
                    strcat( return_string, short_name_2 );
               }

               /*
               **  . . . or a constant integer.
               */

               else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
               {
                    sprintf(substring, "%d", operand_2-2200);
                    strcat(return_string, substring);
               }
          }

          /*
          **  CLEAR: clear out the operand (prompts only).
          */

          else if ( operator_2 == 5 )
          {
               if ( operand_2 <= 2000 )
               {
                    get_prompt( short_name_2, prompt, operand_2, line_number );
                    set_in_italics( short_name_2 );
                    sprintf( substring, "clear(%s)", short_name_2 );
                    strcat( return_string, substring );
               }
          }

          /*
          **  Extended Operations:
          */

          else if ( operator_2 == 6 )
          {
               /*
               **  EXPONENTIATION: raise operand 2 to the power of operand 3.
               **                  (as usual, may be prompts or constant
               **                  integers, or both.)
               */

               if ( ( operator_3 >= 1 ) && ( operator_3 <= 4 ) )
               {
                    /*
                    **  prompts . . .
                    */

                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number);
                         set_in_italics( short_name_2 );
                    }

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }

                    /*
                    **  constant integers . . .
                    */

                    if ( operand_2 <= 2000 )
                         sprintf(substring, "%s ^ ", short_name_2);
                    else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
                         sprintf(substring, "%d ^ ", operand_2 - 2200);

                    strcat(return_string, substring);

                    if ( operand_3 <= 2000 )
                         sprintf(substring, "%s ^ ", short_name_3);
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(substring, "%d ^ ", operand_3 - 2200);

                    strcat(return_string, substring);

                    /*
                    **  Exit here if we're already done (exponentiation).
                    */

                    if ( M4 == 22002 )
                    {
                         set_in_condensed(return_string);
                         return(0);
                    }

                    /*
                    **  Otherwise, print the fourth operator.
                    */

                    switch ( operator_4 )
                    {
                         case 1:
                              strcat(return_string, " - ");
                              break;

                         case 2:
                              strcat(return_string, " + ");
                              break;

                         case 3:
                              strcat(return_string, " * ");
                              break;

                         case 4:
                              strcat(return_string, " / ");
                              break;

                         default:
                              break;
                    }

                    /*
                    **  . . . and its operand.
                    */

                    if ( operand_4 <= 2000 )
                    {
                         get_prompt( short_name_4, prompt, operand_4, line_number );
                         set_in_italics( short_name_4 );
                         sprintf( substring, "%s ", short_name_4 );
                         strcat( return_string, substring );
                    }
                    else if ( ( operand_4 >= 2200 ) && ( operand_4 <= 3276 ) )
                    {
                         sprintf(substring, "%d ", operand_4 - 2200);
                         strcat(return_string, substring);
                    }

                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  MID$: get the midstring of the operand.
               */

               else if ( operator_3 == 5 )
               {
                    /*
                    **  first parameter must be either a prompt or a CONSTANT.
                    */

                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf( substring, "mid$(%s, ", short_name_2 );
                         strcat( return_string, substring );
                    }

                    /*
                    **  Second and third parameters may be prompts or integers.
                    */

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }
                    if ( operand_4 <= 2000 )
                    {
                         get_prompt( short_name_4, prompt, operand_4, line_number );
                         set_in_italics( short_name_4 );
                    }
                    if ( operand_3 <= 2000 )
                         sprintf(substring, "%s, ", short_name_3);
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(substring, "%d, ", operand_3 - 2200);

                    strcat(return_string, substring);

                    if ( operand_4 <= 2000 )
                         sprintf(substring, "%s)", short_name_4);
                    else if ( ( operand_4 >= 2200 ) && ( operand_4 <= 3276 ) )
                         sprintf(substring, "%d)", operand_4 - 2200);

                    strcat(return_string, substring);

                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  TIME: get the system time from the PC internal clock.
               */

               else if ( operator_3 == 6 )
                    strcat(return_string, "system time");

               /*
               **  Exit here if we're already done (system time).
               */

               if ( M4 == 22002 )
               {
                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  Otherwise, print the fourth operator.
               */

               switch ( operator_4 )
               {
                    case 1:
                         strcat(return_string, " - ");
                         break;

                    case 2:
                         strcat(return_string, " + ");
                         break;

                    case 3:
                         strcat(return_string, " * ");
                         break;

                    case 4:
                         strcat(return_string, " / ");
                         break;

                    default:
                         break;
               }

               /*
               **  . . . and its operand.
               */

               if ( operand_4 <= 2000 )
               {
                    get_prompt( short_name_4, prompt, operand_4, line_number );
                    set_in_italics( short_name_4 );
                    sprintf( substring, "%s ", short_name_4 );
                    strcat( return_string, substring );
               }
               else if ( ( operand_4 >= 2200 ) && ( operand_4 <= 3276 ) )
               {
                    sprintf(substring, "%d ", operand_4 - 2200);
                    strcat(return_string, substring);
               }

               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  LOG: return the logarithm (presumably base e, but not specified).
          */

          else if ( operator_2 == 7 )
          {
               /*
               **  . . . of a prompt
               */

               if ( operand_2 <= 2000 )
               {
                    get_prompt( short_name_2, prompt, operand_2, line_number );
                    set_in_italics( short_name_2 );
                    sprintf( substring, "log(%s)", short_name_2 );
               }

               /*
               ** . . . or of a constant integer.
               */

               else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
               {
                    sprintf(substring, "log(%d)", operand_2 - 2200);
               }

               strcat(return_string, substring);
          }

          /*
          **  INT: return the positive integer portion of the argument.
          */

          else if ( operator_2 == 8 )
          {
               /*
               **  be it a prompt . . .
               */

               if ( operand_2 <= 2000 )
               {
                    get_prompt( short_name_2, prompt, operand_2, line_number );
                    set_in_italics( short_name_2 );
                    sprintf( substring, "INT(%s)", short_name_2 );
               }

               /*
               ** . . . or something else again.
               */

               else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
               {
                    sprintf(substring, "INT(%d)", operand_2 - 2200);
               }

               strcat(return_string, substring);
          }

          /*
          **  Ahh, the Date routines.  Who can begin to understand
          **  the date routines?
          */

          else if ( operator_2 == 9 )
          {
               /*
               **  Future Date 365.
               */

               if ( operator_3 == 1 )
               {
                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number );
                         set_in_italics( short_name_2 );
                    }
                    else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
                         sprintf(short_name_2, "%d", operand_2 - 2200);

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(short_name_3, "%d", operand_3 - 2200);

                    sprintf(substring, "Future date 365(%s, %s)",
                            short_name_2, short_name_3
                    );
                    strcat(return_string, substring);

                    /*
                    **  (Date routines return now.)
                    */

                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  Future Date 360.
               */

               else if ( operator_3 == 2 )
               {
                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number );
                         set_in_italics( short_name_2 );
                    }
                    else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
                         sprintf(short_name_2, "%d", operand_2 - 2200);

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(short_name_3, "%d", operand_3 - 2200);

                    sprintf(substring, "Future date 360(%s, %s)",
                            short_name_2, short_name_3
                    );
                    strcat(return_string, substring);

                    /*
                    **  (Date routines return now.)
                    */

                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  Number of Days between Dates.
               */

               else if ( operator_3 == 3 )
               {
                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number );
                         set_in_italics( short_name_2 );
                    }
                    else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
                         sprintf(short_name_2, "%d", operand_2 - 2200);

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(short_name_3, "%d", operand_3 - 2200);

                    sprintf(substring, "Number of days between %s and %s",
                            short_name_2, short_name_3
                    );
                    strcat(return_string, substring);

                    /*
                    **  (Date routines return now.)
                    */

                    set_in_condensed(return_string);
                    return(0);
               }

               /*
               **  And the infamous 4th Date Routine.  Nobody understands
               **  what this one is for.  And so they never use it.
               */

               else if ( operator_3 == 4 )
               {
                    if ( operand_2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, operand_2, line_number );
                         set_in_italics( short_name_2 );
                    }
                    else if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) )
                         sprintf(short_name_2, "%d", operand_2 - 2200);

                    if ( operand_3 <= 2000 )
                    {
                         get_prompt( short_name_3, prompt, operand_3, line_number );
                         set_in_italics( short_name_3 );
                    }
                    else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
                         sprintf(short_name_3, "%d", operand_3 - 2200);

                    sprintf(substring, "Future date 360(%s, %s) and number of days.",
                            short_name_2, short_name_3
                    );
                    strcat(return_string, substring);

                    /*
                    **  (Date routines return now.)
                    */

                    set_in_condensed(return_string);
                    return(0);
               }
          }

          /*
          **  Are we done with the line yet?  If so, exit now, and avoid
          **  leaving unsightly extra commas lying around.  (I'm serious;
          **  details like this are what make the LIST program so invisible.)
          */

          if ( ( M3 == 22002 ) && ( M4 == 22002 ) )
          {
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Otherwise, print the arithmetic operator this time.  (The
          **  reason why we never (or almost never) print the first operator
          **  is because we are essentially converting Reverse Polish
          **  Notation to Algebraic.  RPN is great.
          */

          switch ( operator_3 )
          {
               case 1:
                    strcat(return_string, " - ");
                    break;

               case 2:
                    strcat(return_string, " + ");
                    break;

               case 3:
                    strcat(return_string, " * ");
                    break;

               case 4:
                    strcat(return_string, " / ");
                    break;

               default:
                    break;
          }

          /*
          **  Operand 3 may be a prompt . . .
          */

          if ( operand_3 <= 2000 )
          {
               get_prompt( short_name_3, prompt, operand_3, line_number );
               set_in_italics( short_name_3 );
               sprintf( substring, "%s", short_name_3 );
               strcat( return_string, substring );
          }

          /*
          **  . . . or it may be one of NFCS's funny integer constants.
          */

          else if ( ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) )
          {
               sprintf(substring, "%d", operand_3 - 2200);
               strcat(return_string, substring);
          }

          /*
          **  Note how much shorter that section was that the previous one.
          **  Many operators, such as MID$ and the Date Routines, require
          **  so many parameters that there isn't much room left for other
          **  operations on that line.  And LEN, etc., only work on the
          **  first parameter (otherwise it doesn't make sense).  The only
          **  things we have to worry about from here on in are addition,
          **  subtraction, multiplication and division.
          */

          /*
          **  Is this a case of exactly two constants in a row?  If so,
          **  compute their {sum | difference | product | quotient} and
          **  display it for the user at the end of the line.  Handle
          **  fractional decimal values, if specified.
          */

          if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) &&
               ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) &&
               ( operator_2 >= 1 ) && ( operator_2 <= 4 ) &&
               ( operator_3 >= 1 ) && ( operator_3 <= 4 ) &&
               ( M4 == 22002 )
             )
          {
               arithmetic_result = (double)operand_2 - 2200L;

               switch ( operator_3 )
               {
                    case 1:
                         arithmetic_result -= ( (double)operand_3 - 2200L );
                         break;

                    case 2:
                         arithmetic_result += ( (double)operand_3 - 2200L );
                         break;

                    case 3:
                         arithmetic_result *= ( (double)operand_3 - 2200L );
                         break;

                    case 4:
                         arithmetic_result /= ( (double)operand_3 - 2200L );
                         break;

                    default:
                         break;
               }

               sprintf(substring, " = %s%g%s", ital, arithmetic_result, itoff);
               strcat(return_string, substring);
          }

          /*
          **  Again, exit if we're already done.
          */

          if ( M4 == 22002 )
          {
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Otherwise, print the fourth operator.
          */

          switch ( operator_4 )
          {
               case 1:
                    strcat(return_string, " - ");
                    break;

               case 2:
                    strcat(return_string, " + ");
                    break;

               case 3:
                    strcat(return_string, " * ");
                    break;

               case 4:
                    strcat(return_string, " / ");
                    break;

               default:
                    break;
          }

          /*
          **  . . . and its operand.
          */

          if ( operand_4 <= 2000 )
          {
               get_prompt( short_name_4, prompt, operand_4, line_number );
               set_in_italics( short_name_4 );
               sprintf( substring, "%s", short_name_4 );
               strcat( return_string, substring );
          }
          else if ( ( operand_4 >= 2200 ) && ( operand_4 <= 3276 ) )
          {
               sprintf(substring, "%d", operand_4 - 2200);
               strcat(return_string, substring);
          }

          /*
          **  Is this a case of exactly three constants in a row?  If so,
          **  compute their {sum | difference | product | quotient} and
          **  display it for the user at the end of the line.
          */

          if ( ( operand_2 >= 2200 ) && ( operand_2 <= 3276 ) &&
               ( operand_3 >= 2200 ) && ( operand_3 <= 3276 ) &&
               ( operand_4 >= 2200 ) && ( operand_4 <= 3276 ) &&
               ( operator_2 >= 1 ) && ( operator_2 <= 4 ) &&
               ( operator_3 >= 1 ) && ( operator_3 <= 4 ) &&
               ( operator_4 >= 1 ) && ( operator_4 <= 4 )
             )
          {
               arithmetic_result = (double)operand_2 - 2200L;

               switch ( operator_3 )
               {
                    case 1:
                         arithmetic_result -= ( (double)operand_3 - 2200L );
                         break;

                    case 2:
                         arithmetic_result += ( (double)operand_3 - 2200L );
                         break;

                    case 3:
                         arithmetic_result *= ( (double)operand_3 - 2200L );
                         break;

                    case 4:
                         arithmetic_result /= ( (double)operand_3 - 2200L );
                         break;

                    default:
                         break;
               }

               switch ( operator_4 )
               {
                    case 1:
                         arithmetic_result -= ( (double)operand_4 - 2200L );
                         break;

                    case 2:
                         arithmetic_result += ( (double)operand_4 - 2200L );
                         break;

                    case 3:
                         arithmetic_result *= ( (double)operand_4 - 2200L );
                         break;

                    case 4:
                         arithmetic_result /= ( (double)operand_4 - 2200L );
                         break;

                    default:
                         break;
               }

               sprintf(substring, " = %s%g%s", ital, arithmetic_result, itoff);
               strcat(return_string, substring);
          }

          /*
          **  Finally.  Return the line and let's go have a beer.
          */

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  File read/write commands.  Most of this is self-explanatory.
     */

     if ( ( M1 >= 16000 ) && ( M1 <= 18999 ) )
     {
          switch ( M1 / 100 )
          {
               case 160:
                    strcat(return_string, "Continue reading (comma or CR/LF), ");
                    break;

               case 161:
                    strcat(return_string, "Open file for fixed-length read ");
                    break;

               case 162:
                    strcat(return_string, "Open file for comma or CR/LF read ");
                    break;

               case 163:
                    strcat(return_string, "Open file for CR/LF read ");
                    break;

               case 164:
                    strcat(return_string, "Open file for fixed-length read, then close ");
                    break;

               case 165:
                    strcat(return_string, "Open file for comma or CR/LF read, then close ");
                    break;

               case 166:
                    strcat(return_string, "Open file for CR/LF read, then close ");
                    break;

               case 167:
                    strcat(return_string, "Continue reading fixed length, ");
                    break;

               case 168:
                    break;

               case 169:
                    strcat(return_string, "Close file ");
                    break;

               case 170:
                    strcat(return_string, "Continue writing (comma or CR/LF), ");
                    break;

               case 171:
                    strcat(return_string, "Open existing file for fixed-length write ");
                    break;

               case 172:
                    strcat(return_string, "Open existing file for comma or CR/LF write ");
                    break;

               case 173:
                    strcat(return_string, "Open existing file for CR/LF write ");
                    break;

               case 174:
                    strcat(return_string, "Open existing file for fixed-length write, then close ");
                    break;

               case 175:
                    strcat(return_string, "Open existing file for comma or CR/LF write, then close ");
                    break;

               case 176:
                    strcat(return_string, "Open existing file for CR/LF write, then close ");
                    break;

               case 177:
                    strcat(return_string, "Continue reading fixed length, ");
                    break;

               case 178:
                    break;

               case 179:
                    strcat(return_string, "Close file ");
                    break;

               case 180:
                    strcat(return_string, "Continue writing (comma or CR/LF), ");
                    break;

               case 181:
                    strcat(return_string, "Open new file for fixed-length write ");
                    break;

               case 182:
                    strcat(return_string, "Open new file for comma or CR/LF write ");
                    break;

               case 183:
                    strcat(return_string, "Open new file for CR/LF write ");
                    break;

               case 184:
                    strcat(return_string, "Open new file for fixed-length write, then close ");
                    break;

               case 185:
                    strcat(return_string, "Open new file for comma or CR/LF write, then close ");
                    break;

               case 186:
                    strcat(return_string, "Open new file for CR/LF write, then close ");
                    break;

               case 187:
                    strcat(return_string, "Continue reading fixed length, ");
                    break;

               case 188:
                    break;

               case 189:
                    strcat(return_string, "Close file ");
                    break;

               default:
                    break;
          }

          /*
          **  End the line here, if only opening or closing a file.
          */

          if ( ( ( M1 / 100 ) != 160 ) &&
               ( ( M1 / 100 ) != 167 ) &&
               ( ( M1 / 100 ) != 170 ) &&
               ( ( M1 / 100 ) != 177 ) &&
               ( ( M1 / 100 ) != 180 ) &&
               ( ( M1 / 100 ) != 187 ) )
          {
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Otherwise, specify what drive, if other than the default one (0).
          */

          operator_2 = M2 / 10;
          operand_2 = inline.L2 % 10;

          if ( operand_2 != 0 )
          {
               sprintf(substring, "(drive %c) ", (char)(64+operand_2));
               strcat(return_string, substring);
          }

          /*
          **  . . . and the position, if other than the current position (-1).
          */

          if ( operand_2 != (-1) )
          {
               sprintf(substring, "at position %d: ", operand_2);
               strcat(return_string, substring);
          }

          /*
          **  If the third parameter is a prompt, give its name.
          */

          if ( ( M3 > 0 ) && ( M3 <= 2000 ) )
          {
               get_prompt( short_name_3, prompt, M3, line_number );
               set_in_italics( short_name_3 );
               sprintf( substring, "%s", short_name_3 );
          }

          /*
          ** If the third parameter is an indirect prompt, indicate this.
          */

          else if ( ( M3 > 10000 ) && ( M3 <= 12000 ) )
          {
               get_prompt( short_name_3, prompt, M3, line_number );
               set_in_italics( short_name_3 );
               sprintf( substring, "(*)%s", short_name_3 );
          }

          /*
          **  . . . and if it is a Table entry, tell where.
          */

          else if ( ( M3 > 20000 ) && ( M3 <= 22000 ) )
               sprintf(substring, "%d (description)", M3-20000);
          else if ( ( M3 > 30000 ) && ( M3 <= 32000 ) )
               sprintf(substring, "%d (item)", M3-20000);

          /*
          **  Add it to the current line.
          */

          strcat(return_string, substring);

          /*
          **  If there is no fourth parameter, then return now.
          */

          if ( M4 == 0 )
          {
               set_in_condensed(return_string);
               return(0);
          }

          /*
          **  Otherwise, print a comma after the third.
          */

          else
               strcat(return_string, ", ");

          /*
          **  If the fourth parameter is a prompt, give its name.
          */

          if ( ( M4 > 0 ) && ( M4 <= 2000 ) )
          {
               get_prompt( short_name_4, prompt, M4, line_number );
               set_in_italics( short_name_4 );
               sprintf( substring, "%s", short_name_4 );
          }

          /*
          ** If the fourth parameter is an indirect prompt, indicate this.
          */

          else if ( ( M4 > 10000 ) && ( M4 <= 12000 ) )
          {
               get_prompt( short_name_4, prompt, M4, line_number );
               set_in_italics( short_name_4 );
               sprintf( substring, "(*)%s", short_name_4 );
          }

          /*
          **  . . . and if it is a Table entry, tell where.
          */

          else if ( ( M4 > 20000 ) && ( M4 <= 22000 ) )
               sprintf(substring, "%d (description)", M4-20000);
          else if ( ( M4 > 30000 ) && ( M4 <= 32000 ) )
               sprintf(substring, "%d (item)", M4-20000);

          /*
          **  Add it to the current line and return.
          */

          strcat(return_string, substring);

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  Form display commands (long prompt name).
     */

     if ( ( M1 > 20000 ) && ( M1 <= 22000 ) )
     {
          get_prompt( prompt_name, prompt, M1 - 20000, line_number );
          set_in_italics( prompt_name );
          sprintf( substring, "Display: %s", prompt_name );
          strcat( return_string, substring );
          set_in_condensed( return_string );
          return(0);
     }

     /*
     **  Table commands.
     */

     if ( ( M1 > 30000 ) && ( M1 <= 32000 ) )
     {
          operator_4 = M4 % 100;
          operand_4 = M4 / 100;

          strcat(return_string, "Table ");

          get_modified_prompt( short_name_1, prompt, M1-30000, line_number );
          set_in_italics( short_name_1 );

          switch (operator_4)
          {
               case 1:       /* Table Search */

                    strcat(return_string, "search: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return location]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return location]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 2:       /* Table Search */

                    strcat(return_string, "search: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return description]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return description]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 3:       /* Table Search */

                    strcat(return_string, "search range: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return location]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return location]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 4:       /* Table Search */

                    strcat(return_string, "search range: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return location]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return location]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 5:       /* Table Search */

                    strcat(return_string, "search range pairs: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return location]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return location]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 6:       /* Table Search */

                    strcat(return_string, "search range pairs: ");
                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         sprintf(substring, "%s <-- %s at %d (%d) [return location]",
                                 short_name_1,
                                 short_name_2,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         sprintf(substring, "%s <-- %d at %d (%d) [return location]",
                                 short_name_1,
                                 M2 - 10000,
                                 M3,
                                 operand_4
                         );
                         strcat(return_string, substring);
                    }
                    break;

               case 7:       /* Table Transfer */

                    sprintf(substring, "transfer: %s <-- ", short_name_1);
                    strcat(return_string, substring);

                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return item]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return item]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return item]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return item]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    break;

               case 8:       /* Table Transfer */

                    sprintf(substring, "transfer: %s <-- ", short_name_1);
                    strcat(return_string, substring);

                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         if ( operand_4 == 1)
                              sprintf(substring, "%d (%d item) [return description]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return description]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return description]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return description]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    break;

               case 9:       /* Table Transfer */

                    sprintf(substring, "transfer: %s --> ", short_name_1);
                    strcat(return_string, substring);

                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return item]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return item]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return item]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return items]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    break;

               case 10:      /* Table Transfer */

                    sprintf(substring, "transfer: %s --> ", short_name_1);
                    strcat(return_string, substring);

                    if ( M2 <= 2000 )
                    {
                         get_prompt( short_name_2, prompt, M2, line_number );
                         set_in_italics( short_name_2 );
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return description]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return description]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    else if ( ( M2 >= 10000 ) && ( M2 <= 19999 ) )
                    {
                         if ( operand_4 == 1 )
                              sprintf(substring, "%d (%d item) [return description]",
                                      M3,
                                      operand_4
                              );
                         else
                              sprintf(substring, "%d (%d items) [return description]",
                                      M3,
                                      operand_4
                              );
                         strcat(return_string, substring);
                    }
                    break;

               default:
                    break;
          }

          /*
          **  Return the Table line.
          */

          set_in_condensed(return_string);
          return(0);
     }

     /*
     **  otherwise...
     */

     fprintf(stdout, "\n\aRun-time error in LIST: no comments generated for line %ld\n",
                     line_number);

     fprintf(stdout, "--> %d,%d,%d,%d\n",
                     inline.L1,
                     inline.L2,
                     inline.L3,
                     inline.L4
     );

     strcpy(return_string, "(LIST DID NOT UNDERSTAND THIS LINE)");
     set_in_boldface(return_string);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit COMMENTS\n");
#endif

     return(0);
}


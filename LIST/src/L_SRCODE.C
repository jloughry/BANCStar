
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  SOURCECODE: format the BANCStar source code with boldface prompts.      **
**                                                                          **
*****************************************************************************/

int sourcecode( struct sourceline inline,
                struct sourceline_string *inline_string )
{
     char *numstring( int );
     char *zero_numstring( int );
     int set_in_boldface( char * );

     unsigned int M1, M2, M3, M4;

     unsigned int operator_2, operand_2;     /******************************/
     unsigned int operator_3, operand_3;     /* This part does the same    */
     unsigned int operator_4, operand_4;     /* thing as the original LIST */
                                             /* program.  Only better.     */
     char substring[MAX_LINE_LENGTH];        /******************************/

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SOURCECODE\n");
#endif
                                             /******************************/
     M1 = abs(inline.L1);                    /* so we don't have to fool   */
     M2 = abs(inline.L2);                    /* around with INT all of the */
     M3 = abs(inline.L3);                    /* time for absolute value.   */
     M4 = abs(inline.L4);                    /******************************/
                                                                 /***********/
     strcpy(inline_string->L1_string, numstring(inline.L1));     /* for the */
     strcpy(inline_string->L2_string, numstring(inline.L2));     /* bold-   */
     strcpy(inline_string->L3_string, numstring(inline.L3));     /* facing. */
     strcpy(inline_string->L4_string, numstring(inline.L4));     /***********/

     /*
     **  displayed prompts are set in boldface.
     */

     if ( ( M1 >= 1 ) && ( M1 <= 2000 ) )
     {
          set_in_boldface(inline_string->L1_string);
     }

     /*
     **  Discrete lines [ Note that only constructions requiring boldfacing
     **                   or other changes are mentioned by name.  All others
     **                   are assumed to fall through to the end. ]
     */

     /*
     **  Stop codes: no prompts highlighted.
     */

     /*
     **  Conditionals:
     */

     switch ( M1 )
     {                /****************************/
          case 3000:  /* Single Condition         */
          case 3001:  /* Block Condition          */
          case 3100:  /* Reverse Single Condition */
          case 3101:  /* Reverse Block Condition  */
                      /****************************/

               set_in_boldface(inline_string->L2_string);

               operator_4 = M4 / 10000;
               operand_4 = M4 % 10000;

               if ( ( M4 > 20000 ) && ( M4 <= 22000 ) )
               {
                    sprintf(substring, "%d", inline.L4 - 20000);
                    set_in_boldface(substring);

                    if ( M4 < 20010 )
                         sprintf(inline_string->L4_string, "2000%s", substring);
                    else if ( M4 < 20100 )
                         sprintf(inline_string->L4_string, "200%s", substring);
                    else if ( M4 < 21000 )
                         sprintf(inline_string->L4_string, "20%s", substring);
                    else
                         sprintf(inline_string->L4_string, "2%s", substring);
               }
               break;

          case 8550:  /* Combination Goto */

               if ( ( M2 > 10000 ) && ( M2 <= 12000 ) )
               {
                    sprintf(substring, "%d", inline.L2 - 10000);
                    set_in_boldface(substring);

                    if ( M2 < 10010 )
                         sprintf(inline_string->L2_string, "1000%s", substring);
                    else if ( M2 < 10100 )
                         sprintf(inline_string->L2_string, "100%s", substring);
                    else if ( M2 < 11000 )
                         sprintf(inline_string->L2_string, "10%s", substring);
                    else
                         sprintf(inline_string->L4_string, "1%s", substring);
               }

               if ( ( M3 > 10000 ) && ( M3 <= 12000 ) )
               {
                    sprintf(substring, "%d", inline.L3 - 10000);
                    set_in_boldface(substring);

                    if ( M3 < 10010 )
                         sprintf(inline_string->L3_string, "1000%s", substring);
                    else if ( M3 < 10100 )
                         sprintf(inline_string->L3_string, "100%s", substring);
                    else if ( M3 < 11000 )
                         sprintf(inline_string->L3_string, "10%s", substring);
                    else
                         sprintf(inline_string->L3_string, "1%s", substring);
               }

               if ( ( M4 > 10000 ) && ( M4 <= 12000 ) )
               {
                    sprintf(substring, "%d", inline.L4 - 10000);
                    set_in_boldface(substring);

                    if ( M4 < 10010 )
                         sprintf(inline_string->L4_string, "1000%s", substring);
                    else if ( M4 < 10100 )
                         sprintf(inline_string->L4_string, "100%s", substring);
                    else if ( M4 < 11000 )
                         sprintf(inline_string->L4_string, "10%s", substring);
                    else
                         sprintf(inline_string->L4_string, "1%s", substring);
               }

               break;

          case 8560:  /* DOS Exec Command */

               if ( M4 <= 2000 )
                    set_in_boldface(inline_string->L4_string);

               break;

          case 8650:  /* Set Video Mode */

               if ( M4 <= 2000 )
                    set_in_boldface(inline_string->L4_string);

               break;

          default:    /* (end of discrete cases) */

               break;
     }

     /*
     **  Data model commands.
     */

     if ( ( M1 >= 9200 ) && ( M1 <= 9401 ) )
     {
          if ( M3 <= 2000 )
               set_in_boldface(inline_string->L3_string);

          if ( M4 <= 2000 )
               set_in_boldface(inline_string->L4_string);
     }

     /*
     **  Arithmetic commands.
     */

     if ( ( M1 > 10000 ) && ( M1 <= 12000 ) )
     {
          sprintf(substring, "%d", inline.L1 - 10000);
          set_in_boldface(substring);

          if ( M1 < 10010 )
               sprintf(inline_string->L1_string, "1000%s", substring);
          else if ( M1 < 10100 )
               sprintf(inline_string->L1_string, "100%s", substring);
          else if ( M1 < 11000 )
               sprintf(inline_string->L1_string, "10%s", substring);
          else
               sprintf(inline_string->L1_string, "1%s", substring);

          operator_2 = M2 % 10;
          operand_2 = M2 / 10;

          operator_3 = M3 % 10;
          operand_3 = M3 / 10;

          operator_4 = M4 % 10;
          operand_4 = M4 / 10;

          if ( operand_2 <= 2000 )
          {
               strcpy(inline_string->L2_string, numstring(operand_2));
               set_in_boldface(inline_string->L2_string);
               strcat(inline_string->L2_string, zero_numstring(operator_2));
          }

          if ( operand_3 <= 2000 )
          {
               strcpy(inline_string->L3_string, numstring(operand_3));
               set_in_boldface(inline_string->L3_string);
               strcat(inline_string->L3_string, zero_numstring(operator_3));
          }

          if ( operand_4 <= 2000 )
          {
               strcpy(inline_string->L4_string, numstring(operand_4));
               set_in_boldface(inline_string->L4_string);
               strcat(inline_string->L4_string, zero_numstring(operator_4));
          }
     }

     /*
     **  File read-write commands.
     */

     if ( ( M1 >= 16000 ) && ( M1 <= 18999 ) )
     {
          if ( M3 <= 2000 )
          {
               strcpy(inline_string->L3_string, numstring(M3));
               set_in_boldface(inline_string->L3_string);
          }
          else if ( ( M3 > 10000 ) && ( M3 <= 12000 ) )
          {
               sprintf(substring, "%d", inline.L3 - 10000);
               set_in_boldface(substring);

               if ( M3 < 10010 )
                    sprintf(inline_string->L3_string, "1000%s", substring);
               else if ( M3 < 10100 )
                    sprintf(inline_string->L3_string, "100%s", substring);
               else if ( M3 < 11000 )
                    sprintf(inline_string->L3_string, "10%s", substring);
               else
                    sprintf(inline_string->L3_string, "1%s", substring);
          }

          if ( M4 <= 2000 )
          {
               strcpy(inline_string->L4_string, numstring(inline.L4));
               set_in_boldface(inline_string->L4_string);
          }
          else if ( ( M4 > 10000 ) && ( M4 <= 12000 ) )
          {
               sprintf(substring, "%d", inline.L4 - 10000);
               set_in_boldface(substring);

               if ( M4 < 10010 )
                    sprintf(inline_string->L4_string, "1000%s", substring);
               else if ( M4 < 10100 )
                    sprintf(inline_string->L4_string, "100%s", substring);
               else if ( M4 < 11000 )
                    sprintf(inline_string->L4_string, "10%s", substring);
               else
                    sprintf(inline_string->L4_string, "1%s", substring);
          }
     }

     /*
     **  Form Display commands.
     */

     if ( ( M1 > 20000) && ( M1 <= 22000 ) )
     {
          sprintf(substring, "%d", inline.L1 - 20000);
          set_in_boldface(substring);

          if ( M1 < 20010 )
               sprintf(inline_string->L1_string, "2000%s", substring);
          else if ( M1 < 20100 )
               sprintf(inline_string->L1_string, "200%s", substring);
          else if ( M1 < 21000 )
               sprintf(inline_string->L1_string, "20%s", substring);
          else
               sprintf(inline_string->L1_string, "2%s", substring);
     }

     /*
     **  Table commands.
     */

     if ( ( M1 > 30000) && ( M1 <= 32000 ) )
     {
          sprintf(substring, "%d", inline.L1 - 30000);
          set_in_boldface(substring);

          if ( M1 < 30010 )
               sprintf(inline_string->L1_string, "3000%s", substring);
          else if ( M1 < 30100 )
               sprintf(inline_string->L1_string, "300%s", substring);
          else if ( M1 < 31000 )
               sprintf(inline_string->L1_string, "30%s", substring);
          else
               sprintf(inline_string->L1_string, "3%s", substring);

          if ( M2 <= 2000 )
               set_in_boldface(inline_string->L2_string);
     }

     /*
     **  otherwise...
     */

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit SOURCECODE\n");
#endif

     return(0);
}



#include "l_extern.h"

extern unsigned char space[], question_mark[], page_break_char[],
                     extend_forward_block[], open_forward_block[],
                     continue_forward_block[], another_forward_block[],
                     close_forward_block[], single_arrowhead[],
                     open_reverse_block[], continue_reverse_block[],
                     another_reverse_block[], continue_crossed_reverse_block[],
                     close_reverse_block[], double_arrowhead[];

/*****************************************************************************
**
**  ARROWS: determines the proper arrangement of arrows on the line.        **
**                                                                          **
**      NOTE: This is another one of those state machines; if you want      **
**            to learn more about state machines and deterministic finite   **
**            autonoma, I recommend either -1Computability, Complexity, and-0   **
**            -1Languages: Fundamentals of Theoretical Computer Science-0,      **
**            by Davis & Weyuker (Academic Press, 1983), or any good        **
**            text on discrete structures and algorithms.                   **
**                                                                          **
*****************************************************************************/

int arrows(char *string, boolean *forward, boolean *reverse,
           struct sourceline inline, long linenum)
{
     int read_bancstar_source_code(char *, struct sourceline *);

     long i, countdown;
     char *rc;
     struct sourceline lookahead;             /* for lookahead on reverse */
     char dummy_line[MAX_LINE_LENGTH];        /* block conditionals.      */

     /*
     **  NOTE: The line-drawing characters mentioned below are very
     **        printer-dependent, and hence are defined in the appropriate
     **        printer driver section.  (See Documentation.)
     */

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering ARROWS\n");
#endif

     strcpy(string, "");

     /*
     **  Close forward block conditional, not inside reverse block.
     */

     if ( (inline.L1 == 3001) && (*forward == 1) && (*reverse == 0) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *forward = 0;
          strcat(string, close_forward_block);             /* "ÀÄ>" */
          strcat(string, extend_forward_block);
          strcat(string, single_arrowhead);
          return(0);
     }

     /*
     **  Close forward block conditional, inside a reverse block.
     */

     if ( (inline.L1 == 3001) && (*forward == 1) && (*reverse == 1) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *forward = 0;
          strcat(string, close_forward_block);             /* "À×>" */
          strcat(string, continue_crossed_reverse_block);
          strcat(string, single_arrowhead);
          return(0);
     }

     /*
     **  Error condition: trying to close a forward block conditonal before
     **                   one has been opened.  (Apparently harmless, though.)
     */

     if ( (inline.L1 == 3001) && (*forward == 0) && (*reverse == 0) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *forward = 0;
          strcat(string, question_mark);                   /* "?? " */
          strcat(string, question_mark);
          strcat(string, space);
          return(1);
     }

     /*
     **  Error condition: trying to close a forward block conditonal before
     **                   one has been opened.  (Apparently harmless, though.)
     **                   [ Inside a reverse block. ]
     */

     if ( (inline.L1 == 3001) && (*forward == 0) && (*reverse == 1) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *forward = 0;
          strcat(string, question_mark);                   /* "?? " */
          strcat(string, question_mark);
          strcat(string, space);
          return(1);
     }

     /*
     **  Close reverse block conditional, not inside forward block.
     */

     if ( (inline.L1 == 3101) && (*reverse == 0) && (*forward == 0) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *reverse = 1;
          strcat(string, space);                           /* " É¯" */
          strcat(string, close_reverse_block);
          strcat(string, double_arrowhead);
          return(0);
     }

     /*
     **  Close reverse block conditional, inside a forward block condition.
     */

     if ( (inline.L1 == 3101) && (*reverse == 0) && (*forward == 1) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *reverse = 1;
          strcat(string, continue_forward_block);          /* "³É¯" */
          strcat(string, close_reverse_block);
          strcat(string, double_arrowhead);
          return(0);
     }

     /*
     **  Error conditon: Trying to close a reverse block condition when
     **                  we are already inside one.  [ Reverse block
     **                  conditionals require thinking backwards in time. ]
     */

     if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 0) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *reverse = 1;
          strcat(string, question_mark);                   /* "???" */
          strcat(string, question_mark);
          strcat(string, question_mark);
          return(1);
     }

     /*
     **  Error conditon: Trying to close a reverse block condition when
     **                  we are already inside one.  [ Reverse block
     **                  conditionals require thinking backwards in time. ]
     **                  [ Inside a forward block conditional. ]
     */

     if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 1) &&
          (inline.L2 == 0) && (inline.L3 == 0) && (inline.L4 == 0) )
     {
          *reverse = 1;
          strcat(string, question_mark);                   /* "???" */
          strcat(string, question_mark);
          strcat(string, question_mark);
          return(1);
     }

     /*
     **  Open a forward block condition (not inside a reverse block).
     */

     if ( (inline.L1 == 3001) && (*forward == 0) && (*reverse == 0) &&
          ((inline.L2 != 0) || (inline.L3 != 0) || (inline.L4 != 0)) )
     {
          *forward = 1;
          strcat(string, open_forward_block);             /* "ÚÄ " */
          strcat(string, extend_forward_block);
          strcat(string, space);
          return(0);
     }

     /*
     **  Open a forward block condition (inside a reverse block).
     */

     if ( (inline.L1 == 3001) && (*forward == 0) && (*reverse == 1) &&
          ((inline.L2 != 0) || (inline.L3 != 0) || (inline.L4 != 0)) )
     {
          *forward = 1;
          strcat(string, open_forward_block);              /* "Ú× " */
          strcat(string, continue_crossed_reverse_block);
          strcat(string, space);
          return(0);
     }

     /*
     **  Open another forward block conditional, not inside a reverse block.
     */

     if ( (inline.L1 == 3001) && (*forward == 1) && (*reverse == 0) &&
          ((inline.L2 != 0) || (inline.L3 != 0) || (inline.L4 != 0)) )
     {
          *forward = 1;
          strcat(string, another_forward_block);           /* "ÃÄ " */
          strcat(string, extend_forward_block);
          strcat(string, space);
          return(0);
     }

     /*
     **  Open another forward block conditional, inside a reverse block.
     */

     if ( (inline.L1 == 3001) && (*forward == 1) && (*reverse == 1) &&
          ((inline.L2 != 0) || (inline.L3 != 0) || (inline.L4 != 0)) )
     {
          *forward = 1;
          strcat(string, another_forward_block);           /* "Ã× " */
          strcat(string, continue_crossed_reverse_block);
          strcat(string, space);
          return(0);
     }

     /*************************************************************************
     **  This is subtle: Whenever we encounter an open-reverse-block-condition
     **                  [ 3101 op code ], in order to determine the proper
     **                  orientation of the arrow character we must know if
     **                  the next 3101 is a close-reverse-block-conditonal or
     **                  another open-reverse-block-conditional (or the end of
     **                  the file).  This requires looking forward in time.
     **
     **                  Since that would be rather too difficult to code, we
     **                  settle instead for saving our current place in the
     **                  file, reading ahead until we find enough information
     **                  to make a decision, then closing the file, re-opening
     **                  it, and skipping ahead to the original spot, with the
     **                  state of the program intact.
     **
     **                  It sounds silly, and it is, but the user never knows
     **                  what's going on and she does like the way the arrows
     **                  always line up exactly right.  So we do it this way.
     *************************************************************************/

     if ( (inline.L1 == 3101) && (*reverse == 1) )
     {
          countdown = 0;       /* yet another of the infamous state machines */

          while (((rc = fgets(dummy_line, MAX_LINE_LENGTH, fp_input)) != NULL))
          {
               read_bancstar_source_code(dummy_line, &lookahead);

               if (lookahead.L1 == 3101)
                    break;
               else
                    ++countdown;
          }

          if ( (rc == NULL) && (lookahead.L1 != 3101) )
               countdown = 0;

          if ( (lookahead.L1 == 3101) && (lookahead.L2 == 0) &&
               (lookahead.L3 == 0) && (lookahead.L4 == 0) )
               countdown = 0;

          fclose(fp_input);

          if ((fp_input = fopen(name, "r")) == NULL)
          {
               perror("LIST");
               fclose(fp_input);
               return(-1);
          }

          i = 1;

          while ( (((fgets(dummy_line, MAX_LINE_LENGTH, fp_input)) != NULL))
                  && (i < linenum) )
               ++i;

          /*
          **  There is another reverse block conditional ahead.  [ Not inside
          **  a forward block conditional. ]
          */

          if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 0) &&
               (countdown > 0) )
          {
               *reverse = 1;
               strcat(string, space);                      /* " Ì " */
               strcat(string, another_reverse_block);
               strcat(string, space);
               return(0);
          }

          /*
          **  There is not another reverse block conditional ahead (not inside
          **  a forward block), so close it.
          */

          if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 0) &&
               (countdown <= 0) )
          {
               *reverse = 0;
               strcat(string, space);                      /* " È " */
               strcat(string, open_reverse_block);
               strcat(string, space);
               return(0);
          }

          /*
          **  There is another reverse block conditional ahead.  [ Inside a
          **  forward block conditional. ]
          */

          if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 1) &&
               (countdown > 0) )
          {
               *reverse = 1;
               strcat(string, continue_forward_block);     /* "³Ì " */
               strcat(string, another_reverse_block);
               strcat(string, space);
               return(0);
          }

          /*
          **  There is not another reverse block conditional ahead (inside a
          **  forward block), so close it.
          */

          if ( (inline.L1 == 3101) && (*reverse == 1) && (*forward == 1) &&
               (countdown <= 0) )
          {
               *reverse = 0;
               strcat(string, continue_forward_block);     /* "³È " */
               strcat(string, open_reverse_block);
               strcat(string, space);
               return(0);
          }

          /*
          **  Error condition: Trying to open a reverse block conditional
          **                   without having first closed it.  [ I told you
          **                   these things were wired backwards.  So was
          **                   whoever thought them up. ]
          */

          if ( (inline.L1 == 3101) && (*reverse == 0) && (*forward == 0) )
          {
               *reverse = 0;
               strcat(string, question_mark);              /* "?? " */
               strcat(string, question_mark);
               strcat(string, space);
               return(1);
          }

          /*
          **  Error condition: Trying to open a reverse block conditional
          **                   without having first closed it.  [ Inside a
          **                   forward block conditional. ]
          */

          if ( (inline.L1 == 3101) && (*reverse == 0) && (*forward == 1) )
          {
               *reverse = 0;
               strcat(string, question_mark);              /* "?? " */
               strcat(string, question_mark);
               strcat(string, space);
               return(1);
          }

     } /* end if lookahead */

     /*
     **  The simplest case: not inside any kind of conditional.
     */

     if ( (*forward == 0) && (*reverse == 0) )
     {
          strcat(string, space);                           /* "   " */
          strcat(string, space);
          strcat(string, space);
          --countdown;
          return(0);
     }

     /*
     **  Continuing inside a reverse block conditional.
     */

     if ( (*forward == 0) && (*reverse == 1) )
     {
          strcat(string, space);                           /* " º " */
          strcat(string, continue_reverse_block);
          strcat(string, space);
          --countdown;
          return(0);
     }

     /*
     **  Continuing inside a forward block conditional.
     */

     if ( (*forward == 1) && (*reverse == 0) )
     {
          strcat(string, continue_forward_block);          /* "³  " */
          strcat(string, space);
          strcat(string, space);
          --countdown;
          return(0);
     }

     /*
     **  Continuing inside both a forward AND a reverse block conditional.
     **  [ The issue of nesting is ill-defined in BANCStar, however. ]
     */

     if ( (*forward == 1) && (*reverse == 1) )
     {
          strcat(string, continue_forward_block);          /* "³º " */
          strcat(string, continue_reverse_block);
          strcat(string, space);
          --countdown;
          return(0);
     }

     /*
     ** (otherwise)
     */

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit ARROWS\n");
#endif

     return(0);
}


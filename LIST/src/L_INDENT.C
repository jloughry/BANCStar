
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  INDENTATION: maintain the indentation level for the comments.           **
**                                                                          **
*****************************************************************************/

int indentation(char *string, struct sourceline inline, boolean in_reverse)
{
     char *spaces(int);
     int M1, M2, M3, M4;       /* indent_level is currently global */

#ifdef DAIGNOSTICS
     fprintf(stderr, "entering INDENTATION\n");
#endif

     M1 = abs(inline.L1);      /**************************************/
     M2 = abs(inline.L2);      /* NOTE: this routine makes use of    */
     M3 = abs(inline.L3);      /*       all those global indentation */
     M4 = abs(inline.L4);      /*       variables defined at the     */
                               /*       beginning of the program.    */
                               /**************************************/

     /*
     **  All code inside a 3001 block conditional is indented 2 spaces.
     */

     if ( (M1 == 3001) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
     {
          strcpy(string, spaces(4+I2));
          goto skip;
     }

     /*
     **  All code inside a 3101 reverse block is also indented 2 spaces.
     */

     if ( (M1 == 3101) && ((M2 != 0) || (M3 != 0) || (M4 != 0)) )
     {
          strcpy(string, spaces(4+I3));
          goto skip;
     }

     /*
     **  Handling the tricky question of nested reverse block conditionals.
     **  The current scheme is vaguely unsatisfying, but to all analysis
     **  appears to be theoretically correct and morally upstanding.
     **  I don't think it can be done any better.  (Famous Last Words)
     */

     if ( (M1 == 3101) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
          I3 = indent_level;

     strcpy(string, spaces(4+indent_level));

     skip:

     /*
     **  Only the first line following a 3000 single condition is to be
     **  indented, UNLESS it is another 3000 single condition.  Then we
     **  keep indenting, level by level, as appropriate.
     */

     if ( (NX > 0) && (M1 != 3000) )
     {
          indent_level -= NX;
          NX = 0;
     }

     if (M1 == 3000)
     {
          ++indent_level;
          ++NX;
     }

     /*
     **  Handling 3001 forward block conditionals again.
     */

     if ( (M1 == 3001) && ((M2 != 0) || (M3 != 0) || (M4 != 0)) )
     {
          ++indent_level;
          FB = 1;
     }

     if ( (M1 == 3001) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
     {
          indent_level = I2;
          FB = 0;
     }

     /*
     **  And 3101 reverse block conditionals . . .
     */

     if ( (M1 == 3101) && (M2 == 0) && (M3 == 0) && (M4 == 0) )
     {
          ++indent_level;
          RB = 1;
     }

     if ( (M1 == 3101) && ((M2 != 0) || (M3 != 0) || (M4 != 0)) )
     {
          if (in_reverse == FALSE)
          {
               indent_level = I3;
               RB = 0;
          }
          else if (in_reverse == TRUE)
          {
               RB = 1;
          }
     }

     /*
     **  No, to be honest I don't understand how it works anymore.  All I
     **  know is that I simulated it extensively on an Apple II one weekend,
     **  got a scheme that worked, and translated it verbatim to this code.
     **  But hey, it works!
     */

     if ( (FB == 1) && (RB == 1) )
          I2 = 1;
     else
          I2 = 0;

#ifdef DAIGNOSTICS
     fprintf(stderr, "Exit INDENTATION\n");
#endif

     return(0);
}


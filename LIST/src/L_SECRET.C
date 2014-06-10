
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  SECRET: display the secret message if the user discovers the special    **
**        secret option on the command line.                                **
**                                                                          **
**  output: the Secret Message . . .                                        **
**                                                                          **
**  returns: (0) if successful.                                             **
**                                                                          **
*****************************************************************************/

int secret_option( void )
{
     int old_color;
     struct rccoord old_text_position;

     char outtext_string[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "Entering SECRET_OPTION\n");
#endif

     old_color = _gettextcolor();                      /* save current color */
     old_text_position = _settextposition(11,17);   /* save current location */

     _settextcolor(12);                             /* set bright red border */

     /*
     **  Secret _outtext message border:
     */

     sprintf(outtext_string, "ษอออออออออออออออออออออออออออออออออออออออออออออป\n");
     _outtext(outtext_string);

     _settextposition(12,17);
     sprintf(outtext_string, "บ                                             บ\n");
     _outtext(outtext_string);

     _settextposition(13,17);
     sprintf(outtext_string, "บ                                             บ\n");
     _outtext(outtext_string);

     _settextposition(14,17);
     sprintf(outtext_string, "บ                                             บ\n");
     _outtext(outtext_string);

     _settextposition(15,17);
     sprintf(outtext_string, "ศอออออออออออออออออออออออออออออออออออออออออออออผ\n");
     _outtext(outtext_string);

     /*
     **  Secret _outtext message text:
     */

     _settextcolor(10);                             /* set bright green text */

     _settextposition(12,19);
     sprintf(outtext_string, "You can hedge against ignorance, error,");
     _outtext(outtext_string);

     _settextposition(13,19);
     sprintf(outtext_string, "blunder, mischief, and even hubris. But the");
     _outtext(outtext_string);

     _settextposition(14,19);
     sprintf(outtext_string, "capacity for stupidity is infinite.");
     _outtext(outtext_string);

     /*
     **  End of secret _outtext message:
     */

     _settextcolor(old_color);       /* back to original color and position */
     _settextposition(15,19);
     sprintf(outtext_string, "\n");
     _outtext(outtext_string);

     _settextposition(old_text_position.row, old_text_position.col);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit SECRET_OPTION\n");
#endif

     return(0);
}


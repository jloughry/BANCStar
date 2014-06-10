
#include "l_global.h"

/*****************************************************************************
**                                                                          **
**  MAIN function.                                                          **
**                                                                          **
*****************************************************************************/

main(int argc, char **argv)
{
     int color;
     char outtext_string[MAX_LINE_LENGTH];

     for (color=0; color<=15; color++)
     {
          _settextcolor(color);
          sprintf(outtext_string, "%s", BANNER);
          _outtext(outtext_string);
          _settextcolor(7);
          sprintf(outtext_string, "%s", COPYRIGHT);
          _outtext(outtext_string);
          sprintf(outtext_string, "\n");
          _outtext(outtext_string);
     }
     fprintf(stdout, "done.");
}


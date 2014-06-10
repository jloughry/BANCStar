
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  various simple functions used by CHECK.                                 **
**                                                                          **
*****************************************************************************/


int iscomma(int c)
{
     if (c == ',')
          return(1);
     else
          return(0);
}



int isblank(int c)
{
     if (c == ' ')
          return(1);
     else
          return(0);
}



int isminus(int c)
{
     if (c == '-')
          return(1);
     else
          return(0);
}



int isnewline(int c)
{
     if (c == '\n')
          return(1);
     else
          return(0);
}


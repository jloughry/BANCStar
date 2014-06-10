
#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  get_table_item: given a line number and a side, return the ITEM or       **
**                  DESCRIPTION from main.m.                                 **
**                                                                           **
******************************************************************************/

int get_table_item ( char *return_string, MainDotM table, User_table side,
                     int line_number ) {

   if ( side == DESCRIPTION )
      strcpy ( return_string, table[line_number].description );
   else
      strcpy ( return_string, table[line_number].item );
/*
printf("\nL_GETTBL: table[800].description = '%s'", table[800].description);
printf("\nL_GETTBL: table[800].item = '%s'", table[800].item);
printf("\nL_GETTBL: return_string(%d) = '%s', strlen = %u", line_number, return_string, strlen(return_string));
*/
   return(0);
}


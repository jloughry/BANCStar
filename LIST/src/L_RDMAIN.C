
#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  read_in_main_dot_m: read in the correct MAIN.M from disk and store       **
**                      in a dynamically allocated array, similar to the     **
**                      prompt file.                                         **
**                                                                           **
******************************************************************************/

int read_in_main_dot_m ( Sellstar_system which_file, MainDotM table ) {
   char filename[MAX_LINE_LENGTH];
   char dummy_line[MAX_LINE_LENGTH];
   char description[MAX_LINE_LENGTH];
   char item[MAX_LINE_LENGTH];
   int line_number, i, ptr;
   int c;

   if ( which_file == LIABILITIES )
      strcpy ( filename, LIABILITIES_MAIN_M );
   else if ( which_file == CONSUMER )
      strcpy ( filename, CONSUMER_MAIN_M );
   else if ( which_file == RATESTAR )
      strcpy ( filename, RATESTAR_MAIN_M );
   else
      strcpy ( filename, DEFAULT_MAIN_M );

   if ( ( fp_main = fopen ( filename, "r" ) ) == NULL ) {
      fprintf(stdout, "LIST error: cannot open file %s -- files closed.\n",
              filename );

      fclose(fp_input);
      fclose(fp_output);
      fclose(fp_prompt);
      fclose(fp_check);
      fclose(fp_main);

      exit(-2);
   }

   /*
   **  Throw away the header of MAIN.M -- we're not interested in that.
   */

   for ( i=1; i<=52; i++ ) {
      fgets ( dummy_line, MAX_LINE_LENGTH, fp_main );
   }

   /*
   **  Then read in the 1000 table items we _are_ interested in.
   */

   for ( line_number=1; line_number<=1000; line_number++ ) {
      strcpy ( description, "" );
      ptr = 0;

      c = fgetc ( fp_main );

      if ( c == '"' ) {
         c = fgetc ( fp_main );
         description[ptr] = (char)c;
         while ( (c = fgetc ( fp_main )) != '"' )
            description[++ptr] = (char)c;
         description[++ptr] = '\0';
         c = fgetc ( fp_main );                     /* eat the final comma */
      }
      else if ( c == ',' )
         goto read_second_part;
      else if ( c == '\n' )
         goto store;
      else {
         description[ptr] = (char)c;
         while ( ( c = fgetc ( fp_main ) ) != ',' )
            description[++ptr] = (char)c;
         description[++ptr] = '\0';
      }

      read_second_part:
      strcpy ( item, "" );
      ptr = 0;

      c = fgetc ( fp_main );

      if ( c == '"' ) {
         c = fgetc ( fp_main );
         item[ptr] = (char)c;
         while ( (c = fgetc ( fp_main )) != '"' )
            item[++ptr] = (char)c;
         item[++ptr] = '\0';

         c = fgetc ( fp_main );
         if ( ( c != '\n' ) && ( c != ',' ) )
            ungetc ( c, fp_main );
      }
      else if ( c == ',' )
         goto store;
      else if ( c == '\n' )
         goto store;
      else {
         item[ptr] = (char)c;
         while ( ( ( c = fgetc ( fp_main ) ) != ',' ) && ( c != '\n' ) )
            item[++ptr] = (char)c;
         item[++ptr] = '\0';
      }

store:
      table[line_number].description = (char *)malloc(strlen(description)+1);

      strcpy ( table[line_number].description, description );
/*
      switch(_heapchk()) {
         case _HEAPOK:       fprintf(stdout, "Heap OK.\n");
                             break;
         case _HEAPEMPTY:    fprintf(stdout, "Heap not initialized.\n");
                             break;
         case _HEAPBADBEGIN: fprintf(stdout, "Heap header bad.\n");
                             break;
         case _HEAPBADNODE:  fprintf(stdout, "Heap has a bad node.\n");
                             break;
      }
*/
      table[line_number].item = (char *)malloc(strlen(item)+1);
      strcpy ( table[line_number].item, item );
   }

   fclose(fp_main);

   return(0);
}


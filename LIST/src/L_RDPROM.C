
#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  READ_PROMPTFILE_CODE: read a comma-delimited line of promptfile data.    **
**                                                                           **
******************************************************************************/

int read_promptfile_code(char *line, struct pfile *promptfile)
{
     int i, j;

     char firsttoken[MAX_LINE_LENGTH];
     char secondtoken[MAX_LINE_LENGTH];
     char thirdtoken[MAX_LINE_LENGTH];
     char fourthtoken[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS_2
     fprintf(stderr, "entering READ_PROMPTFILE_CODE\n");
#endif

     strcpy(firsttoken, "");            /**********************************/
     strcpy(secondtoken, "");           /* NOTE: again, as above, we have */
     strcpy(thirdtoken, "");            /*       to do it the hard way... */
     strcpy(fourthtoken, "");           /**********************************/

     i = 0;
     j = 0;

     if ( line[0] == '"' )
     {
          firsttoken[0] = '"';
          i = 1;
          j = 1;

          while ( ( line[i] != '"' ) && ( line[i] != NULL ) )
          {
               firsttoken[j] = line[i];      /* find the first parameter */
               firsttoken[j+1] = NULL;       /* (if any) up to a double quote */
               ++j;
               ++i;
          }

          firsttoken[j] = line[i];      /* read the last double quote too. */
          firsttoken[j+1] = NULL;
          ++j;
          ++i;
     }
     else
     {
          while ( ( line[i] != ',' ) && ( line[i] != NULL ) )
          {
               firsttoken[j] = line[i];      /* find the first parameter */
               firsttoken[j+1] = NULL;       /* (if any) up to a comma.  */
               ++j;
               ++i;
          }
     }

     if ( (line[i] == ',') && (line[i] != NULL) )
     {
          ++i;
          j = 0;
     }

     while ( (line[i] != ',') && (line[i] != NULL) )
     {
          secondtoken[j] = line[i];     /* find the second parameter */
          secondtoken[j+1] = NULL;      /* (if any) up to a comma.   */
          ++j;
          ++i;
     }

     if ( (line[i] == ',') && (line[i] != NULL) )
     {
          ++i;
          j = 0;
     }

     while ( (line[i] != ',') && (line[i] != NULL) )
     {
          thirdtoken[j] = line[i];      /* find the third parameter */
          thirdtoken[j+1] = NULL;       /* (if any) up to a comma.   */
          ++j;
          ++i;
     }
     promptfile->type = atoi(secondtoken);      /* and convert them into */
     promptfile->length = atoi(thirdtoken);     /* a suitable form.      */
     strcpy((*promptfile).name, firsttoken);

#ifdef DIAGNOSTICS_2
     fprintf(stderr, "exit READ_PROMPTFILE_CODE\n");
#endif

     return(0);
}


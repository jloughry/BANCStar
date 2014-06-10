
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  READ_BANCSTAR_SOURCE_CODE: read a comma-delimited line of 4 int fields. **
**                                                                          **
*****************************************************************************/

int read_bancstar_source_code(char *line, struct sourceline *parameters)
{
     int i, j;

     char firsttoken[MAX_LINE_LENGTH];
     char secondtoken[MAX_LINE_LENGTH];
     char thirdtoken[MAX_LINE_LENGTH];
     char fourthtoken[MAX_LINE_LENGTH];

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering READ_BANCSTAR_SOURCE_CODE\n");
#endif

     strcpy(firsttoken, "");        /*************************************/
     strcpy(secondtoken, "");       /* NOTE: the reason we have to do    */
     strcpy(thirdtoken, "");        /*       things the hard way is      */
     strcpy(fourthtoken, "");       /*       because fscanf() will read  */
                                    /*       across line boundaries to   */
                                    /*       fill its quota.  That type  */
     i = 0;                         /*       of behavior is exceedingly  */
     j = 0;                         /*       undesirable in this case.   */
                                    /*************************************/

     while ( (line[i] != ',') && (line[i] != NULL) )
     {
          firsttoken[j] = line[i];               /* find the first parameter */
          firsttoken[j+1] = NULL;                /* up to a comma.           */
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
          secondtoken[j] = line[i];             /* find the second parameter */
          secondtoken[j+1] = NULL;              /* (if any) up to a comma.   */
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
          thirdtoken[j] = line[i];               /* find the third parameter */
          thirdtoken[j+1] = NULL;                /* (if any) up to a comma.  */
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
          fourthtoken[j] = line[i];             /* find the fourth parameter */
          fourthtoken[j+1] = NULL;              /* (if any) up to a comma.   */
          ++j;
          ++i;
     }

     parameters->L1 = atoi(firsttoken);        /* convert them to integers */
     parameters->L2 = atoi(secondtoken);       /* and return.              */
     parameters->L3 = atoi(thirdtoken);
     parameters->L4 = atoi(fourthtoken);

#ifdef DIAGNOSTICS
     fprintf(stdout, "parameters = %d,%d,%d,%d\n", parameters->L1,
             parameters->L2, parameters->L3, parameters->L4);
     fprintf(stderr, "exit READ_BANCSTAR_SOURCE_CODE\n");
#endif

     return(0);
}


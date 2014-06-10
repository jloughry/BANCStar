
#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  GET_SHORT_PROMPT: validate the number n in [1-2000] and return a         **
**                    shortened version of the prompt name, with ellipses... **
**                                                                           **
******************************************************************************/

int get_short_prompt( char *name, Promptfile prompt, int n,
                     long line_number )
{
     Reflist_ptr reflist_alloc( void );
     Reflist_ptr pointer;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering GET_SHORT_PROMPT at line %ld\n", line_number);
#endif

     if ((n >= 1) && (n <= MAX_NUM_PROMPTS))
     {
          ++prompt[n].referenced;              /* track number of times used */

          pointer = prompt[n].reference_list;               /***************/
                                                            /* follow the  */
          if ( pointer == NULL )                            /* linked list */
          {                                                 /* of elements */
               prompt[n].reference_list = reflist_alloc();  /* to the end. */
               pointer = prompt[n].reference_list;          /***************/

               pointer->linenumber = line_number;
               pointer->next = NULL;
          }
          else
          {
               while ( pointer->next != NULL )
                    pointer = pointer->next;

               if ( pointer->linenumber != line_number )
               {
                    pointer->next = reflist_alloc();
                    pointer = pointer->next;

                    pointer->linenumber = line_number;
                    pointer->next = NULL;
               }
          }

          strcpy(name, prompt[n].name);

/*
          if (strlen(name) > SHORT_PROMPT_LENGTH)
          {
               first_space = strlen(name) - strlen(strstr(name, " "));
               strncpy(name, prompt[n].name,
                       MAX(first_space, SHORT_PROMPT_LENGTH));
               strcat(name, "...");
          }
*/

          return(0);
     }
     else
     {
          fprintf(stdout, "prompt(%d): prompt reference out of bounds in line %ld\n", n, line_number);
          exit(-1);
     }

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit GET_SHORT_PROMPT\n");
#endif

}


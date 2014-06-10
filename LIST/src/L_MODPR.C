
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  GET_MODIFIED_PROMPT: return the name of prompt n being modified.        **
**                                                                          **
*****************************************************************************/

int get_modified_prompt( char *name, Promptfile prompt, int n,
                         long line_number )
{
     Reflist_ptr reflist_alloc( void );
     Reflist_ptr pointer;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering GET_MODIFIED_PROMPT at line %ld\n", line_number);
#endif

     if ((n >= 1) && (n <= MAX_NUM_PROMPTS))
     {
          ++prompt[n].modified;            /* track how many times modified */
          ++prompt[n].referenced;          /* and how many times used       */

          /*
          **  Update the linked list of pointers for REFERENCED.
          */

          pointer = prompt[n].referenced_list;              /***************/
                                                            /* follow the  */
          if ( pointer == NULL )                            /* linked list */
          {                                                 /* of elements */
               prompt[n].referenced_list = reflist_alloc(); /* to the end. */
               pointer = prompt[n].referenced_list;         /***************/

               pointer->linenumber = line_number;
               pointer->next = NULL;

          }
          else
          {
               while ( pointer->next != NULL )
                    pointer = pointer->next;

               if ( pointer -> linenumber != line_number )
               {
                    pointer->next = reflist_alloc();
                    pointer = pointer->next;

                    pointer->linenumber = line_number;
                    pointer->next = NULL;
               }
          }

          /*
          **  Update the linked list of pointers for MODIFIED.
          */

          pointer = prompt[n].modified_list;                /***************/
                                                            /* follow the  */
          if ( pointer == NULL )                            /* linked list */
          {                                                 /* of elements */
               prompt[n].modified_list = reflist_alloc();   /* to the end. */
               pointer = prompt[n].modified_list;           /***************/

               pointer->linenumber = line_number;
               pointer->next = NULL;

          }
          else
          {
               while ( pointer->next != NULL )
                    pointer = pointer->next;

               if ( pointer -> linenumber != line_number )
               {
                    pointer->next = reflist_alloc();
                    pointer = pointer->next;

                    pointer->linenumber = line_number;
                    pointer->next = NULL;
               }
          }

          strcpy(name, prompt[n].name);
          return(0);
     }
     else
     {
          fprintf(stdout, "prompt(%d): prompt reference out of bounds in line %ld\n", n, line_number);

          exit(-1);
     }

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit GET_MODIFIED_PROMPT\n");
#endif

}


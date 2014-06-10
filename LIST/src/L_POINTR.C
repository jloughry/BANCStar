
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  REFLIST_ALLOC: return a pointer to a data structure capable of          **
**                 holding one element of the linked list of prompt lines   **
**                 referenced.                                              **
**                                                                          **
**                 For more information see Kernighan & Ritchie, p.146.     **
**                                                                          **
*****************************************************************************/

Reflist_ptr reflist_alloc( void )
{
     Reflist_ptr pointer;

#ifdef DIAGNOSTICS
     fprintf(stderr, "Entering REFLIST_ALLOC\n");
     fprintf(stderr, "Memory available = %u\n", _memavl());
     fprintf(stderr, "Approximate number of items available: %u\n", _freect(sizeof( Reference_node)));
#endif

     if ( ( pointer = (Reflist_ptr)malloc(sizeof( Reference_node ) ) ) == NULL )
     {
          fprintf(stdout, "LIST: error allocating memory for reference list.\n");
          exit(-1);
     }

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit REFLIST_ALLOC\n");
#endif

     return( pointer );
}


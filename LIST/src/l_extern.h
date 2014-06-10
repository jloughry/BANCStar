
/*****************************************************************************
**                                                                          **
**  include files.  These are #included in all source files.                **
**                                                                          **
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <dos.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <graph.h>

#include "l_define.h"


/*****************************************************************************
**                                                                          **
**  external global variable declarations.  These are external to all       **
**                                          source files except MAIN().     **
**                                                                          **
*****************************************************************************/

typedef int boolean;

/*
**  Operating system variables and file pointers.
*/

extern FILE *fp_input, *fp_output, *fp_check, *fp_prompt, *fp_report, *fp_main,
            *fopen();

extern char filename[_MAX_PATH], longfilename[_MAX_PATH], pathname[_MAX_PATH],
     drive[_MAX_DRIVE], subdir[_MAX_DIR], file[_MAX_FNAME], ext[_MAX_EXT],
     name[_MAX_PATH];

extern unsigned long total_disk_space, free_disk_space, bytes_per_disk_cluster;
extern struct diskfree_t dfinfo;

extern int console_input;

extern int done;
extern struct find_t dta;

extern struct dosdate_t startdate;
extern struct dostime_t starttime;
extern struct dosdate_t enddate;
extern struct dostime_t endtime;

typedef struct window_t {
   short row1;
   short col1;
   short row2;
   short col2;
} Window;

typedef enum s_system { LIABILITIES, CONSUMER, RATESTAR } Sellstar_system;
typedef enum u_table { DESCRIPTION, ITEM } User_table;

/*
**  Structures and variables required to handle the prompt file.
*/

typedef struct refnode *Reflist_ptr;

typedef struct refnode {
     long linenumber;
     Reflist_ptr next;
} Reference_node;

extern char promptfilename[_MAX_PATH];
extern int promptfile_handle;
extern unsigned pdate;
extern unsigned ptime;

struct pfile {
     int  type;
     int  length;
     int  referenced;
     int  displayed;
     int  modified;
     Reflist_ptr referenced_list;
     Reflist_ptr displayed_list;
     Reflist_ptr modified_list;
     char *name;                        /* place variable-length items at end */
     char *contents;
};

typedef struct pfile * Promptfile;

/*
**  Structures and variables required to handle MAIN.M.
*/

struct usertable {
     char *description;
     char *item;
};

typedef struct usertable * MainDotM;

/*
**  Structures and variables required to handle and format the input code.
*/

struct sourceline {
     int L1;
     int L2;
     int L3;
     int L4;
};

struct sourceline_string {
     char L1_string[MAX_LINE_LENGTH];
     char L2_string[MAX_LINE_LENGTH];
     char L3_string[MAX_LINE_LENGTH];
     char L4_string[MAX_LINE_LENGTH];
};

/*
**  This array holds the names of the various prompt types.
*/

extern char prompt_type[9][10];

/*
**  And these are the names of the colors used in windows.
*/

extern char bancstar_color[16][32];

/*
**  Miscellaneous global variables.
*/
           /* these should probably be defined locally in list() */

extern int external_references, data_model_references;
extern char *external_line[MAX_EXTERNALS];
extern char *data_model_line[MAX_EXTERNALS];

extern boolean top_of_page;
extern boolean end_of_page;
extern boolean partial_page;

         /* these should probably be static local to indentation(). */
       /* (used to maintain the current level of comment indentation) */

extern int indent_level, I2, I3, NX, FB, RB;



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
**  global variable declarations.  These are formally defined only in       **
**                                 MAIN(); they are external to all other   **
**                                 source files.                            **
**                                                                          **
*****************************************************************************/

typedef int boolean;

/*
**  Operating system variables and file pointers.
*/

FILE *fp_input, *fp_output, *fp_check, *fp_prompt, *fp_report, *fp_main,
     *fopen();

char filename[_MAX_PATH], longfilename[_MAX_PATH], pathname[_MAX_PATH],
     drive[_MAX_DRIVE], subdir[_MAX_DIR], file[_MAX_FNAME], ext[_MAX_EXT],
     name[_MAX_PATH];

unsigned long total_disk_space, free_disk_space, bytes_per_disk_cluster;
struct diskfree_t dfinfo;

int console_input;

int done;
struct find_t dta;

struct dosdate_t startdate;
struct dostime_t starttime;
struct dosdate_t enddate;
struct dostime_t endtime;

typedef enum s_system { LIABILITES, CONSUMER, RATESTAR } Sellstar_system;
typedef enum u_table { DESCRIPTION, ITEM } User_table;

/*
**  Structures and variables required to handle the prompt file.
*/

typedef struct refnode *Reflist_ptr;

typedef struct refnode {
     long linenumber;
     Reflist_ptr next;
} Reference_node;

char promptfilename[_MAX_PATH];
int promptfile_handle;
unsigned pdate;
unsigned ptime;

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

char prompt_type[9][10] = { "    ", "PHONE", "S.S.#", "DATE", "NUMBER",
                            "DOLLAR", "RATE", "ALPHA", "CONSTANT" };

/*
**  And these are the names of the colors used in windows.
*/

char bancstar_color[16][32] = { "Black", "Blue", "Green", "Cyan", "Red",
                                "Magenta", "Brown", "White", "Blinking Grey",
                                "Blinking Blue", "Blinking Green",
                                "Blinking Cyan", "Blinking Red",
                                "Blinking Magenta", "Blinking Yellow",
                                "Blinking White" };

/*
**  Miscellaneous global variables.
*/
           /* these should probably be defined locally in list() */

int external_references, data_model_references;
char *external_line[MAX_EXTERNALS];
char *data_model_line[MAX_EXTERNALS];

boolean top_of_page;
boolean end_of_page;
boolean partial_page;

         /* these should probably be static local to indentation(). */
       /* (used to maintain the current level of comment indentation) */

int indent_level, I2, I3, NX, FB, RB;


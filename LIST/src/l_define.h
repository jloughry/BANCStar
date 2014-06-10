
/*****************************************************************************
**                                                                          **
**  compiler directives.  These are #defined in all source files.           **
**                                                                          **
*****************************************************************************/

#define NAME                 "LIST Version 2.2b Revision 04-11-93"
#define BANNER               "LIST Screen File Formatter Version 2.2b                       Revision 04-11-93\n"
#define COPYRIGHT            "Copyright (C) 1990, 1991, 1993 Joe Loughry. All rights reserved.\n"
#define USAGE                "Usage:list [/prompt][/sN][/eN][/a][/d][/f][/h][/n][/x][/z] filename.ext\n"

                                       /***************************************/
#define MAX_NUM_PROMPTS      2000      /* size of the prompt file             */
#define MAX_PROMPT_LENGTH    102       /* required by halloc() -- see pfile() */
#define SHORT_PROMPT_LENGTH  8         /* truncated length of a prompt name   */
#define MAX_LINE_LENGTH      250       /* maximum length of any line alloc.   */
#define MAX_EXTERNALS        4096      /* maximum number of gotos remembered  */
#define A_VERY_LARGE_NUMBER  99999999  /* highest line number supported       */
#define EXPANSION_FACTOR     10        /* used to determine space on disk     */
#define MAX_LENGTH_TABLE     1001      /* number of lines in MAIN.M           */
                                       /***************************************/

#define TRUE                 1
#define FALSE                0

#define MAX(a,b)             (a>b) ? a : b

#define BLACK                0L
#define BLUE                 1L        /* default background text color */
#define GREEN                2L
#define CYAN                 3L
#define RED                  4L
#define MAGENTA              5L
#define BROWN                6L
#define WHITE                7L
#define GREY                 8L
#define BRIGHT_BLUE          9L
#define BRIGHT_GREEN         10L
#define BRIGHT_CYAN          11L
#define BRIGHT_RED           12L
#define BRIGHT_MAGENTA       13L
#define BRIGHT_YELLOW        14L
#define BRIGHT_WHITE         15L       /* default foreground text color */

                                       /***************************************/
#define P_PHONE              1         /* code in prompt file for a Phone No. */
#define P_SSNUMBER           2         /* Social Security or Tax ID Number    */
#define P_DATE               3         /* Date prompt                         */
#define P_NUMBER             4         /* Numeric prompt                      */
#define P_DOLLAR             5         /* Dollar prompt                       */
#define P_RATE               6         /* Rate prompt                         */
#define P_ALPHA              7         /* Alpha prompt                        */
#define P_CONSTANT           8         /* CONSTANT type in prompt file        */
                                       /***************************************/


/*
**  Where we expect to find various prompt files.  This part is site-dependent.
*/

#define PROMPT               "c:\\PROMPT.PFL"
#define ICL                  "c:\\genstar\\loandoc\\icl.pfl"
#define RATES                "c:\\ratestar\\rates.pfl"
#define LIAB                 "c:\\genstar\\liabdoc\\liab.pfl"
#define OTHER                "c:\\genstar\\liabdoc\\other.pfl"

/*
**  Subdirectories where we keep our screens and forms.
*/

#define SCREENS              "c:\\genstar\\"
#define FORMS                "c:\\genstar\\"

#define CONVERSION           "c:\\banker\\forms\\scn\\"

#define LIABILITIES_MAIN_M   "c:\\genstar\\liabdoc\\main.01l"
#define CONSUMER_MAIN_M      "c:\\genstar\\loandoc\\main.01c"
#define RATESTAR_MAIN_M      "d:\\ratestar\\main.m"
#define DEFAULT_MAIN_M       "c:\\main.m"

#define LIABILITIES_HELPFILE_DIR        "c:\\bancstar\\help1\\"
#define CONSUMER_HELPFILE_DIR           "c:\\loanstar\\help\\"

/*
**  Various files we create and commands we use to print them.
*/

#define LISTFILE             "c:\\listfile.rjl"
#define REPORTFILE           "c:\\listrept.rjl"
#define TEMPFILE             "c:\\listfile.tmp"
#define CANCEL_PRINTING      "c:\\dos\\print c:\\listfile.rjl /c"
#define SEND_TO_PRINTER      "c:\\dos\\print c:\\listfile.rjl"
#define SEND_TO_BATCH_PRINT  "copy c:\\listfile.rjl > prn"


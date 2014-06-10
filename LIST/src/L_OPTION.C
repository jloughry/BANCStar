
#include "l_extern.h"

/*****************************************************************************
**                                                                          **
**  READ_COMMAND_LINE_OPTIONS: search the command line for options of the   **
**                             form "/n" and figure out what they mean.     **
**                                                                          **
**                             Give the user help if "/H" or "/h" appear.   **
**                                                                          **
**  input:  argc and argv from main()                                       **
**                                                                          **
**  output: *start_listing_here, *end_listing_there, *nocheck,              **
**          *report_only, *list_only, *extended_report, *file_only,         **
**          *convert_file, *laser_option, *screen_print_option,             **
**          *starting_where, *main_m               [returns to setup()]     **
**                                                                          **
**                                                                          **
**  returns: (0) if successful.  (If the user asks for help and yet         **
**           provides enough information to continue, then do so.)          **
**                                                                          **
**  modified:   05-08-93 -- added code to support conversion of BS prompt-  **
**              file to BB data dictionary.                                 **
**                                                                          **
*****************************************************************************/

int read_command_line_options ( int argc, char **argv, long *start_listing_here,
                                long *end_listing_there, boolean *nocheck,
                                boolean *report_only, boolean *list_only,
                                boolean *extended_report, boolean *file_only,
                                boolean *convert_file, boolean *laser_option,
                                boolean *screen_print_option,
                                boolean *convert_data_dictionary,
                                int *starting_where,
                                Sellstar_system *main_m ) {

     int help( void );                /* function to print the Help message */
     int secret_option ( void );

     int i;                           /* loop index */
     char *n;                         /* for reading the options themselves */

     /*
     **  get and decode the command line options.
     */

     *start_listing_here  = 1;
     *end_listing_there   = A_VERY_LARGE_NUMBER;
     *nocheck             = FALSE;
     *report_only         = FALSE;
     *list_only           = FALSE;
     *extended_report     = TRUE;
     *file_only           = FALSE;
     *convert_file        = FALSE;
     *laser_option        = FALSE;
     *screen_print_option = FALSE;
     *convert_data_dictionary = FALSE;
     *starting_where      = 1;
     *main_m              = LIABILITIES;

     for (i=1; i<=argc; i++)
     {
          /*
          **  ICL promptfile.
          */

          if ((strstr(argv[i], "/i") != NULL) ||
              (strstr(argv[i], "/I") != NULL) ) {
                 strcpy(promptfilename, ICL);
                 *main_m = CONSUMER;
          }

          /*
          **  RATES promptfile.
          */

          if ((strstr(argv[i], "/r") != NULL) ||
              (strstr(argv[i], "/R") != NULL) ) {
                 strcpy(promptfilename, RATES);
                 *main_m = RATESTAR;
          }

          /*
          **  LIAB promptfile.
          */

          if ((strstr(argv[i], "/l") != NULL) ||
              (strstr(argv[i], "/L") != NULL) ) {
                 strcpy(promptfilename, LIAB);
                 *main_m = LIABILITIES;
          }

          /*
          **  OTHER promptfile.
          */

          if ((strstr(argv[i], "/o") != NULL) ||
              (strstr(argv[i], "/O") != NULL) ) {
                 strcpy(promptfilename, OTHER);
                 *main_m = LIABILITIES;
          }

          /*
          **  Arbitrary prompt file.
          */

          if ((n = strstr(argv[i], "/p")) != NULL)
               strcpy(promptfilename, n+2);

          if ((n = strstr(argv[i], "/P")) != NULL)
               strcpy(promptfilename, n+2);

          /*
          **  Convert the input file from BANCStar to BRANCHBANKER format.
          */

          if ((strstr(argv[i], "/c") != NULL) ||
              (strstr(argv[i], "/C") != NULL) )
                 *convert_file = TRUE;

          /*
          **  Translate the prompt file to a BB data dictionary.
          */

          if ((n = strstr(argv[i], "/g")) != NULL) {
               *starting_where = atoi(n+2) - 1;
               *convert_data_dictionary = TRUE;
          }

          if ((n = strstr(argv[i], "/G")) != NULL) {
               *starting_where = atoi(n+2) - 1;
               *convert_data_dictionary = TRUE;
          }

          /*
          **  Format the output for the IBM laser printer and send to file.
          */

          if ((strstr(argv[i], "/d") != NULL) ||
              (strstr(argv[i], "/D") != NULL) )
                 *laser_option = TRUE;

          /*
          **  Generate screen print of each page at end of listing.
          */

          if ((strstr(argv[i], "/b") != NULL) ||
              (strstr(argv[i], "/B") != NULL) )
                 *screen_print_option = TRUE;

          /*
          **  Don't perform a syntax check this time.
          */

          if ((strstr(argv[i], "/n") != NULL) ||
              (strstr(argv[i], "/N") != NULL) )
                 *nocheck = TRUE;

          /*
          **  Generate a listing only, without the report.
          */

          if ((strstr(argv[i], "/a") != NULL) ||
              (strstr(argv[i], "/A") != NULL) )
                 *list_only = TRUE;

          /*
          **  "Just give me the bottom line."
          */

          if ((strstr(argv[i], "/z") != NULL) ||
              (strstr(argv[i], "/Z") != NULL) )
                 *report_only = TRUE;

          /*
          **  Annie wants to send the output to a file.
          */

          if ((strstr(argv[i], "/f") != NULL) ||
              (strstr(argv[i], "/F") != NULL) )
                 *file_only = TRUE;

          /*
          **  Generate extended List Report.
          */

          if ((strstr(argv[i], "/x") != NULL) ||
              (strstr(argv[i], "/X") != NULL) )
                 *extended_report = TRUE;

          /*
          **  Start at this line.
          */

          if ((n = strstr(argv[i], "/s")) != NULL)
               *start_listing_here = atol(n+2) - 1;

          if ((n = strstr(argv[i], "/S")) != NULL)
               *start_listing_here = atol(n+2) - 1;

          /*
          **  End at this line.
          */

          if ((n = strstr(argv[i], "/e")) != NULL)
               *end_listing_there = atol(n+2);

          if ((n = strstr(argv[i], "/E")) != NULL)
               *end_listing_there = atol(n+2);

          /*
          **     HH  HH   EEEEEE   LL       PPPPP    !!   !!   !!
          **     HH  HH   EE       LL       PP  PP   !!   !!   !!
          **     HHHHHH   EEEE     LL       PPPPP    !!   !!   !!
          **     HH  HH   EE       LL       PP
          **     HH  HH   EEEEEE   LLLLLL   PP       !!   !!   !!
          */

          if ((strstr(argv[i], "/h") != NULL) ||
              (strstr(argv[i], "/H") != NULL) )
          {
               help();
          }

          /*
          **  Secret Message . . .
          */

          if ( strstr( argv[i], "/@" ) != NULL )
          {
               secret_option();
          }
     }

     return(0);
}


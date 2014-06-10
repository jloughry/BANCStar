
#include "l_extern.h"

extern unsigned char bold[], boff[];

/*****************************************************************************
**                                                                          **
**  PRINT_LIST_FOOTER: prints the page footer at the end of the listing.    **
**                                                                          **
*****************************************************************************/

int print_list_footer(boolean forward, boolean reverse)
{
     int print_page_break(boolean, boolean);
     long elapsed_time_in_seconds;
     long hours, minutes, seconds;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_LIST_FOOTER\n");
#endif

     /*
     **  Don't print another page break if we've done that already.
     */

     if (!end_of_page)
          print_page_break(forward, reverse);

     /*
     **  Print today's date again and the elapsed CPU time.
     */

     fprintf(fp_output, "\n\n");
     fprintf(fp_output, "%s", bold);
     fprintf(fp_output, "%02d-", startdate.month);

     if (startdate.day < 10)
          fprintf(fp_output, "0%d-", startdate.day);
     else
          fprintf(fp_output, "%d-", startdate.day);

     fprintf(fp_output, "%d / ", startdate.year);

     if (endtime.hour <= 12)
          fprintf(fp_output, "%d:", endtime.hour);
     else
          fprintf(fp_output, "%d:", endtime.hour - 12);

     if (endtime.minute < 10)
          fprintf(fp_output, "0%d:", endtime.minute);
     if (endtime.minute >= 10)
          fprintf(fp_output, "%2d:", endtime.minute);

     if (endtime.second < 10)
          fprintf(fp_output, "0%d ", endtime.second);
     if (endtime.second >= 10)
          fprintf(fp_output, "%2d ", endtime.second);

     if (endtime.hour < 12)
          fprintf(fp_output, "AM");
     if (endtime.hour >= 12)
          fprintf(fp_output, "PM");

     fprintf(fp_output, "\n");

     elapsed_time_in_seconds = ((endtime.hour - starttime.hour) * 3600) +
                               ((endtime.minute - starttime.minute) * 60) +
                                (endtime.second - starttime.second);

     hours = (elapsed_time_in_seconds / 3600);
     minutes = ((elapsed_time_in_seconds - (hours * 3600)) / 60);
     seconds = elapsed_time_in_seconds - (hours * 3600) - (minutes * 60);

     fprintf(fp_output, "CPU time: ");
     if (hours == 1)
          fprintf(fp_output, "%ld hour ", hours);
     else if ( (hours !=0) && (hours != 1) )
          fprintf(fp_output, "%ld hours ", hours);

     if (minutes == 1)
          fprintf(fp_output, "%ld minute ", minutes);
     else if ( (minutes !=0) && (minutes != 1) )
          fprintf(fp_output, "%ld minutes ", minutes);

     if (seconds == 1)
          fprintf(fp_output, "%ld second", seconds);
     else if ( (seconds !=0) && (seconds != 1) )
          fprintf(fp_output, "%ld seconds", seconds);

     fprintf(fp_output, "%s\n", boff);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit PRINT_LIST_FOOTER\n");
#endif

     return(0);
}


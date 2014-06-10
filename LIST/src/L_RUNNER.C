
#include "l_extern.h"

extern unsigned char bold[], boff[];

/******************************************************************************
**                                                                           **
**  GENERATE_HEADER_MACRO: prints page headers for laser option listing.     **
**                                                                           **
******************************************************************************/

int generate_header_macro ( char *filename, long total_lines ) {
     int print_page_break( boolean, boolean );
     unsigned getbits( unsigned, int, int );
     char *long_numstring ( long );
     char *spaces ( int );

     char spaces_string[MAX_LINE_LENGTH];
     int i, j_length;

     /*
     **  Required to get the date and time of last modification of a file.
     */

     struct f_date {
          int day;
          int month;
          int year;
          int hour;
          int minute;
          int second;
     } file_date;

     unsigned fdate, ftime;       /* file date and time packed into integers */
     int file_handle;

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_RUNNING_PAGE_HEADER\n");
#endif

     /*
     **  First, send the macro header to the printer.
     */

     /* (IBM Laser Printer macro commands not implemented in this version ) */

     /*
     **  Print today's date at the top of the first page.
     */

     fprintf(fp_output, "%s", bold);
     fprintf(fp_output, "%02d-", startdate.month);

     if (startdate.day < 10)
          fprintf(fp_output, "0%d-", startdate.day);
     else
          fprintf(fp_output, "%d-", startdate.day);

     fprintf(fp_output, "%d", startdate.year);

     /*
     **  The neatly center the filename . . .
     */

     for (i=1; i<=(28 - (strlen(filename) / 2)); i++)
          fprintf(fp_output, " ");
     fprintf(fp_output, "%s", filename);

     if (total_lines < 10)            /* sure it's silly, but it works */
          j_length = 1;
     else if (total_lines < 100)
          j_length = 2;
     else if (total_lines < 1000)
          j_length = 3;
     else if (total_lines < 10000)
          j_length = 4;
     else if (total_lines < 100000)
          j_length = 5;
     else if (total_lines < 1000000)
          j_length = 6;

     for (i=(38+(strlen(filename) / 2)); i<=(80-9-j_length); i++)
          fprintf(fp_output, " ");

     /*
     **  . . . and print the size of the file in lines.
     */

     if (total_lines == 1)
          fprintf(fp_output, " %ld line\n", total_lines);
     else
          fprintf(fp_output, "%ld lines\n", total_lines);

     /*
     **  Print the current time on the next line.
     */

     if (starttime.hour <= 12)
          fprintf(fp_output, "%d:", starttime.hour);
     else
          fprintf(fp_output, "%d:", starttime.hour - 12);

     if (starttime.minute < 10)
          fprintf(fp_output, "0%d:", starttime.minute);
     if (starttime.minute >= 10)
          fprintf(fp_output, "%2d:", starttime.minute);

     if (starttime.second < 10)
          fprintf(fp_output, "0%d ", starttime.second);
     if (starttime.second >= 10)
          fprintf(fp_output, "%2d ", starttime.second);

     if (starttime.hour < 12)
          fprintf(fp_output, "AM");
     if (starttime.hour >= 12)
          fprintf(fp_output, "PM");

     /*
     **  Get the file date and time from DOS.
     */

     file_handle = fileno(fp_input);
     _dos_getftime(file_handle, &fdate, &ftime);

     file_date.day = getbits(fdate, 4, 5);
     file_date.month = getbits(fdate, 8, 4);
     file_date.year = getbits(fdate, 15, 7) + 1980;

     file_date.second = getbits(ftime, 4, 5) * 2;
     file_date.minute = getbits(ftime, 10, 6);
     file_date.hour = getbits(ftime, 15, 5);

     /*
     ** Date and time of last modification:
     */
                                                       /*****************/
     fprintf(fp_output, "          ");                 /*   ten spaces  */
                                                       /* for centering */
     if ( ( starttime.hour < 10 ) ||                   /*****************/
          ( starttime.hour > 12 ) )                    /* another space */
          fprintf(fp_output, " ");                     /*****************/

     fprintf(fp_output, "%sLast modified %02d-", boff, file_date.month);

     if (file_date.day < 10)
          fprintf(fp_output, "0%d-", file_date.day);
     else
          fprintf(fp_output, "%d-", file_date.day);

     fprintf(fp_output, "%d at ", file_date.year);

     if (file_date.hour <= 12)
          fprintf(fp_output, "%d:", file_date.hour);
     else
          fprintf(fp_output, "%d:", file_date.hour - 12);

     if (file_date.minute < 10)
          fprintf(fp_output, "0%d ", file_date.minute);
     if (file_date.minute >= 10)
          fprintf(fp_output, "%2d ", file_date.minute);

     if (file_date.hour < 12)
          fprintf(fp_output, "AM");
     if (file_date.hour >= 12)
          fprintf(fp_output, "PM");

     fprintf(fp_output, "%s", bold);

     /*
     **  Size of the file in bytes.
     */

     strcpy( spaces_string, spaces(22 - strlen(long_numstring(dta.size)) - 6 ));
     fprintf( fp_output, "%s %ld bytes",
              spaces_string,
              dta.size
     );

     /*
     **  end the line.
     */

     fprintf(fp_output, "%s\n\n", boff);

     /*
     **  Now print the first page break and away we go!
     */

     print_page_break(FALSE, FALSE);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit PRINT_RUNNING_PAGE_HEADER\n");
#endif

     return(0);
}


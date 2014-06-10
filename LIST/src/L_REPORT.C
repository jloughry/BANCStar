
#include "l_extern.h"

extern unsigned char bold[], boff[], ital[], itoff[];

/*****************************************************************************
**                                                                          **
**  PRINT_LIST_REPORT: prints a nice report at the end of the run.          **
**                                                                          **
*****************************************************************************/

int print_list_report( char *file_name, Promptfile prompt,
                       long numlines, boolean extended_report )
{
     unsigned getbits( unsigned, int, int );
     char *long_numstring( long );
     char *spaces( int );

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
     } file_date, promptfile_date;

     int i, fprintf_length, prompt_ref_count;
     boolean none_displayed, none_referenced, none_modified;
     Reflist_ptr prompt_ref_pointer, old_pointer;

     unsigned fdate;          /* file date and time (packed into integers) */
     unsigned ftime;
     int file_handle;                      /**********************************/
                                           /* to avoid problems with sending */
     char spaces_line[MAX_LINE_LENGTH];    /* the function spaces() directly */
     char spaces_string[MAX_LINE_LENGTH];  /* to fprintf().  [doesn't work.] */
                                           /**********************************/
#ifdef DIAGNSOTICS
     fprintf(stderr, "entering PRINT_LIST_REPORT\n");
#endif

     /*
     **  All of this stuff should be perfectly centered.
     */

     fprintf( fp_output, "                                  %sLIST Report\n\n",
                        bold );

     strcpy( spaces_line, spaces( 38 - strlen( long_numstring( numlines ) ) - 6 - ( strlen( file_name ) / 2 ) ) );

     if ( numlines != 1 )
     {
          fprintf( fp_output, "%ld lines %s %s %n",
                               numlines,
                               spaces_line,
                               file_name,
                               &fprintf_length       /*********************/
                 );                                  /* NOTE: attributes! */
     }                                               /*********************/
     else
     {
          fprintf( fp_output, "%ld line  %s %s %n",
                               numlines,
                               spaces_line,
                               file_name,
                               &fprintf_length       /*********************/
                 );                                  /* NOTE: attributes! */
     }                                               /*********************/

     strcpy( spaces_line, spaces ( 80 - fprintf_length - strlen ( long_numstring ( dta.size ) ) - 7 ) );

     if ( dta.size != 1 )
     {
          fprintf( fp_output, "%s %ld bytes%s\n\n",
                               spaces_line,
                               dta.size,
                               boff
                 );
     }
     else
     {
          fprintf( fp_output, "%s %ld  byte%s\n\n",
                               spaces_line,
                               dta.size,
                               boff
                 );
     }

     /*
     ** get the file dates and times from DOS.
     */

     file_handle = fileno(fp_input);
     _dos_getftime(file_handle, &fdate, &ftime);

     file_date.day = getbits(fdate, 4, 5);
     file_date.month = getbits(fdate, 8, 4);
     file_date.year = getbits(fdate, 15, 7) + 1980;

     file_date.second = getbits(ftime, 4, 5) * 2;
     file_date.minute = getbits(ftime, 10, 6);
     file_date.hour = getbits(ftime, 15, 5);

     promptfile_date.day = getbits(pdate, 4, 5);
     promptfile_date.month = getbits(pdate, 8, 4);
     promptfile_date.year = getbits(pdate, 15, 7) + 1980;

     promptfile_date.second = getbits(ptime, 4, 5) * 2;
     promptfile_date.minute = getbits(ptime, 10, 6);
     promptfile_date.hour = getbits(ptime, 15, 5);

     /*
     ** Date and time of last modification:
     */

     fprintf(fp_output, "Last modified %02d-",file_date.month);

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

     /*
     ** Date and time when printed:
     */

     if (file_date.hour < 10)                    /**********************/
          fprintf(fp_output, " ");               /*  extra spaces for  */
                                                 /*      right margin  */
     if (file_date.hour > 12)                    /*         alignment  */
          if ( (file_date.hour - 12) < 10 )      /**********************/
               fprintf(fp_output, " ");

     if (starttime.hour < 10)
          fprintf(fp_output, " ");

     if (starttime.hour > 12)
          if ( (starttime.hour - 12) < 10 )
               fprintf(fp_output, " ");

     fprintf(fp_output, "              Printed %02d-", startdate.month);

     if (startdate.day < 10)
          fprintf(fp_output, "0%d-", startdate.day);
     else
          fprintf(fp_output, "%d-", startdate.day);

     fprintf(fp_output, "%d at ", startdate.year);

     if (starttime.hour <= 12)
          fprintf(fp_output, "%d:", starttime.hour);
     else
          fprintf(fp_output, "%d:", starttime.hour - 12);

     if (starttime.minute < 10)
          fprintf(fp_output, "0%d ", starttime.minute);
     if (starttime.minute >= 10)
          fprintf(fp_output, "%2d ", starttime.minute);

     if (starttime.hour < 12)
          fprintf(fp_output, "AM\n");
     if (starttime.hour >= 12)
          fprintf(fp_output, "PM\n");

     /*
     ** Date and time of prompt file:
     */

     fprintf(fp_output, "\nPromptfile %s was last modified %02d-",
             strupr(promptfilename),
             promptfile_date.month);

     if (promptfile_date.day < 10)
          fprintf(fp_output, "0%d-", promptfile_date.day);
     else
          fprintf(fp_output, "%d-", promptfile_date.day);

     fprintf(fp_output, "%d at ", promptfile_date.year);

     if (promptfile_date.hour <= 12)
          fprintf(fp_output, "%d:", promptfile_date.hour);
     else
          fprintf(fp_output, "%d:", promptfile_date.hour - 12);

     if (promptfile_date.minute < 10)
          fprintf(fp_output, "0%d ", promptfile_date.minute);
     if (promptfile_date.minute >= 10)
          fprintf(fp_output, "%2d ", promptfile_date.minute);

     if (promptfile_date.hour < 12)
          fprintf(fp_output, "AM\n");
     if (promptfile_date.hour >= 12)
          fprintf(fp_output, "PM\n");

     /*
     ** External references: (any line with a GOTO in it is reproduced here)
     */

     fprintf(fp_output, "\n---\n");

     fprintf(fp_output, "\n%sLines Containing External References:%s ", bold, boff);

     if (external_references == 0)
          fprintf(fp_output, "(none)\n");
     else
     {
          fprintf(fp_output, "\n\n");
          for (i=0; i<external_references; i++)
          {
               fprintf(fp_output, "%s", external_line[i]);
               free(external_line[i]);
          }

          if ( external_references >= MAX_EXTERNALS )
               fprintf(fp_output, "\n-- more --\n");
     }

     /*
     ** Data Model: (any line beginning with [9200...9401] is reproduced here)
     */

     fprintf(fp_output, "\n---\n");

     fprintf(fp_output, "\n%sLines Containing Data Model Calls:%s ", bold, boff);

     if (data_model_references == 0)
          fprintf(fp_output, "(none)\n");
     else
     {
          fprintf(fp_output, "\n\n");
          for (i=0; i<data_model_references; i++)
          {
               fprintf(fp_output, "%s", data_model_line[i]);
               free(data_model_line[i]);
          }

          if ( data_model_references >= MAX_EXTERNALS )
               fprintf(fp_output, "\n-- more --\n");
     }

     none_displayed = TRUE;        /* so that we can print "none" */
     none_referenced = TRUE;       /* if there aren't any.        */
     none_modified = TRUE;

     for (i=1; i<=MAX_NUM_PROMPTS; i++)
     {
          if (prompt[i].displayed > 0)
               none_displayed = FALSE;

          if (prompt[i].referenced > 0)
               none_referenced = FALSE;

          if (prompt[i].modified > 0)
               none_modified = FALSE;
     }
     fprintf(fp_output, "\n---\n");

     /*
     ** Prompts displayed in this screen:
     */

     _splitpath(longfilename, drive, subdir, file, ext);

     fprintf(fp_output, "\n%sPrompts Displayed in This ", bold);

     if (strcmp(strupr(ext), ".SCN") == 0)
          fprintf(fp_output, "Screen:%s ", boff);
     else if (strcmp(strupr(ext), ".FRM") == 0)
          fprintf(fp_output, "Form:%s ", boff);
     else
          fprintf(fp_output, "File:%s ", boff);

     if (none_displayed)
          fprintf(fp_output, "(none)\n");
     else
     {
          fprintf(fp_output, "\n\n");

          if ( extended_report )
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES          WHERE               DISPLAYED %s\n\n",
                        ital, itoff );
          }
          else
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES                              DISPLAYED %s\n\n",
                        ital, itoff );
          }

          for (i=1; i<=MAX_NUM_PROMPTS; i++)
          {
               if(prompt[i].displayed > 0)
               {
                    if (i<10)
                         fprintf(fp_output, "   %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<100)
                         fprintf(fp_output, "  %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<1000)
                         fprintf(fp_output, " %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else fprintf(fp_output, "%d) %s", i,
                                prompt_type[prompt[i].type]);

                    fprintf(fp_output, "\t ");

                    if (prompt[i].length < 100)
                         fprintf(fp_output, " ");

                    if (prompt[i].length < 10)
                         fprintf(fp_output, " ");

                    if (prompt[i].length > 0)
                    {
                         strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                         fprintf( fp_output, "%d  %s %s%5d\n", prompt[i].length,
                                  prompt[i].name,
                                  spaces_line,
                                  prompt[i].displayed);

                         if ( extended_report )
                         {
                              /* do extended report stuff here */

                              strcpy(spaces_string, spaces(50));
                              fprintf(fp_output, "%s", spaces_string);
                              prompt_ref_count = 0;

                              prompt_ref_pointer = prompt[i].displayed_list;

                              while ( prompt_ref_pointer != NULL )
                              {
                                   ++prompt_ref_count;

                                   fprintf( fp_output, "[%ld] ",
                                            prompt_ref_pointer->linenumber );

                                   old_pointer = prompt_ref_pointer;
                                   prompt_ref_pointer = prompt_ref_pointer->next;
                                   free( old_pointer );

                                   if ( prompt_ref_count > 3 )
                                   {
                                        strcpy(spaces_string, spaces(50));
                                        fprintf(fp_output, "\n%s", spaces_string);
                                        prompt_ref_count = 0;
                                   }
                              }
                              fprintf(fp_output, "\n");
                         }
                    }
                    else
                    {
                         if (strcmp(prompt[i].name, "-->UNDEFINED<--") != 0)
                         {
                              strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                              fprintf( fp_output, "   %s %s%5d\n",
                                       prompt[i].name,
                                       spaces_line,
                                       prompt[i].displayed);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].displayed_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                         else
                         {
                              strcpy(spaces_line, spaces(67));

                                   fprintf( fp_output, "%s %5d\n",
                                            spaces_line,
                                            prompt[i].displayed);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].displayed_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                    }
               }
          }
     }

     /*
     ** Prompts referenced in this screen:
     */

     fprintf(fp_output, "\n---\n\n%sPrompts Referred To in This ", bold);

     if (strcmp(strupr(ext), ".SCN") == 0)
          fprintf(fp_output, "Screen:%s ", boff);
     else if (strcmp(strupr(ext), ".FRM") == 0)
          fprintf(fp_output, "Form:%s ", boff);
     else
          fprintf(fp_output, "File:%s ", boff);

     if (none_referenced)
          fprintf(fp_output, "(none)\n");
     else
     {
          fprintf(fp_output, "\n\n");

          if ( extended_report )
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES          WHERE              REFERENCED %s\n\n",
                        ital, itoff );
          }
          else
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES                             REFERENCED %s\n\n",
                        ital, itoff );
          }

          for (i=1; i<=MAX_NUM_PROMPTS; i++)
          {
               if(prompt[i].referenced > 0)
               {
                    if (i<10)
                         fprintf(fp_output, "   %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<100)
                         fprintf(fp_output, "  %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<1000)
                         fprintf(fp_output, " %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else fprintf(fp_output, "%d) %s", i,
                                prompt_type[prompt[i].type]);

                    fprintf(fp_output, "\t ");

                    if (prompt[i].length < 100)
                         fprintf(fp_output, " ");

                    if (prompt[i].length < 10)
                         fprintf(fp_output, " ");

                    if (prompt[i].length > 0)
                    {
                         strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                         fprintf( fp_output, "%d  %s %s%5d\n", prompt[i].length,
                                  prompt[i].name,
                                  spaces_line,
                                  prompt[i].referenced);

                         if ( extended_report )
                         {
                              /* do extended report stuff here */

                              strcpy(spaces_string, spaces(50));
                              fprintf(fp_output, "%s", spaces_string);
                              prompt_ref_count = 0;

                              prompt_ref_pointer = prompt[i].referenced_list;

                              while ( prompt_ref_pointer != NULL )
                              {
                                   ++prompt_ref_count;

                                   fprintf( fp_output, "[%ld] ",
                                            prompt_ref_pointer->linenumber );

                                   old_pointer = prompt_ref_pointer;
                                   prompt_ref_pointer = prompt_ref_pointer->next;
                                   free( old_pointer );

                                   if ( prompt_ref_count > 3 )
                                   {
                                        strcpy(spaces_string, spaces(50));
                                        fprintf(fp_output, "\n%s", spaces_string);
                                        prompt_ref_count = 0;
                                   }
                              }
                              fprintf(fp_output, "\n");
                         }
                    }
                    else
                    {
                         if (strcmp(prompt[i].name, "-->UNDEFINED<--") != 0)
                         {
                              strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                              fprintf( fp_output, "   %s %s%5d\n",
                                       prompt[i].name,
                                       spaces_line,
                                       prompt[i].referenced);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].referenced_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                         else
                         {
                              strcpy(spaces_line, spaces(67));

                              fprintf( fp_output, "%s %5d\n",
                                       spaces_line,
                                       prompt[i].referenced);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].referenced_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                    }
               }
          }
     }

     /*
     ** Prompts modified in this screen:
     */

     fprintf(fp_output, "\n---\n\n%sPrompts Modified in This ", bold);

     if (strcmp(strupr(ext), ".SCN") == 0)
          fprintf(fp_output, "Screen:%s ", boff);
     else if (strcmp(strupr(ext), ".FRM") == 0)
          fprintf(fp_output, "Form:%s ", boff);
     else
          fprintf(fp_output, "File:%s ", boff);

     if (none_modified)
          fprintf(fp_output, "(none)\n");
     else
     {
          fprintf(fp_output, "\n\n");

          if ( extended_report )
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES          WHERE                MODIFIED %s\n\n",
                        ital, itoff );
          }
          else
          {
               fprintf( fp_output, "%s NO.  TYPE     LENGTH BORROWER VARIABLES                               MODIFIED %s\n\n",
                        ital, itoff );
          }

          for (i=1; i<=MAX_NUM_PROMPTS; i++)
          {
               if(prompt[i].modified > 0)
               {
                    if (i<10)
                         fprintf(fp_output, "   %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<100)
                         fprintf(fp_output, "  %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else if (i<1000)
                         fprintf(fp_output, " %d) %s", i,
                                prompt_type[prompt[i].type]);
                    else fprintf(fp_output, "%d) %s", i,
                                prompt_type[prompt[i].type]);

                    fprintf(fp_output, "\t ");

                    if (prompt[i].length < 100)
                         fprintf(fp_output, " ");

                    if (prompt[i].length < 10)
                         fprintf(fp_output, " ");

                    if (prompt[i].length > 0)
                    {
                         strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                         fprintf( fp_output, "%d  %s %s%5d\n", prompt[i].length,
                                  prompt[i].name,
                                  spaces_line,
                                  prompt[i].modified);

                         if ( extended_report )
                         {
                              /* do extended report stuff here */

                              strcpy(spaces_string, spaces(50));
                              fprintf(fp_output, "%s", spaces_string);
                              prompt_ref_count = 0;

                              prompt_ref_pointer = prompt[i].modified_list;

                              while ( prompt_ref_pointer != NULL )
                              {
                                   ++prompt_ref_count;

                                   fprintf( fp_output, "[%ld] ",
                                            prompt_ref_pointer->linenumber );

                                   old_pointer = prompt_ref_pointer;
                                   prompt_ref_pointer = prompt_ref_pointer->next;
                                   free( old_pointer );

                                   if ( prompt_ref_count > 3 )
                                   {
                                        strcpy(spaces_string, spaces(50));
                                        fprintf(fp_output, "\n%s", spaces_string);
                                        prompt_ref_count = 0;
                                   }
                              }
                              fprintf(fp_output, "\n");
                         }
                    }
                    else
                    {
                         if (strcmp(prompt[i].name, "-->UNDEFINED<--") != 0)
                         {
                              strcpy(spaces_line, spaces(47-strlen(prompt[i].name)));

                              fprintf( fp_output, "   %s %s%5d\n",
                                       prompt[i].name,
                                       spaces_line,
                                       prompt[i].modified);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].modified_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                         else
                         {
                              strcpy(spaces_line, spaces(67));

                              fprintf( fp_output, "%s %5d\n",
                                       spaces_line,
                                       prompt[i].modified);

                              if ( extended_report )
                              {
                                   /* do extended report stuff here */

                                   strcpy(spaces_string, spaces(50));
                                   fprintf(fp_output, "%s", spaces_string);
                                   prompt_ref_count = 0;

                                   prompt_ref_pointer = prompt[i].modified_list;

                                   while ( prompt_ref_pointer != NULL )
                                   {
                                        ++prompt_ref_count;

                                        fprintf( fp_output, "[%ld] ",
                                                 prompt_ref_pointer->linenumber );

                                        old_pointer = prompt_ref_pointer;
                                        prompt_ref_pointer = prompt_ref_pointer->next;
                                        free( old_pointer );

                                        if ( prompt_ref_count > 3 )
                                        {
                                             strcpy(spaces_string, spaces(50));
                                             fprintf(fp_output, "\n%s", spaces_string);
                                             prompt_ref_count = 0;
                                        }
                                   }
                                   fprintf(fp_output, "\n");
                              }
                         }
                    }
               }
          }
     }

     /*
     ** End of List Report:
     */

     fprintf(fp_output, "\n---\n\n");

     /*
     **  Print the name of this program (LIST).
     */

     fprintf(fp_output, "%s", bold);
     fprintf(fp_output, NAME);
     fprintf(fp_output, " -- File OK %s\n\n", boff);

#ifdef DIAGNOSTICS
     fprintf(stderr, "Exit PRINT_LIST_REPORT\n");
#endif

     return(0);
}


/*****************************************************************************

LIST(1)                  SELLSTAR Programmer's Manual                  LIST(1)

NAME
     list - produce nicely formatted listings of BANCStar code

SYNOPSIS
     list [/prompt][/sN][/eN][/a][/b][/c][/d][/f][/h][/n][/x][/z] filename.ext

DESCRIPTION
     -1List-0 takes a screen file or form file as input and sends it
     to the printer in a more readable form.  Prompt numbers are set
     in boldface, forward and reverse block conditionals are set off
     and uniquely identified, pages are separated and numbered, and
     the listing is printed with line numbers, perforation breaks, and
     a header showing the filename and date.  Off to the right is a
     fully commented pseudocode translation.  A listing of all of the
     prompts referenced by the screen and other useful information
     is printed at the end.

     -1List-0 has the capability to print the entire file or only a given
     range of lines, and will automatically format and print a list
     of files specified with standard DOS wildcard characters.  It
     automatically looks in the appropriate subdirectory for each file
     depending on that file's extension.  Full pathnames may be specified.

     -1List-0 reads the indicated file(s) and sends its output to stdout.
     Errors are reported to stdout.  Output is normally redirected to
     a temporary file to be printed as a background task by -1Print-0.

     The options, which may appear in any order, are as follows:

     /s-1N-0     Start listing at line -1N-0 of the source file.
     /e-1N-0     Stop listing at line -1N-0 of the source file.
     /a      Print listing only; do not print List Report.
     /d      Format output for the IBM Laser Printer.
     /f      Send output to the file C:\LISTFILE.RJL -- do not print.
     /h      Display Help.
     /n      Do not perform syntax checking.
     /x      Print extended Reference List report. [default]
     /z      Print List report only; do not print listing.

FILES
     list.bas           source code in Basic (requires -1basica-0)
     list.c             version 2.0 in C
     list.exe           compiled executable version
     list.doc           documentation page
     c:\listfile.rjl    temporary print file for output

AUTHOR
     Joe Loughry

SEE ALSO
     -1check-0(1), -1print-0

Revision 2.2 11/08/91                                         Created 04/06/90
*****************************************************************************/

/*


                       E-1S C R E E N    O P    C O D E S-0F


      [    1 - 2000 ]           Prompts
      [ 2001 - 2999 ]           Stop Code
      3000                      Single Condition
      3001                      Forward Block Condition
      [ 3002 - 3100 ]           Reverse Single Condition
      3101                      Reverse Block Condition
  *   [ 3102 - 4000 ]           Set Date/Time
      [ 4001 - 7999 ]           Draw
      8000                      Window
      [ 8001 - 8400 ]           Save Address
      [ 8401 - 8500 ]           Goto
      [ 8501 - 8550 ]           Combination Goto
      [ 8551 - 8600 ]           User Exit
      [ 8601 - 8607 ]           Deactivate F6/F7
  *   8608                      Deactivate F2 - activate all other F-keys
  *   8609                      Deactivate F1 - activate all other F-keys
  *   8610                      Deactivate F0 - activate all other F-keys
  *   8611                      Deactivate F1 - activate all other F-keys
  *   8612                      Deactivate F2 - activate all other F-keys
  *   8613                      Deactivate F3 - activate all other F-keys
  *   8614                      Deactivate F4 - activate all other F-keys
  *   8615                      Deactivate F5 - activate all other F-keys
  *   8616                      Deactivate F6 - activate all other F-keys
  *   8617                      Deactivate F7 - activate all other F-keys
  *   8618                      Deactivate F8 - activate all other F-keys
      8619                      Deactivate F9 - activate all other F-keys
  *   8620                      Deactivate F10 - activate all other F-keys
  *   [ 8621 - 8630 ]           Deactivate all F-keys
      [ 8631 - 8650 ]           Set Video Mode
  *   [ 8651 - 8660 ]           Set Host Online
      [ 8661 - 8700 ]           Auto Solve
      [ 8701 - 9001 ]           Auto-Save
  *   9002                      Auto-Read
  *   9003                      Auto-Delete
  *   9004                      Auto-Close
  *   9005                      Auto-Read Close
  *   9006                      Auto-Sort
  *   9007                      Auto-CIF
  *   9008                      Auto-Quick Report
  *   9009                      Auto-Advanced Report
  *   [ 9010 - 9100 ]           Auto-Send
      [ 9101 - 9191 ]           Auto Increment
      [ 9192 - 9200 ]           Run A.F. Procedure
      [ 9201 - 9301 ]           Put to Data Model
      [ 9302 - 9401 ]           Get from Data Model
  *   [ 9402 - 9999 ]           ILLEGAL COMMAND
      [ 10000 - 12000 ]         Arithmetic Commands
      [ 12001 - 16999 ]         Auto File Read
      [ 17000 - 18999 ]         Auto File Write
  *   [ 19000 - 25999 ]         Verify
  *   [ 26000 - 26099 ]         Send Keys-Update
  *   [ 26100 - 26199 ]         Send Keys-No Update
  *   [ 26200 - 27099 ]         Copy to Host
  *   [ 27100 - 27199 ]         Copy from Host
  *   [ 27200 - 28999 ]         IC
  *   [ 29000 - 32000 ]         Table Commands
  *   [ 32001 - 32767 ]         ILLEGAL COMMAND

*/

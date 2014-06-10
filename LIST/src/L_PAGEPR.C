
#include "l_extern.h"

/******************************************************************************

WW       WW     AA      RRRRRRR     NNN    NNN   IIIII   NNN    NNN     GGGGGG
WWW     WWW    AAAA     RRR   RR    NNNN   NNN    III    NNNN   NNN    GGG  GGG
WWW  W  WWW   AA  AA    RRR   RRR   NNNNN  NNN    III    NNNNN  NNN   GGG
WWW  W  WWW  AAA  AAA   RRR   RR    NNN NN NNN    III    NNN NN NNN   GGG GGGGG
 WW WWW WW   AAAAAAAA   RRRRRRR     NNN  NNNNN    III    NNN  NNNNN   GGG GGGGG
 WWWWWWWWW   AAA  AAA   RRR  RRR    NNN   NNNN    III    NNN   NNNN    GGG  GGG
  WWWWWWW    AAA  AAA   RRR   RRR   NNN    NNN   IIIII   NNN    NNN     GGGGGG

                     << PRINTER-DEPENDENT CODE AHEAD >>

******************************************************************************/


#ifdef PAGEPRINTER

/*
**  escape sequences and control characters for the IBM 4019 Laser Printer
**  ..........................................in Proprinter emulation mode.
*/

unsigned char lf[]         = { 10, 0 },
     ff[]         = { 12, 0 },
     cr[]         = { 13, 0 },
     esc[]        = { 27, 0 },
     con[]        = { 15, 0 },                  /* condensed print  */
     coff[]       = { 18, 0 },
     ital[]       = { 27, '-', '1', 0 },        /* underlined print */
     itoff[]      = { 27, '-', '0', 0 },
     bold[]       = { 27, 'E', 0 },             /* emphasized print */
     boff[]       = { 27, 'F', 0 },
     normal[]     = { 27, 'I', '0', 18, 0 },
     setup_string[] = { 27, 78, 4, 27, 'I', '0', 27, 85, 1, 0 },

     /* line-drawing characters */

     space[]                          = { ' ', 0 },
     question_mark[]                  = { '?', 0 },
     page_break_char[]                = { 'Ä', 0 },
     extend_forward_block[]           = { 'Ä', 0 },
     open_forward_block[]             = { 'Ú', 0 },
     continue_forward_block[]         = { '³', 0 },
     another_forward_block[]          = { 'Ã', 0 },
     close_forward_block[]            = { 'À', 0 },
     single_arrowhead[]               = { '>', 0 },
     open_reverse_block[]             = { 'È', 0 },
     continue_reverse_block[]         = { 'º', 0 },
     another_reverse_block[]          = { 'Ì', 0 },
     continue_crossed_reverse_block[] = { '×', 0 },
     close_reverse_block[]            = { 'É', 0 },
     double_arrowhead[]               = { '¯', 0 };

#endif


#ifdef PAGEPRINTER

/*****************************************************************************
**                                                                          **
**  BIGNUM: convince an IBM Laser Printer to print nice big page numbers.   **
**                                                                          **
*****************************************************************************/

int print_big_page_number(long pagenumber)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_BIG_PAGE_NUMBER\n");
#endif

     fputc(27, fp_output);
     fputc(91, fp_output);
     fputc(64, fp_output);
     fputc(4,  fp_output);
     fputc(0,  fp_output);        /* Note: you cannot put a null in a C */
     fputc(0,  fp_output);        /*       string -- that's why we have */
     fputc(0,  fp_output);        /*       to do it the hard way . . .  */
     fputc(34, fp_output);
     fputc(2,  fp_output);

     fprintf(fp_output, "%ld\n", pagenumber);

     fputc(27, fp_output);
     fputc(91, fp_output);
     fputc(64, fp_output);
     fputc(4,  fp_output);
     fputc(0,  fp_output);
     fputc(0,  fp_output);
     fputc(0,  fp_output);
     fputc(17, fp_output);
     fputc(1,  fp_output);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit PRINT_BIG_PAGE_NUMBER\n");
#endif

     return(0);
}

#endif


#ifdef PAGEPRINTER

/*****************************************************************************
**                                                                          **
**  SETUP_PRINTER: sends appropriate setup string for IBM Laser Printer.    **
**                                                                          **
*****************************************************************************/

int setup_printer(void)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SETUP_PRINTER\n");
#endif

     fputc(18, fp_output);
     fputc(27, fp_output);
     fputc(78, fp_output);
     fputc(4, fp_output);          /* set automatic perforation skip */
     fputc(27, fp_output);
     fputc(73, fp_output);
     fputc(0, fp_output);          /* set 10cpi DP native font */
     fputc(27, fp_output);
     fputc(85, fp_output);
     fputc(1, fp_output);          /* set unidirectional printing */
     fputc(27, fp_output);
     fputc(65, fp_output);
     fputc(12, fp_output);
     fputc(27, fp_output);
     fputc(50, fp_output);         /* set 6 lines per inch text spacing */

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SETUP_PRINTER\n");
#endif

     return(0);
}

#endif


#ifdef PAGEPRINTER

/*****************************************************************************
**                                                                          **
**  UN_SETUP_PRINTER: turns off undesired global attributes after the run.  **
**                                                                          **
*****************************************************************************/

int un_setup_printer(void)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering UN_SETUP_PRINTER\n");
#endif

     fputc(27, fp_output);      /* turn off automatic perforation skip. */
     fputc(79, fp_output);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit UN_SETUP_PRINTER\n");
#endif

     return(0);
}

#endif


#ifdef PAGEPRINTER

/*****************************************************************************
**                                                                          **
**  RESET_PRINTER: reset IBM Laser Printer to a known state when finished.  **
**                                                                          **
*****************************************************************************/

int reset_printer(void)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering RESET_PRINTER\n");
#endif

     fputc(18, fp_output);
     fputc(27, fp_output);
     fputc(85, fp_output);
     fputc(0, fp_output);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit RESET_PRINTER\n");
#endif

     return(0);
}

#endif















/******************************************************************************

 NNNNNN   NNNNNN      OOOOO      TTTTTTTTTTTT  IIIIII      CCCCC    EEEEEEEEEEE
  NNNNNN   NNNN    OOOOOOOOOOO   TTTTTTTTTTTT   IIII    CCCCCCCCCC   EEEEEEEEEE
  NNNNNNN  NNNN   OOOOO   OOOOO      TTTT       IIII   CCCCC   CCCC  EEEE
  NNNNNNNN NNNN   OOOO     OOOO      TTTT       IIII   CCCC          EEEE    E
  NNNN NNNNNNNN   OOOO     OOOO      TTTT       IIII   CCCC          EEEEEEEEE
  NNNN  NNNNNNN   OOOO     OOOO      TTTT       IIII   CCCC          EEEE    E
  NNNN   NNNNNN   OOOOO   OOOOO      TTTT       IIII   CCCCC   CCCC  EEEE
  NNNN    NNNNN    OOOOOOOOOOO       TTTT       IIII    CCCCCCCCCC   EEEEEEEEEE
 NNNNNN    NNNNN      OOOOO         TTTTTT     IIIIII      CCCCC    EEEEEEEEEEE

                    << END OF PRINTER-DEPENDENT CODE >>

******************************************************************************/



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



#ifdef LASERJET

/*
**  escape sequences and control characters for the HP Laserjet printer.
*/

unsigned char lf[]         = { 10, 0 },
     ff[]         = { 12, 0 },
     cr[]         = { 13, 0 },
     esc[]        = { 27, 0 },
     reset[]      = { 27, 'E', 0 },
     con[]        = { 27, '(', 's', '1', '6', '.', '6', 'H', 0 },
     coff[]       = { 27, '(', 's', '1', '0', 'H', 0 },  /* condensed print  */
     ital[]       = { 27, '(', 's', '1', 'S', 0 },
     itoff[]      = { 27, '(', 's', '0', 'S', 0 },       /* underlined print */
     bold[]       = { 27, '(', 's', '3', 'B', 0 },
     boff[]       = { 27, '(', 's', '0', 'B', 0 },       /* emphasized print */
     normal[]     = { 27, 'E', 0 },
     setup_string[] = { 0 },

     /* line-drawing characters */

     space[]                          = { ' ', 0 },
     question_mark[]                  = { '?', 0 },
     page_break_char[]                = { '=', 0 },
     extend_forward_block[]           = { '-', 0 },
     open_forward_block[]             = { ',', 0 },
     continue_forward_block[]         = { '|', 0 },
     another_forward_block[]          = { '+', 0 },
     close_forward_block[]            = { '`', 0 },
     single_arrowhead[]               = { '>', 0 },
     open_reverse_block[]             = { '`', 0 },
     continue_reverse_block[]         = { '|', 0 },
     another_reverse_block[]          = { '+', 0 },
     continue_crossed_reverse_block[] = { '+', 0 },
     close_reverse_block[]            = { ',', 0 },
     double_arrowhead[]               = { '>', 0 };

#endif


#ifdef LASERJET

/*****************************************************************************
**                                                                          **
**  BIGNUM: convince an HP LaserJet to print nice big page numbers.         **
**                                                                          **
*****************************************************************************/

int print_big_page_number(long pagenumber)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_BIG_PAGE_NUMBER\n");
#endif

     /*
     **  I have to remember to look this up some day.
     */

     fprintf(fp_output, "%s%ld%s\n", bold, pagenumber, boff);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit PRINT_BIG_PAGE_NUMBER\n");
#endif

     return(0);
}

#endif


#ifdef LASERJET

/*****************************************************************************
**                                                                          **
**  SETUP_PRINTER: sends the appropriate setup string for the HP LaserJet.  **
**                                                                          **
*****************************************************************************/

int setup_printer(void)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering SETUP_PRINTER\n");
#endif

     fprintf(fp_output, "%s", normal);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit SETUP_PRINTER\n");
#endif

     return(0);
}

#endif


#ifdef LASERJET

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

     fprintf(fp_output, "%s", normal);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit UN_SETUP_PRINTER\n");
#endif

     return(0);
}

#endif


#ifdef LASERJET

/*****************************************************************************
**                                                                          **
**  RESET_PRINTER: reset the HP LaserJet to a known state when finished.    **
**                                                                          **
*****************************************************************************/

int reset_printer(void)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering RESET_PRINTER\n");
#endif

     fprintf(fp_output, "%s", reset);

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
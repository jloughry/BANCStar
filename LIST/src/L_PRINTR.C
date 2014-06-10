
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



#ifdef GENERIC_PRINTER

/*
**  Replace these codes with the ones for your printer.
*/

unsigned char lf[]         = { 10, 0 },
     ff[]         = { 12, 0 },
     cr[]         = { 13, 0 },
     esc[]        = { 27, 0 },
     reset[]      = { 0 },
     con[]        = { 0 },              /* condensed print  */
     coff[]       = { 0 },
     ital[]       = { 0 },              /* underlined print */
     itoff[]      = { 0 },
     bold[]       = { 0 },              /* empahsized print */
     boff[]       = { 0 },
     normal[]     = { 0 },
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


#ifdef GENERIC_PRINTER

/*****************************************************************************
**                                                                          **
**  BIGNUM: print page numbers on a generic printer.                        **
**                                                                          **
*****************************************************************************/

int print_big_page_number(long pagenumber)
{

#ifdef DIAGNOSTICS
     fprintf(stderr, "entering PRINT_BIG_PAGE_NUMBER\n");
#endif

     fprintf(fp_output, "%ld\n", pagenumber);

#ifdef DIAGNOSTICS
     fprintf(stderr, "exit PRINT_BIG_PAGE_NUMBER\n");
#endif

     return(0);
}

#endif


#ifdef GENERIC_PRINTER

/*****************************************************************************
**                                                                          **
**  SETUP_PRINTER: sends appropriate setup string for a generic printer.    **
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


#ifdef GENERIC_PRINTER

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


#ifdef GENERIC_PRINTER

/*****************************************************************************
**                                                                          **
**  RESET_PRINTER: reset the printer to a known state when finished.        **
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
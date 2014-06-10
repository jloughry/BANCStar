#include "l_extern.h"

/******************************************************************************
**                                                                           **
**  WINDOW: routines for manipulating text windows on the screen.            **
**                                                                           **
******************************************************************************/

int create_window( Window how_big, char *title, short border_color,
                    long background_color, short foreground_color ) {
   static int window_counter;
   int i, j;

#ifdef DIAGNOSTICS
   fprintf(stderr, "entering CREATE_WINDOW\n");
#endif

   _settextwindow(how_big.row1-1, how_big.col1-1, how_big.row2+1, how_big.col2+1);
   _wrapon ( _GWRAPOFF );
   _setbkcolor ( background_color );
   _clearscreen ( _GWINDOW );
   _settextwindow(how_big.row1-1, how_big.col1-1, how_big.row2+2, how_big.col2+4);

   _settextcolor( border_color );
   _settextposition(1,1);
   _outtext("É");
   for (i=1; i<=(how_big.col2-how_big.col1) / 2 - strlen(title) / 2 - 1; i++)
      _outtext("Í");
   if ( strlen ( title ) % 2 == 0 )
      _outtext("Í");
   _outtext(" ");
   _settextcolor( BRIGHT_WHITE );
   _outtext(strupr(title));
   _settextcolor( border_color );
   _outtext(" ");
   for (i=1; i<=(how_big.col2-how_big.col1) / 2 - strlen(title) / 2 - 1; i++)
      _outtext("Í");
   if ( strlen ( title ) % 2 == 0 )
      _outtext("Í");
   _outtext("»");

   for (j=2; j<=how_big.row2-how_big.row1+2; j++) {
      _settextposition(j, 1);
      _outtext("º");
      _settextposition(j, how_big.col2-how_big.col1+3);
      _outtext("º");
   }

   _settextposition(how_big.row2-how_big.row1+3, 1);
   _outtext("È");
   for (i=1; i<=how_big.col2-how_big.col1+1; i++)
      _outtext("Í");
   _outtext("¼");

   /*
   **  Draw shadows (shadows are necessary to avoid end-of-window wrapping).
   */

   _setbkcolor ( BLACK );
   for (i=2; i<=how_big.row2+2; i++) {
      _settextposition ( i, how_big.col2-how_big.col1+4 );
      _outtext ( "  " );
   }
   for (j=2; j<=how_big.col2-how_big.col1+3; j++) {
      _settextposition ( how_big.row2+2, j );
      _outtext ( " " );
   }

   _setbkcolor ( background_color );
   _settextcolor ( foreground_color );

   _settextwindow(how_big.row1, how_big.col1, how_big.row2, how_big.col2);
   _settextposition(1,1);

   return(0);

#ifdef DIAGNOSTICS
   fprintf(stderr, "exit CREATE_WINDOW\n");
#endif

   return(window_counter++);
}

int destroy_windows( void ) {

#ifdef DIAGNOSTICS
   fprintf(stderr, "entering DESTROY_WINDOWS\n");
#endif

   _settextwindow ( 1, 1, 24, 80 );
   _settextposition ( 24, 1 );

#ifdef DIAGNOSTICS
   fprintf(stderr, "exit DESTROY_WINDOWS\n");
#endif

   return ( 0 ) ;
}


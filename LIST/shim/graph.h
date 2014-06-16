struct rccoord {
    short row;
    short col;
};

void setup_console(void);

void _outtext(unsigned char* text);
struct rccoord _gettextposition(void);
struct rccoord _settextposition(short row, short column);
short _settextcolor(short pixel);
long _setbkcolor(long color);
void _settextwindow(short r1, short c1, short r2, short c2);
short _gettextcolor(void);
long _getbkcolor(void);
void _clearscreen(short mode);
#define _GCLEARSCREEN 0
#define _GVIEWPORT 1
#define _GWINDOW 2

short _wrapon(short option);
#define _GWRAPOFF 0
#define _GWRAPON 1

#define _BLACK         0
#define _BLUE          1
#define _GREEN         2
#define _CYAN          3
#define _RED           4
#define _MAGENTA       5
#define _BROWN         6
#define _WHITE         7
#define _GRAY          8
#define _LIGHTBLUE     9
#define _LIGHTGREEN   10
#define _LIGHTCYAN    11
#define _LIGHTRED     12
#define _LIGHTMAGENTA 13
#define _LIGHTYELLOW  14
#define _BRIGHTWHITE  15
#define _YELLOW       14

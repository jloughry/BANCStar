#include "graph.h"
#include "windows.h"
#include "wincon.h"

HANDLE STDCONSOLE;

void setup_console(void) {
    STDCONSOLE = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

void _outtext(unsigned char* text) {
    DWORD written;
    WriteConsole(STDCONSOLE, text, strlen(text), &written, NULL);
}

COORD rccoord_to_COORD(struct rccoord dospos) {
    COORD winpos;
    winpos.X = dospos.col;
    winpos.Y = dospos.row;
    return winpos;
}

struct rccoord COORD_to_rccoord(COORD winpos) {
    struct rccoord dospos;
    dospos.col = winpos.X;
    dospos.row = winpos.Y;
    return dospos;
}

WORD color_to_WORD(short dosclr, int is_background) {
    WORD winclr;
    long red   = (is_background ? BACKGROUND_RED : FOREGROUND_RED);
    long green = (is_background ? BACKGROUND_GREEN : FOREGROUND_GREEN);
    long blue  = (is_background ? BACKGROUND_BLUE : FOREGROUND_BLUE);
    long intensity = (is_background ? BACKGROUND_INTENSITY : FOREGROUND_INTENSITY);
    switch(dosclr) {
        case _BLACK:
            return 0;
        case _BLUE:
            return blue;
        case _GREEN:
            return green;
        case _CYAN:
            return green|blue;
        case _RED:
            return red;
        case _MAGENTA:
            return red|blue;
        case _BROWN:
            return red|green;
        case _WHITE:
            return red|green|blue;
        case _GRAY:
            return 0|intensity;
        case _LIGHTBLUE:
            return blue|intensity;
        case _LIGHTGREEN:
            return green|intensity;
        case _LIGHTCYAN:
            return green|blue|intensity;
        case _LIGHTRED:
            return red|intensity;
        case _LIGHTMAGENTA:
            return red|blue|intensity;
        case _LIGHTYELLOW:
            return red|green|intensity;
        case _BRIGHTWHITE:
            return red|green|blue|intensity;
        default:
            return red|green|blue;
    }
}

short WORD_to_color(WORD winclr, int is_background) {
    short dosclr;
    long red   = (is_background ? BACKGROUND_RED : FOREGROUND_RED);
    long green = (is_background ? BACKGROUND_GREEN : FOREGROUND_GREEN);
    long blue  = (is_background ? BACKGROUND_BLUE : FOREGROUND_BLUE);
    long intensity = (is_background ? BACKGROUND_INTENSITY : FOREGROUND_INTENSITY);
    if(winclr & red) {
        if(winclr & green) {
            dosclr = (winclr & blue) ? _WHITE : _BROWN;
        } else {
            dosclr = (winclr & blue) ? _MAGENTA : _RED;
        }
    } else {
        if(winclr & green) {
            dosclr = (winclr & blue) ? _CYAN : _GREEN;
        } else {
            dosclr = (winclr & blue) ? _BLUE : _BLACK;
        }
    }
    if(winclr & intensity) {
        dosclr += 8;
    }
    return dosclr;
}

struct rccoord _gettextposition(void) {
    CONSOLE_SCREEN_BUFFER_INFO record;
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    return COORD_to_rccoord(record.dwCursorPosition);
}
struct rccoord _settextposition(short row, short column) {
    struct rccoord prev = _gettextposition();
    COORD newpos;
    newpos.X = column;
    newpos.Y = row;
    SetConsoleCursorPosition(STDCONSOLE, newpos);
    return prev;
}
short _settextcolor(short pixel) {
    short prev = _gettextcolor();
    CONSOLE_SCREEN_BUFFER_INFO record;
    WORD attrib;
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    attrib = record.wAttributes;
    attrib -= (attrib & color_to_WORD(_BRIGHTWHITE, 0));
    attrib |= color_to_WORD(pixel, 0);
    SetConsoleTextAttribute(STDCONSOLE, attrib);
    return prev;
}
long _setbkcolor(long pixel) {
    long prev = _getbkcolor();
    CONSOLE_SCREEN_BUFFER_INFO record;
    WORD attrib;
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    attrib = record.wAttributes;
    attrib -= (attrib & color_to_WORD(_BRIGHTWHITE, 1));
    attrib |= color_to_WORD(pixel, 1);
    SetConsoleTextAttribute(STDCONSOLE, attrib);
    return prev;
}
void _settextwindow(short r1, short c1, short r2, short c2) {
    // not implemented
}
short _gettextcolor(void) {
    CONSOLE_SCREEN_BUFFER_INFO record;
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    return WORD_to_color(record.wAttributes, 0);
}
long _getbkcolor(void) {
    CONSOLE_SCREEN_BUFFER_INFO record;
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    return WORD_to_color(record.wAttributes, 1);
}

void clear_region(struct rccoord start, int num_cells) {
    struct rccoord oldpos = _gettextposition();
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO record;

    GetConsoleScreenBufferInfo(STDCONSOLE, &record);

    // fill with blanks
    FillConsoleOutputCharacter(STDCONSOLE, ' ', num_cells, rccoord_to_COORD(start), &cCharsWritten);

    // reset attributes
    FillConsoleOutputAttribute(STDCONSOLE, record.wAttributes, num_cells, rccoord_to_COORD(start), &cCharsWritten);

    _settextposition(oldpos.row, oldpos.col);
    return;
}
void clear_whole_screen(void) {
    struct rccoord start = { 0, 0 };
    DWORD dwConSize;
    CONSOLE_SCREEN_BUFFER_INFO record;

    // get buffer size
    GetConsoleScreenBufferInfo(STDCONSOLE, &record);
    dwConSize = record.dwSize.X * record.dwSize.Y;

    clear_region(start, dwConSize);

    // home cursor
    SetConsoleCursorPosition(STDCONSOLE, rccoord_to_COORD(start));
    return;
}

short _wrapon(short option) {
    // not implemented
    return _GWRAPON;
}

void _clearscreen(short mode) {
    switch(mode) {
        case _GCLEARSCREEN:
            clear_whole_screen();
        default:
            clear_whole_screen();
    }
}

#include "ErrorAlert.h"
#include <curses.h>

void ErrorAlert::alertMsg()
{
	WINDOW* errorWin = newwin(2, 50, 10, COLS / 3);
	wprintw(errorWin, "First you have to choose item to edition\n \t(go to -> EditionMode)");
	wrefresh(errorWin);
	delwin(errorWin);
}

void ErrorAlert::fileAlert()
{
	WINDOW* errorWin = newwin(2, 40, 10, COLS / 3);
	wprintw(errorWin, "Wrong file name, try one more time");
	wrefresh(errorWin);
	delwin(errorWin);
}

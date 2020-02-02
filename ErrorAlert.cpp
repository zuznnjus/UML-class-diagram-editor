#include "ErrorAlert.h"
#include <curses.h>

WINDOW* ErrorAlert::alertMsg(WINDOW* window, std::string msg)
{
	WINDOW* errorWin = derwin(window,2, 50, 10, COLS / 3);
	wprintw(errorWin, msg.c_str());
	wrefresh(errorWin);
	//werase(errorWin);
	//delwin(errorWin);
	return errorWin;
}

/*
  This file is part of bcalc.
  bcalc is a multi-ui RPN calculator
  Copyright (C) 2007 - 2015  Avraham Lembke
  
  bcalc is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  bcalc is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/* a curses based ui */
#include "bcalc.h"

#ifdef UI_CURSES

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <curses.h>
#include "curses-ui.h"

WINDOW *printwin; /* used as a "paper trail" */
WINDOW *inputwin; /* the command line */

void curses_ui_init( void )
{
    initscr();
    printwin = newwin( 22, 80, 0, 0 );
    inputwin = newwin( 2, 80, 22, 0 );
    keypad( inputwin, true );
    scrollok( printwin, true );
    echo();
    mvwhline( inputwin, 0, 0, ACS_HLINE, 80 );
    curses_ui_redraw();
    print_banner();
}

void curses_ui_stop( void )
{
    endwin();
}

void curses_ui_input( char *buf, size_t bufsize )
{
    int x;

    mvwgetnstr( inputwin, 1, 0, buf, bufsize );
    curses_ui_print( "%s", buf );
    
    for( x = 0; x <= 81; x++ )
        mvwprintw( inputwin, 1, x, " " );

    curses_ui_redraw();
}

void curses_ui_print( char *fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    vwprintw( printwin, fmt, ap );
    va_end( ap );
    wprintw( printwin, "\n" );
    curses_ui_redraw();
}

void curses_ui_redraw( void )
{
    /* Note: implementation specific function */
    wrefresh( printwin );
    wrefresh( inputwin );
}

#endif

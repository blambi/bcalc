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

/* wraps the ui code so other code dont have to look like this all over the
   place =) */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "bcalc.h"
#include "text-ui.h"
#include "curses-ui.h"
#include "readline-ui.h"
#include "gtk2-ui.h"

/* function pointers */
void ( *ui_init_f[] )( void ) = {
    text_ui_init,
#ifdef UI_CURSES
    curses_ui_init,
#endif
#ifdef UI_READLINE
    readline_ui_init,
#endif
#ifdef UI_GTK2
    gtk2_ui_init
#endif
};

void ( *ui_stop_f[] )( void ) = {
    text_ui_stop,
#ifdef UI_CURSES
    curses_ui_stop,
#endif
#ifdef UI_READLINE
    readline_ui_stop,
#endif
#ifdef UI_GTK2
    gtk2_ui_stop
#endif
};

void ( *ui_print_f[] )( char *fmt, ... ) = {
    text_ui_print,
#ifdef UI_CURSES
    curses_ui_print,
#endif
#ifdef UI_READLINE
    readline_ui_print,
#endif
#ifdef UI_GTK2
    gtk2_ui_print
#endif
};

void ( *ui_input_f[] )( char *buf, size_t bufsize ) = {
    text_ui_input,
#ifdef UI_CURSES
    curses_ui_input,
#endif
#ifdef UI_READLINE
    readline_ui_input,
#endif
#ifdef UI_GTK2
    gtk2_ui_input
#endif
};

void ui_init( void )
{
    ui_init_f[current_ui]();
}

void ui_stop( void )
{
    ui_stop_f[current_ui]();
}

void ui_print( char *fmt, ... )
{
    /* FIXME: IF posible, make this behave like the other wraper functions */
    va_list ap;
    char buf[200]; /* i hope its long enought */

    va_start( ap, fmt );
    vsnprintf( buf, sizeof( buf ), fmt, ap );
    va_end( ap );

    ui_print_f[current_ui]( buf );
}

void ui_input( char *buf, size_t bufsize )
{
    ui_input_f[current_ui]( buf, bufsize );
}

int ui_name_to_int( char *buf )
{
    /* Note: look up the in for a ui, if there is no such ui return -1 */
    if( strcmp( "text", buf ) == 0 )
        return 0; /* text-ui */
#ifdef UI_CURSES
    else if( strcmp( "curses", buf ) == 0 )
        return 1; /* curses-ui */
#endif
#ifdef UI_READLINE
    else if( strcmp( "readline", buf ) == 0 )
        return 2; /* readline-ui */
#endif
#ifdef UI_GTK2
    else if( strcmp( "gtk2", buf ) == 0 )
        return 3; /* GTK gui */
#endif
    else
        return -1;
}

bool ui_is_self_looping()
{
    /* Note: returns true if the current ui is self looping
       (in other words, it handles its own event loop) */

    switch( current_ui )
    {
    case 0: /* text-ui */
        return false;
        break;
#ifdef UI_CURSES
    case 1:
        return false;
        break;
#endif
#ifdef UI_READLINE
    case 2:
        return false;
        break;
#endif
#ifdef UI_GTK2
    case 3:
        return true;
        break;
#endif
    default:
        return false;
        break;
    }
}

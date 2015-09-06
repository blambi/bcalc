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

/* a ui using the readline library */
#include "bcalc.h"

#ifdef UI_READLINE

#include <stdio.h>
#include <stdarg.h>
#include <readline/readline.h>
#include <string.h>
#include <stdlib.h>
#include <readline/history.h>
#include "bcalc.h"
#include "readline-ui.h"

static char *readline_read = (char *)NULL;

void readline_ui_init( void )
{}

void readline_ui_stop( void )
{}

void readline_ui_input( char *buf, size_t bufsize )
{
    /* Note: this function is more of a translator between readline and 
             bcalc */
    
    strncpy( buf, readline_ui_gets(), bufsize );
}

void readline_ui_print( char *fmt, ... )
{
    /* Note: stolen from text-ui */
    va_list ap;
    va_start( ap, fmt );
    vprintf( fmt, ap );
    va_end( ap );
    printf( "\n" );
}

char *readline_ui_gets()
{
    /* this function was created since input is quite diffrent with readline */
    if( readline_read )
    {
        free( readline_read );
        readline_read = (char *)NULL;
    }

    /* read the line */
    readline_read = readline( "" );
    
    /* if we got only a EOF return a 'q' so the program shutdown */
    if( readline_read == NULL )
        return "q";
    
    /* history */
    if( strcmp( "", readline_read ) != 0 )
        add_history (readline_read);

    return readline_read;
}
#endif

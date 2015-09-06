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

/* This ui is just a simple text based one, could be used as a template for
   new ui's */

#include <stdio.h>
#include <stdarg.h>
#include "bcalc.h"
#include "text-ui.h"

void text_ui_init( void )
{}
void text_ui_stop( void )
{}

void text_ui_input( char *buf, size_t bufsize )
{
    /* Note: gui just returns strings, what the string is etc should not
       be handled here */
    int count;
    int inp;
    
    count = 0;
    inp = -1;
    
    while( count < bufsize && ( inp = getchar() ) != '\n' )
    {
        buf[count] = inp;
        count++;
    }
    buf[count] = '\0';
}

void text_ui_print( char *fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );
    vprintf( fmt, ap );
    va_end( ap );
    printf( "\n" );
}

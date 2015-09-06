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


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "bcalc.h"
#include "wraper-ui.h"
#include "drmath.h"

int current_ui;
int running;

int main( int argc, char **argv )
{
    int opt_index;
    int choice;
    int result;
    int selected_ui;

    static struct option long_options[] =
    {
        { "help", no_argument, 0, 'h' },
        { "ui", required_argument, 0, 'u' },
        { 0, 0, 0, 0 }
    };

    /* set defaults */
    current_ui = DEFAULT_UI;
    selected_ui = DEFAULT_UI;

    /* print the banner */
    print_banner();

    /* Argument parser */
    while( 1 )
    {
        opt_index = 0;
        choice = getopt_long( argc, argv, "hu:", long_options, &opt_index );
        
        if( choice == -1 ) /* no arguments left */
            break;
        
        switch( choice )
        {
        case 'h':
            ui_print( "Usage: [OPTIONS]\n" );
            ui_print( "Option\t\tMeaning" );
            ui_print( " -h, --help\tShow this message" );
            ui_print( " -u UI, --ui UI\tuse interface UI" );
            ui_print( "\t\tavailable interfaces are: text" );
#ifdef UI_CURSES
            ui_print( "\t\tcurses" );
#endif
#ifdef UI_READLINE
            ui_print( "\t\treadline" );
#endif
#ifdef UI_GTK2
            ui_print( "\t\tgtk2" );
#endif
            ui_print( "" );
            exit( EXIT_SUCCESS );
            break;
        case 'u':
            selected_ui = ui_name_to_int( optarg );
            if( selected_ui == -1 )
            {
                ui_print( "Error: no interface called '%s'", optarg );
                exit( EXIT_FAILURE );
            }
            break;
        default:
            exit( EXIT_FAILURE );
            break;
        }   
    }
    
    dr_init();
    current_ui = selected_ui; /* switch to the selected one */
    result = ui_loop();
    exit( result );
}

int ui_loop( void )
{
    char buff[200];
    running = true;
    /* start of ui calls */
    ui_init();
    
    /* beg for input */
    if( ! ui_is_self_looping() ) /* FIXME: not sure if this is the best way */
    {
        while( running )
        {
            ui_input( buff, sizeof( buff ) );
            dr_parse_input( buff, sizeof( buff ) );
        }
    }

    /* stop of ui calls */
    ui_stop();
    return 0;
}

void print_banner( void )
{
    /* Note: prints the banner */
    ui_print( "bcalc v%s", VERSION );
    ui_print( "Copyright (C) 2007 - 2015  Avraham Lembke." );
    ui_print( "This is free software with ABSOLUTELY NO WARRANTY.\n" );
}

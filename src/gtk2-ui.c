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

/* a gui based on GTK 2.x */
#include "bcalc.h"

#ifdef UI_GTK2

#include <stdio.h>
#include <stdarg.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include "bcalc.h"
#include "gtk2-ui.h"
#include "drmath.h"

GtkWidget *window;
GtkWidget *input_entry;
GtkWidget *hbox;
GtkWidget *out_view;
GtkTextBuffer *out_buffer;
GtkWidget *scrolled_window;

/* The local prototypes */
void gtk2_ui_destroy( GtkWidget * widget, gpointer data );
void gtk2_ui_enter( GtkWidget *widget, gpointer *entry );

void gtk2_ui_init( void )
{
    /* kick start it.. or something */
    gtk_init( NULL, NULL ); /* normaly &argc and &argv is passed */

    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    
    /* setup the window */
    gtk_window_set_title( GTK_WINDOW( window ), "bcalc v" VERSION );
    gtk_window_set_default_size( GTK_WINDOW( window ), 340, 270 );
    gtk_container_set_border_width( GTK_CONTAINER( window ), 5 );

    /* setup the window widgets */
    hbox = gtk_vbox_new( FALSE, 0 );
    input_entry = gtk_entry_new();
    out_view = gtk_text_view_new();
    gtk_text_view_set_editable( GTK_TEXT_VIEW( out_view ), FALSE );
    out_buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW( out_view ) );

    scrolled_window = gtk_scrolled_window_new( NULL, NULL );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolled_window ),
                                    GTK_POLICY_AUTOMATIC, 
                                    GTK_POLICY_ALWAYS );
    gtk_container_add( GTK_CONTAINER( scrolled_window ), out_view );

    gtk_box_pack_start( GTK_BOX( hbox ), scrolled_window, true, true, 3 );
    gtk_box_pack_end( GTK_BOX( hbox ), input_entry, false, false, 0 );
    gtk_container_add( GTK_CONTAINER( window ), hbox );
    gtk_widget_grab_focus( input_entry );

    /* events and there connections */
    g_signal_connect( G_OBJECT( window ), "destroy",
                      G_CALLBACK( gtk2_ui_destroy ), NULL );

    g_signal_connect( G_OBJECT( input_entry ), "activate",
                      G_CALLBACK( gtk2_ui_enter ),
                      (gpointer) input_entry );

    /* show them, yes show them ALL! */
    gtk_widget_show_all( window );

    /* GTK got a loop called gtk_main() so we need not to implement our own */
    print_banner();
    gtk_main();
}

void gtk2_ui_stop( void )
{
    if( running )
    {
        running = false;
        gtk_main_quit();
    }
}

void gtk2_ui_input( char *buf, size_t bufsize )
{
    /* NOTE: this function isn't really used in this ui... */
}

void gtk2_ui_print( char *fmt, ... )
{
    /* Note: not that cute function but it works.. maybe FIXME! */
    va_list ap;
    char buf[200]; /* i hope its long enought */
    GtkTextIter end_iter;

    va_start( ap, fmt );
    vsnprintf( buf, sizeof( buf ), fmt, ap );
    va_end( ap );
    sprintf( buf, "%s\n", buf );

    /* place cursor at the end */
    gtk_text_buffer_get_end_iter( out_buffer, &end_iter );
    gtk_text_buffer_place_cursor( out_buffer, &end_iter );

    /* print */
    gtk_text_buffer_insert( out_buffer, &end_iter, buf, -1 );

    /* do some magic markers and then scroll to the bottom */
    gtk_text_buffer_get_end_iter( out_buffer, &end_iter );
    gtk_text_buffer_create_mark( out_buffer, "end", &end_iter, false );
    gtk_text_view_scroll_mark_onscreen( GTK_TEXT_VIEW( out_view ), 
                                        gtk_text_buffer_get_mark( out_buffer,
                                                                  "end"  ));
    gtk_text_buffer_delete_mark_by_name( out_buffer, "end" ); /* remove mark */    
}

/* local functions */

void gtk2_ui_destroy( GtkWidget *widget, gpointer data )
{
    /* Note: This function destroys all hope... no it closes the window, 
       and terminates the gtk loop */
    
    running = false;
    gtk_main_quit();
}

void gtk2_ui_enter( GtkWidget *widget, gpointer *entry )
{
    /* Note: this is the "real" gtk2_ui_input function */
    
    gchar *buff;
    buff = gtk_entry_get_text( GTK_ENTRY( entry ) );
#ifdef DEBUG
    g_print( "debug,input: '%s'\n", buff );
#endif
    gtk2_ui_print( buff );
    dr_parse_input( buff, strlen( buff ) +1 );
    gtk_entry_set_text( GTK_ENTRY( entry ), "" );

    /* check if we are still running */
    if( ! running )
        gtk_main_quit();
}

#endif

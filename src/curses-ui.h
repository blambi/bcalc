#ifndef CURSES_UI_H
#define CURSES_UI_H

void curses_ui_init( void );
void curses_ui_stop( void );
void curses_ui_input( char *buf, size_t bufsize );
void curses_ui_print( char *fmt, ... );

void curses_ui_redraw( void );

#endif /* CURSES_UI_H */



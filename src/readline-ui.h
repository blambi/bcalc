#ifndef READLINE_UI_H
#define READLINE_UI_H

void readline_ui_init( void );
void readline_ui_stop( void );
void readline_ui_input( char *buf, size_t bufsize );
void readline_ui_print( char *fmt, ... );

char *readline_ui_gets();

#endif /* READLINE_UI_H */



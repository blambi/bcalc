#ifndef GTK2_UI_H
#define GTK2_UI_H

void gtk2_ui_init( void );
void gtk2_ui_stop( void );
void gtk2_ui_input( char *buf, size_t bufsize );
void gtk2_ui_print( char *fmt, ... );

#endif /* GTK2_UI_H */



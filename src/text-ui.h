#ifndef TEXT_UI_H
#define TEXT_UI_H

void text_ui_init( void );
void text_ui_stop( void );
void text_ui_input( char *buf, size_t bufsize );
void text_ui_print( char *fmt, ... );

#endif /* TEXT_UI_H */

#ifndef WRAPER_UI_H
#define WRAPER_UI_H

void ui_init( void );
void ui_stop( void );
void ui_print( char *fmt, ... );
void ui_input( char *buf, size_t bufsize );

/* other helpers */
int ui_name_to_int( char *buf );
bool ui_is_self_looping();

#endif /* WRAPER_UI_H */

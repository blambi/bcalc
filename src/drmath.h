#ifndef __DRMATH_H
#define __DRMATH_H

#include <stdbool.h>

/* functions */
void dr_init( void );
void dr_parse_input( char *input, size_t input_size );
int dr_find_op( char *string );
int test_double( double subj );
bool dr_do( int op_code );
bool dr_do_math( int op_code );
bool dr_do_math_one( int op_code );
void dr_do_on_all( int op_code );
void dr_print_result( double value );

/* The list/stack */
typedef struct Stack_node Stack_node;
struct Stack_node
{
    double value;
    Stack_node *next;
};

void add_element( double value );
void del_element( void );
void print_stack( void );
void clear_stack( void );


#endif /* __DRMATH_H */

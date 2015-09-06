/*
  This file is part of bcalc.
  bcalc is a multi-ui RPN calculator
  Copyright (C) 2007-2015  Avraham Lembke
  
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


/* this is the calculator part really, with some extras */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "bcalc.h"
#include "wraper-ui.h"
#include "drmath.h"

/* OP-codes */
enum { NOOP = 0, ADDITION = 1, SUBTRACTION, MULTIPLICATION, DIVISION, POWERS,
       FACT, SQRT, MOD, QUIT, PRINT_TOP, PRINT_STACK, CLEAR_STACK,
       DELETE_ELEMENT, OP_ON_ALL, INVERT, PRINT_HELP };

Stack_node *stack_start;
Stack_node *stack_stop;
int last_op;
uint stack_size;

void dr_init( void )
{
    /* Note: the doctor needs to initalize some stuff */
    stack_start = ( Stack_node * )malloc( sizeof( Stack_node ) );
    stack_stop = ( Stack_node * )malloc( sizeof( Stack_node ) );
    stack_start->next = stack_stop;
    stack_stop->next = NULL;
    stack_size = 0;

    last_op = NOOP;
}

void dr_parse_input( char *input, size_t input_size )
{
    /* Note: This function is used to determine what the input was,
       And what to do with it */

    int x, len, op_code;
    float new_num;
    const char delimiter[] = " "; /* for strtok */
    char *token;

    /* remove eventual ',' and make them '.' */
    len = strlen( input );
    for( x = 0; x <= len; x++ )
        if( input[x] == ',' )
            input[x] = '.';
    
    /* multi op / number, ig: line spliter */
    token = strtok( input, delimiter );

    while( token != NULL )
    {      
        /* real parser part */
        op_code = dr_find_op( token );  

#ifdef DEBUG
        ui_print( "op_code: %d", op_code );
#endif
    
        if( op_code != false ) /* it was a command */
        {   /* MAYBE: add flag if a op is do_math related and skip this then */
            dr_do( op_code );
            if( op_code != OP_ON_ALL )
                last_op = op_code;
        }
        else if( len > 0 )
        {
            if( sscanf( token, "%f", &new_num ) && len > 0 ) /* find numbers */
                add_element( new_num );
            else
                ui_print( "Error: unknown command '%s'", token );
        }
    /* end of the parser part */
        token = strtok( NULL, delimiter );
    }

#ifdef DEBUG
    ui_print( "'%s' of length %d", input, strlen( input ));
#endif
}

int dr_find_op( char *string )
{
    /* Note: returns the op-code if there is one, else we return false */
    /* FIXME: might be nicer to have some kind of array that we check against
       insteed */
    if( strncmp( "+", string, sizeof( string )) == 0 )
        return ADDITION;
    else if( strncmp( "-", string, sizeof( string )) == 0 )
        return SUBTRACTION;
    else if( strncmp( "*", string, sizeof( string )) == 0 )
        return MULTIPLICATION;
    else if( strncmp( "/", string, sizeof( string )) == 0 )
        return DIVISION;
    else if( strncmp( "^", string, sizeof( string )) == 0 )
        return POWERS;
    else if( strncmp( "!", string, sizeof( string )) == 0 )
        return FACT;
    else if( strncmp( "sqrt", string, sizeof( string )) == 0 )
        return SQRT;
    else if( strncmp( "%", string, sizeof( string )) == 0 )
        return MOD;
    else if( strncmp( "i", string, sizeof( string )) == 0 )
        return INVERT; /* Almost math anyway */
    /* non-math */
    else if( strncmp( "q", string, sizeof( string )) == 0 )
        return QUIT;
    else if( strncmp( "p", string, sizeof( string )) == 0 )
        return PRINT_TOP;
    else if( strncmp( "s", string, sizeof( string )) == 0 )
        return PRINT_STACK;
    else if( strncmp( "c", string, sizeof( string )) == 0 )
        return CLEAR_STACK;
    else if( strncmp( "d", string, sizeof( string )) == 0 )
        return DELETE_ELEMENT;
    else if( strncmp( "a", string, sizeof( string )) == 0 )
        return OP_ON_ALL;
    else if( strncmp( "h", string, sizeof( string )) == 0 )
        return PRINT_HELP;
    else
        return false;
}

int test_double( double subj )
{
    /* a simple test to see if subj is a double(true) or a int(false) */
    double test_d;
    int test_i;
 
    test_i = subj;
    test_d = test_i;
    if( test_d != subj )
        return true;
    else
        return false;
}

bool dr_do( int op_code )
{
    /* Note: abstraction */
    switch( op_code )
    {
    case ADDITION:
        return dr_do_math( ADDITION );
        break;
    case SUBTRACTION:
        return dr_do_math( SUBTRACTION );
        break;
    case MULTIPLICATION:
        return dr_do_math( MULTIPLICATION );
        break;
    case DIVISION:
        return dr_do_math( DIVISION );
        break;
    case POWERS:
        return dr_do_math( POWERS );
        break;
    case FACT:
        return dr_do_math_one( FACT );
        break;
    case SQRT:
        return dr_do_math_one( SQRT );
        break;
    case MOD:
        return dr_do_math( MOD );
        break;
    case INVERT:
        if( stack_start->next->next != NULL )
        {
            add_element( -1.0 );
            return dr_do_math( MULTIPLICATION );
        }
        else
            ui_print( "no value to invert" );
        break;
        /* non-math */
    case QUIT:
        ui_print( "good bye" );
        running = 0;
        break;
    case PRINT_TOP:
        if( stack_start->next->next != NULL )
            dr_print_result( stack_start->next->value );
        else
            ui_print( "stack is empty" );
        break;
    case PRINT_STACK:
        print_stack();
        break;
    case CLEAR_STACK:
        clear_stack();
        break;
    case DELETE_ELEMENT:
        del_element();
        break;
    case OP_ON_ALL:
        if( stack_start->next->next != NULL )
            dr_do_on_all( last_op );
        else
            ui_print( "stack is empty" );
        break;
    case PRINT_HELP:
        ui_print( "Online help:" );
        ui_print( "  Operands:" );
        ui_print( "  +,-   normal");
        ui_print( "  *,/   -\"\"-");
        ui_print( "  !     faculty (requires only one element)");
        ui_print( "  ^     powers");
        ui_print( "  sqrt  square root");
        ui_print( "" );
        ui_print( "  Commands:");
        ui_print( "  p   print top value");
        ui_print( "  s   print stack");
        ui_print( "  d   delete top value");
        ui_print( "  q   quit");
        ui_print( "  c   clear stack");
        ui_print( "  a   repeats last operation on all the stack");
        ui_print( "  i   invert multiplies top value with -1");
        ui_print( "  h   prints this text");
        break;
    default:
        ui_print( "Warning: undefined op-code %d", op_code );
        break;
    }

    return true;
}

bool dr_do_math( int op_code )
{
    /* Note: this is really the calculator part */
    double value1, value2, result;
    int ok;

    if( stack_start->next->next != NULL &&
        stack_start->next->next->next != NULL )
    {
        value1 = stack_start->next->value;
        value2 = stack_start->next->next->value;

        switch( op_code )
        {
        case ADDITION:
            result = value2 + value1;
            ok = true;
            break;
        case SUBTRACTION:
            result = value2 - value1;
            ok = true;
            break;
        case MULTIPLICATION:
            result = value2 * value1;
            ok = true;
            break;
        case DIVISION:
            result = value2 / value1;
            ok = true;
            break;
        case POWERS:
            result = pow( value2, value1 );
            ok = true;
            break;
        case MOD: /* FIXME: not as exact as for example bc */
            result = fmod( value2, value1 );
            ok = true;
            break;
        default:
            ui_print( "warning: dr_do_math (drmath.c) got unknown",
                      " op code '%d'", op_code );
            ok = false;
            result = 0; /* to stop some nagging during compile */
            break;
        }
        
        /* clean up */
        if( ok )
        {
            del_element();
            del_element();
            add_element( result );

            dr_print_result( result );
            return true;
        }
    }
    else
        ui_print( "stack is empty/not big enought" );
    return false;
}

bool dr_do_math_one( int op_code )
{
    /* Note: similar to dr_do_math but just involves one stack element */
    int x, ok;
    double value, result;

    ok = false;

    if( stack_start->next->next != NULL )
    {
        value = stack_start->next->value;

        switch( op_code )
        {
        case FACT:
            result = 1;

            for( x = value; x >= 1; x-- )
            {
                result *= x;
            }
            ok = true;
            break;
        case SQRT:
            result = sqrt( value );
            ok = true;
            break;
        default:
            ui_print( "warning: dr_do_math_one (drmath.c) got unhandled op",
                      " code %d", op_code );
            result = 0; 
            ok = false;
            break;
        }

        if( ok )
        {
            del_element();
            add_element( result );
        
            dr_print_result( result );
            return true;
        }
    }
    else
        ui_print( "stack is empty/not big enought" );
    return false;
}

void dr_do_on_all( int op_code )
{
    /* Note: runs an op over all the stack */
    uint last_stack_size;
    double top_value;
    
    last_stack_size = stack_size;
    top_value = stack_start->next->value;

    while( dr_do( op_code ) )
    {
        if( stack_start->next->next == NULL )
            break;

        /* try to catch run-away inf loops */
        if( last_stack_size == stack_size )
            break;
    }
}

void dr_print_result( double value )
{
    /* Note: prints the result of a operand */
    int stripper;

    if( test_double( value ) )
        ui_print( "-> %f", value );
    else
    {
        stripper = value;
        ui_print( "-> %i", stripper );
    }
}

/* stack stuff */
void add_element( double value )
{
    /* Note: adds a new element to the stack */
    Stack_node *node;

    node = ( Stack_node * )malloc( sizeof( Stack_node ) );
    node->next = stack_start->next;
    node->value = value;
    stack_start->next = node;
    stack_size++;
#ifdef DEBUG
    ui_print( "deb: next->%i,value->%f", &node->next, node->value );
#endif
}

void del_element( void )
{
    /* Note: deletes the top element in the stack */
    Stack_node *node;
    
    if( stack_start->next->next == NULL )
        ui_print( "stack is empty" );
    else
    {
        node = stack_start->next;
        stack_start->next = node->next;
        free( node );
        stack_size--;
    }
}

void print_stack( void )
{
    /* Note: prints all elements in the stack */
    Stack_node *node;
    int x, stripper;
    
    x = 0;
    node = stack_start->next;
    while( node->next != NULL )
    {
        if( test_double( node->value ) )
            ui_print( "%i: %f", x, node->value );
        else
        {
            stripper = node->value;
            ui_print( "%i: %i", x, stripper );
        }
        x++;
        node = node->next;
    }

    if( stack_start->next->next == NULL )
        ui_print( "stack is empty" );
}

void clear_stack( void )
{
    /* Note: clears the stack */
    int done;
    
    done = false;

    while( !done )
    {
        if( stack_start->next->next == NULL )
            done = true;
        else
            del_element();
    }
    stack_size = 0;
    ui_print( "stack cleared" );
}

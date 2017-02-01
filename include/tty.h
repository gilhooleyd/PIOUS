/* tty.h
 *
 * Description: Describes functions that must be
 * implemented to interface with the terminal screen.
 */

#ifndef TTY_H
#define TTY_H


#include "globals.h"

#include <stddef.h>
#include <stdint.h>


bool_t terminal_init(void);
bool_t terminal_is_initialized(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
 

#endif

/* tty_stub.c
 *
 * Description: Stub for implementing the tty.h interface.
 */


#include "tty.h" 


bool_t terminal_init(void) {return FALSE;}
bool_t terminal_is_initialized(void) {return FALSE;}
void terminal_putchar(char c) {}
void terminal_write(const char* data, size_t size) {}
void terminal_writestring(const char* data) {}

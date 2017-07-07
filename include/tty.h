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
#include "framebuffer.h" 

struct fb_screen_t *main_screen;

bool_t terminal_init(struct fb_screen_t *fb);
bool_t terminal_is_initialized(struct fb_screen_t *fb);
void terminal_putchar(struct fb_screen_t *fb, char c);
void terminal_write(struct fb_screen_t *fb, const char* data, int size);
void terminal_writestring(struct fb_screen_t *fb, const char* data);
 
#endif

#include "teletext.h"
#include "framebuffer.h"
#include "tty.h"
#include "uart.h"

#define COLOR_BLACK 0x55550000
#define COLOR_WHITE 0xFFFFFFFF

void static inline DEBUG(char * s) {
    if (1) {
        uart_puts(s);
    }
}

// this char function assumes that each character is 5 bits across
// and 9 bits down
void writechar(struct fb_screen_t *fb, int x_loc, int y_loc, int letter) { //char * ch) {
    unsigned int cursor = fb->info.fb_addr  
        + (x_loc * 5 * 4) 
        + (y_loc * 9 * fb->info.fb_width * 4);
    int r, c;

    for (c = 0; c < 9; c++) {
        for (r = 4; r >= 0; r--) {
            if (teletext[letter - 32][c] & (1 << r)) {
                PUT32(cursor, COLOR_WHITE);
                DEBUG("*");
            }   
            else {
                PUT32(cursor, COLOR_BLACK);
                DEBUG(" ");
            }
            cursor += 4;
        }
        uart_puts("\n");
        cursor -= 4*5;
        cursor += 4*fb->info.fb_width;
    }
}

void inc_terminal_column(struct fb_screen_t *fb) {
    // todo fix these hieght width things
   if (++fb->write_x == fb->info.fb_width) {
        fb->write_x = 0;
        if (++fb->write_y == fb->info.fb_height) {
            fb->write_y = 0;
        }
   }
}

void terminal_putchar(struct fb_screen_t *fb, char c) {
    writechar(fb, fb->write_x, fb->write_y, c);
    inc_terminal_column(fb);
}

void terminal_write(struct fb_screen_t *fb, const char* data, int size) {
    int i;
    for (i = 0; i < size; i++)
        terminal_putchar(fb, data[i]);
}
void terminal_writestring(struct fb_screen_t *fb, const char* data) {
    while (*data != '\0'){
        terminal_putchar(fb, *data);
        data++;
    }
}

/* main.c
 *
 * Description: Defines the kernel main function.
 */


#include "globals.h"
#include "framebuffer.h"
#include "led.h"
#include "tty.h"
#include "io-x86.h"
#include "image_data.h"
#include "teletext.h"
#include "PS2.h"
#include "uart.h"

struct fb_screen_t *main_screen;

void printDigit(int dig){ 
    char codes[] = "0123456789ABCDEF";
    terminal_putchar(main_screen, codes[dig]);
}

void printHex(int num) {
    if (num < 0x10) {
        printDigit(num); 
        return;
    }
    else {
        printHex  (num / 0x10);
        printDigit (num % 0x10);
    }    
}

void printNum(int num) {
    if (num < 10) {
        printDigit(num); 
        return;
    }
    else {
        printNum  (num / 10);
        printDigit(num % 10);
    }    
}

/* Main function for the kernel - never returns */
void kernel_main(void)
{
//    int num_blinks = 5;
    struct fb_screen_t hdmi;
    main_screen = &hdmi;

    // Initialize modules
    uart_init();
    uart_puts("Hello World!\n");
    terminal_init(&hdmi);
    terminal_writestring(main_screen, "Hello World!");
//    led_init();
//    PS2_init();

//    led_blink();
//    led_blink();
//    led_blink();

    // Write a string to the terminal
//    terminal_writestring("Hello World!\nThis is a new line. "
//            "This is a tab\tAnd here's another.\tYay!!!!!!!\t:-)\n");

    while(1) {
//        terminal_putchar(PS2_readChar());
    }

    // Hang forever
    while(TRUE);
}

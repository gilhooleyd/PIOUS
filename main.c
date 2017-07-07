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

void printDigit(int dig){ 
    char codes[] = "0123456789ABCDEF";
    terminal_putchar(codes[dig]);
}

void printHex(int num) {
    int tmp = num;
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
    int tmp = num;
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
    int num_blinks = 5;

    // Initialize modules
    uart_init();
    uart_puts("Hello World!\n");
    terminal_init();
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

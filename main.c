/* main.c
 *
 * Description: Defines the kernel main function.
 */


#include "globals.h"
#include "framebuffer.h"
#include "led.h"
#include "tty.h"

#include "image_data.h"


/* Main function for the kernel - never returns */
void kernel_main(void)
{
    int num_blinks;

    // Initialize modules
    terminal_init();
    led_init();

    // Write a string to the terminal
    terminal_writestring("Hello World!\nThis is a new line. "
            "This is a tab\tAnd here's another.\tYay!!!!!!!\t:-)");

    // Blink a bunch
    num_blinks = 20;
    while (num_blinks-- > 0)
        led_blink();

    // Hang forever
    while(TRUE);
}
